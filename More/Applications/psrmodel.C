/***************************************************************************
 *
 *   Copyright (C) 2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Pulsar/Application.h"
#include "Pulsar/StandardOptions.h"

#include "Pulsar/Archive.h"
#include "Pulsar/Integration.h"
#include "Pulsar/PolnProfile.h"
#include "Pulsar/FaradayRotation.h"
#include "Pulsar/Interpreter.h"

#if HAVE_PGPLOT
#include "Pulsar/PlotOptions.h"
#include "Pulsar/StokesCylindrical.h"
#include "Pulsar/PlotAnnotation.h"
#include <cpgplot.h>
#endif

#include "Pulsar/ComplexRVMFit.h"
#include "MEAL/ComplexRVM.h"
#include "MEAL/RotatingVectorModel.h"
#include "MEAL/ScalarParameter.h"

#include "pairutil.h"

#include <fstream>

using namespace Pulsar;
using namespace std;

//! Pulsar modeling application
class psrmodel: public Pulsar::Application
{
public:

  //! Default constructor
  psrmodel ();

  //! Add a range of pulse longitude to include in fit
  void add_include (const std::string& arg);

  //! Add a range of pulse longitude to exclude from fit
  void add_exclude (const std::string& arg);

  //! Add an orthogonally polarized mode
  void add_opm (const std::string& arg);

  //! Produce a chi-sqared map
  void map_chisq ();

  //! Verify setup
  void setup ();

  //! Process the given archive
  void process (Pulsar::Archive*);

  //! Output the post-fit residuals to file
  void output_residuals ();

protected:

  //! Add command line options
  void add_options (CommandLine::Menu&);

  void set_very_verbose () 
  { 
    ComplexRVMFit::verbose = true; 
    Application::set_very_verbose(); 
    Plot::verbose=true; 
  }

  // complex rotating vector model
  Reference::To<ComplexRVMFit> rvmfit;

  // perform a global search over first guesses in alpha and zeta
  string global_search;
  unsigned nalpha;
  unsigned nzeta;

  // perform a rotating vector model fit
  bool fit_rvm;

  // output the post-fit Stokes Q and U residuals
  bool output_QU_residuals;
  // output the post-fit position angle residuals
  bool output_psi_residuals;

#if HAVE_PGPLOT

  // plot the result
  bool plot_result;
  void set_plot_result ()
  {
    plot_result = true; 
    plot.set_open_device (true);
  }

  // configures the plotting device
  Pulsar::PlotOptions plot;

  void set_plot_options(const std::string& arg);

  // Extra config options for the profile plot
  vector<string> plot_config;

  // Put best-fit params on plot
  bool label_plot;

  // Put angle lines on plot
  bool plot_pa_lines;

  // Proper motion direction and error in val,err format
  string proper_motion;

#endif
};

#if HAVE_PGPLOT
// Following classes are for various plot annotation
class err1: public Pulsar::PlotAnnotation
{
  public:
    err1(float _x, float _y, float _yerr, int _ci=2) 
      { x=_x; y=_y; yerr=_yerr; ci=_ci; }
    float x;
    float y;
    float yerr;
    int ci;
    void draw(const Archive *data)
    {
      cpgsci(ci);
      cpgsls(1);
      cpgerr1(6,x,y,yerr,1.0);
    }
};

class line: public Pulsar::PlotAnnotation
{
  public:
    enum linetype
    {
      Vertical,
      Horizontal
    };
    line(linetype _t, float _v, int _ls=1, int _ci=1) 
      { type=_t; val=_v; ls=_ls; ci=_ci; }
    float val;
    int ls;
    int ci;
    linetype type;
    void draw(const Archive *data)
    {
      int oldls, oldci;
      cpgqls(&oldls);
      cpgqci(&oldci);

      cpgsls(ls);
      cpgsci(ci);

      float x[2], y[2];
      cpgqwin(&x[0], &x[1], &y[0], &y[1]);

      if (type==Vertical) { x[0]=x[1]=val; }
      else if (type==Horizontal) { y[0]=y[1]=val; }
      cpgline(2, x, y);

      cpgsls(oldls);
      cpgsci(oldci);
    }
};
#endif

int main (int argc, char** argv)
{
  psrmodel program;
  return program.main (argc, argv);
}

psrmodel::psrmodel () :
  Pulsar::Application ("psrmodel", "pulsar modeling program")
#if HAVE_PGPLOT
  , plot (false)
#endif
{
  has_manual = false;
  version = "$Id: psrmodel.C,v 1.13 2010/05/28 21:56:32 straten Exp $";

  Pulsar::StandardOptions* preprocessor = new Pulsar::StandardOptions;
  preprocessor->get_interpreter()->allow_infinite_frequency = true;
  add( preprocessor );
#if HAVE_PGPLOT
  add( &plot );
  plot_result = false;
  label_plot = false;
  plot_pa_lines = false;
#endif

  rvmfit = new ComplexRVMFit;
  fit_rvm = true;

  output_QU_residuals = false;
  output_psi_residuals = false;

  nalpha = nzeta = 0;
}

double deg_to_rad (const std::string& arg)
{
  return fromstring<double> (arg) * M_PI/180.0;
}

range range_deg_to_rad (const std::string& arg)
{
  range result = fromstring<range> (arg);
  result.first *= M_PI/180;
  result.second *= M_PI/180;

  return result;
}

void psrmodel::add_options (CommandLine::Menu& menu)
{
  MEAL::RotatingVectorModel* RVM = rvmfit->get_model()->get_rvm();
  fit_rvm = true;

  CommandLine::Argument* arg;

  // blank line in help
  menu.add ("");

#if HAVE_PGPLOT
  arg = menu.add (this, &psrmodel::set_plot_result, 'd');
  arg->set_help ("plot the resulting model with data");

  arg = menu.add (label_plot, 'l');
  arg->set_help ("label plot with best-fit parameter values");

  arg = menu.add (plot_pa_lines, 'L');
  arg->set_help ("plot lines at best-fit phi0 and psi0 angles");

  arg = menu.add (proper_motion, "pm", "val+-err[:ci]");
  arg->set_help("label PA plot with proper motion direction (ci = pgplot color index)");

  arg = menu.add (this, &psrmodel::set_plot_options, 'c', "cfg[s]");
  arg->set_help ("set additional plot options");
#endif

  arg = menu.add (rvmfit.get(), &ComplexRVMFit::set_threshold, 't', "sigma");
  arg->set_help ("cutoff threshold when selecting bins "
		 "[default " + tostring(rvmfit->get_threshold()) + "]");

  arg = menu.add (rvmfit.get(), &ComplexRVMFit::set_guess_smooth,
		  "smooth", "bins");
  arg->set_help ("smoothing factor used to stabilize first guess "
		 "[default " + tostring(rvmfit->get_guess_smooth()) + "]");

  arg = menu.add (this, &psrmodel::add_exclude, "exclude", "deg0:deg1");
  arg->set_help ("add a range of pulse longitude to exclude from fit");

  arg = menu.add (this, &psrmodel::add_include, "include", "deg0:deg1");
  arg->set_help ("add a range of pulse longitude to include in fit");

  arg = menu.add (this, &psrmodel::add_opm, 'o', "deg0:deg1");
  arg->set_long_name ("opm");
  arg->set_help ("add a range over which an orthogonal mode dominates");

  menu.add ("");

  arg = menu.add (RVM->magnetic_axis.get(),
		  &MEAL::ScalarParameter::set_value,
		  deg_to_rad, 'a', "degrees");
  arg->set_help ("alpha: colatitude of magnetic axis");

  arg = menu.add (RVM->magnetic_axis.get(),
		  &MEAL::ScalarParameter::set_fit, 'A', false);
  arg->set_help ("hold alpha constant");

  arg = menu.add (RVM->line_of_sight.get(),
		  &MEAL::ScalarParameter::set_value,
		  deg_to_rad, 'z', "degree");
  arg->set_help ("zeta: colatitude of line of sight");

  arg = menu.add (RVM->line_of_sight.get(),
		  &MEAL::ScalarParameter::set_fit, 'Z', false);
  arg->set_help ("hold zeta constant");

  arg = menu.add (RVM->magnetic_meridian.get(),
		  &MEAL::ScalarParameter::set_value,
		  deg_to_rad, 'b', "degrees");
  arg->set_help ("phi0: longitude of magnetic meridian");

  arg = menu.add (RVM->magnetic_meridian.get(),
		  &MEAL::ScalarParameter::set_fit, 'B', false);
  arg->set_help ("hold phi0 constant");

  arg = menu.add (RVM->reference_position_angle.get(),
		  &MEAL::ScalarParameter::set_value,
		  deg_to_rad, 'p', "degrees");
  arg->set_help ("psi0: position angle at magnetic meridian");

  arg = menu.add (RVM->reference_position_angle.get(),
		  &MEAL::ScalarParameter::set_fit, 'P', false);
  arg->set_help ("hold psi0 constant");

  menu.add ("");
  arg = menu.add (RVM, &MEAL::RotatingVectorModel::use_impact,
		  "use_beta", true);
  arg->set_help ("vary beta (not zeta) as a free model parameter");

  menu.add ("\n" "chi^2 map options:");

  arg = menu.add (global_search, 's', "NxM");
  arg->set_help ("map an NxM grid in alpha X zeta/beta");

  arg = menu.add (this, &psrmodel::map_chisq, 'x');
  arg->set_help ("output map to stdout: alpha, zeta/beta, chi^2");

  arg = menu.add (rvmfit.get(), &ComplexRVMFit::set_range_alpha,
		  range_deg_to_rad, "alpha", "deg0:deg1");
  arg->set_help ("range of alpha on x-axis of grid");

  arg = menu.add (rvmfit.get(), &ComplexRVMFit::set_range_beta,
		  range_deg_to_rad, "beta", "deg0:deg1");
  arg->set_help ("range of beta on y-axis of grid");

  arg = menu.add (rvmfit.get(), &ComplexRVMFit::set_range_zeta,
		  range_deg_to_rad, "zeta", "deg0:deg1");
  arg->set_help ("range of zeta on y-axis of grid");


  menu.add ("\n" "residual output options:");

  arg = menu.add (output_QU_residuals, "resid");
  arg->set_help ("output post-fit Stokes Q and U residuals");

  arg = menu.add (output_psi_residuals, "psi-resid");
  arg->set_help ("output post-fit position angle residuals");

}



void psrmodel::map_chisq()
{
  rvmfit->set_chisq_map (true);
}


void psrmodel::add_include (const std::string& arg)
{
  range incl = range_deg_to_rad (arg);
  cerr << "psrmodel: include " << arg << " degrees" << endl;

  rvmfit->add_include( incl );
}

void psrmodel::add_exclude (const std::string& arg)
{
  range excl = range_deg_to_rad (arg);
  cerr << "psrmodel: exclude " << arg << " degrees" << endl;

  rvmfit->add_exclude( excl);
}

void psrmodel::add_opm (const std::string& arg)
{
  range opm = range_deg_to_rad (arg);
  cerr << "psrmodel: OPM at " << arg << " degrees" << endl;

  rvmfit->add_opm( opm );
}

#if HAVE_PGPLOT
void psrmodel::set_plot_options (const std::string& arg)
{
  vector<string> opts;
  separate(arg, opts, ",");
  for (unsigned i=0; i<opts.size(); i++)
    plot_config.push_back(opts[i]);
}
#endif

void psrmodel::setup ()
{
  if (!fit_rvm)
    throw Error (InvalidState, "psrmodel",
		 "please use -r (can only do RVM fit for now)");

  if (!global_search.empty())
  {
    const char* str = global_search.c_str();
    char separator = 0;

    if ( sscanf (str, "%u%c%u", &nalpha, &separator, &nzeta) == 3 )
      {}
    else if ( sscanf (str, "%u", &nalpha) == 1 )
    {
      // avoid alpha == zeta pole crossing
      nzeta = nalpha - 1;
    }
    else
      throw Error (InvalidState, "psrmodel",
		   "cannot parse chi^2 map dimensions from '%s'", str);

    cerr << "psrmodel: search " << nalpha << "X" << nzeta << " grid" << endl;
  }
}

static const double deg = 180/M_PI;

template<class T>
string state (T& model)
{
  ostringstream os;

  os << ((model->get_infit(0))? "[fit]":"[fix]")
     << " = " << deg*model->get_param(0);

  return os.str();
}

void psrmodel::process (Pulsar::Archive* data)
{
  if (verbose)
    cerr << "psrmodel: fitting " << data->get_filename() << endl;

  // correct PA to infinite frequency
  FaradayRotation xform;
  xform.set_reference_wavelength( 0 );
  xform.set_measure( data->get_rotation_measure() );
  xform.execute( data );

  data->tscrunch();
  data->fscrunch();
  data->convert_state(Signal::Stokes);
  data->remove_baseline();

  Reference::To<PolnProfile> p = data->get_Integration(0)->new_PolnProfile(0);
  rvmfit->set_observation (p);

  MEAL::RotatingVectorModel* RVM = rvmfit->get_model()->get_rvm();

  if (nalpha && nzeta)
  {
    cerr << "psrmodel: performing search of chi^2 map" << endl;

    rvmfit->global_search (nalpha, nzeta);
  }
  else
  {
    cerr << "psrmodel: solving with initial guess: \n"
      "psi_0 " << state(RVM->reference_position_angle) << " deg\n";

    if (RVM->impact)
      cerr << "beta  " << state(RVM->impact) << " deg\n";
    else
      cerr << "zeta  " << state(RVM->line_of_sight) << " deg\n";

    cerr <<
      "alpha " << state(RVM->magnetic_axis) << " deg\n"
      "phi_0 " << state(RVM->magnetic_meridian) << " deg"
	 << endl;

#if HAVE_PGPLOT && _DEBUG
  if (plot_result)
  {
    cerr << "Plotting initial guess" << endl;
    StokesCylindrical plotter;

    AnglePlot* pa = plotter.get_orientation();

    pa->set_threshold( rvmfit->get_threshold() );
    pa->model.set (rvmfit.get(), &ComplexRVMFit::evaluate);
    pa->get_frame()->get_y_scale()->set_range_norm (0, 1.5);

    plotter.get_scale()->set_units( PhaseScale::Degrees );
    for (unsigned i=0; i<plot_config.size(); i++)
      plotter.configure(plot_config[i]);
    plotter.plot( data );

    cerr << "Hit <ENTER> to continue" << endl;
    getchar();
  }
#endif

    rvmfit->solve();
  }

  cerr << endl
       << "chisq=" << rvmfit->get_chisq() << "/nfree=" << rvmfit->get_nfree()
       << " = " << rvmfit->get_chisq()/ rvmfit->get_nfree() 
       << endl;

  cerr <<
    "psi_0=" << deg*RVM->reference_position_angle->get_value() << " deg\n";

  if (RVM->impact)
    cerr << "beta =" << deg*RVM->impact->get_value() << " deg\n";
  else
    cerr << "zeta =" << deg*RVM->line_of_sight->get_value() << " deg\n";

  cerr <<
    "alpha=" << deg*RVM->magnetic_axis->get_value() << " deg\n"
    "phi_0=" << deg*RVM->magnetic_meridian->get_value() << " deg"
	     << endl;

  output_residuals();

#if HAVE_PGPLOT
  if (plot_result)
  {
    StokesCylindrical plotter;

    AnglePlot* pa = plotter.get_orientation();

    pa->set_threshold( rvmfit->get_threshold() );
    pa->model.set (rvmfit.get(), &ComplexRVMFit::evaluate);
    pa->get_frame()->get_y_scale()->set_range_norm (0, 1.5);

    plotter.get_scale()->set_units( PhaseScale::Degrees );
    for (unsigned i=0; i<plot_config.size(); i++)
      plotter.configure(plot_config[i]);

    if (label_plot) 
    {
      StokesPlot* flux = plotter.get_flux();
      char label[256];
      sprintf(label, 
          "\\\\ga=%.1f\xB0\n"
          "\\\\g%c=%.1f\xB0\n"
          "\\\\gf=%.1f\xB0\n"
          "\\\\gq=%.1f\xB0", 
          deg*RVM->magnetic_axis->get_value().get_value(),
          RVM->impact ? 'b' : 'z', 
          RVM->impact ? 
            deg*RVM->impact->get_value().get_value() :
            deg*RVM->line_of_sight->get_value().get_value(),
          deg*RVM->magnetic_meridian->get_value().get_value(),
          deg*RVM->reference_position_angle->get_value().get_value());
      flux->get_frame()->get_label_below()->set_right(label);
    }

    if (plot_pa_lines)
    {
      pa->add_annotation(new line(line::Horizontal,
            deg*RVM->reference_position_angle->get_value().get_value(),2,1));
      pa->add_annotation(new line(line::Vertical,
            deg*RVM->magnetic_meridian->get_value().get_value(),2,1));
    }

    if (proper_motion.length()) 
    {
      int ii, pm_ci = 2;
      if ((ii=proper_motion.find(':')) != string::npos) 
        pm_ci = fromstring<int>(proper_motion.substr(ii+1));
      Estimate<float> pm;
      pm = fromstring< Estimate<float> >(proper_motion.substr(0,ii));
      pa->add_annotation(new line(line::Horizontal,pm.get_value(),1,pm_ci));
      pa->add_annotation(new err1(180.0,pm.get_value(),pm.get_error(),pm_ci));
    }

    plotter.plot( data );

  }
#endif

}


void psrmodel::output_residuals ()
{
  if (output_QU_residuals)
  {
    string filename = "psrmodel_QU_residuals.txt";
    ofstream out (filename.c_str());
    if (!out)
      throw Error (FailedSys, "psrmodel", "could not open " + filename);
    else
      cerr << "\n" "post-fit Stokes Q and U residuals in " << filename << endl;

    vector<double> phases;
    vector< complex< Estimate<double> > > residuals;

    rvmfit->get_residuals (phases, residuals);

    for (unsigned i=0; i < residuals.size(); i++)
    {
      out << phases[i] << " " 
	  << residuals[i].real().get_value() << " "
	  << residuals[i].real().get_error() << " "
	  << residuals[i].imag().get_value() << " "
	  << residuals[i].imag().get_error() << endl;
    }
  }

  if (output_psi_residuals)
  {
    string filename = "psrmodel_psi_residuals.txt";
    ofstream out (filename.c_str());
    if (!out)
      throw Error (FailedSys, "psrmodel", "could not open " + filename);
    else
      cerr << "\n" "post-fit position angle residuals in " << filename << endl;

    vector<double> phases;
    vector< Estimate<double> > residuals;

    rvmfit->get_psi_residuals (phases, residuals);

    for (unsigned i=0; i < residuals.size(); i++)
    {
      out << phases[i] << " "
	   << residuals[i].get_value() << " "
	   << residuals[i].get_error() << endl;
    }
  }
}
