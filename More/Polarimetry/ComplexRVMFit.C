/***************************************************************************
 *
 *   Copyright (C) 2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/ComplexRVMFit.h"
#include "Pulsar/PolnProfile.h"
#include "Pulsar/PhaseWeight.h"

#include "MEAL/ComplexRVM.h"
#include "MEAL/RotatingVectorModel.h"
#include "MEAL/ScalarParameter.h"
#include "MEAL/LevenbergMarquardt.h"

#include <assert.h>

using namespace std;

Pulsar::ComplexRVMFit::ComplexRVMFit()
{
  threshold = 3.0;
  chisq_map = false;
}

//! Set the threshold below which data are ignored
void Pulsar::ComplexRVMFit::set_threshold (float sigma)
{
  threshold = sigma;
}

//! Get the threshold below which data are ignored
float Pulsar::ComplexRVMFit::get_threshold () const
{
  return threshold;
}

//! Set the data to which model will be fit
void Pulsar::ComplexRVMFit::set_observation (const PolnProfile* _data)
{
  if (_data->get_state() != Signal::Stokes)
    throw Error (InvalidParam, "Pulsar::ComplexRVMFit::set_observation"
		 "data are not Stokes parameters");

  data = _data;

  if (model->get_nstate())
    model = 0;

  std::vector< std::complex< Estimate<double> > > linear;
  data->get_linear (linear, threshold);

  const unsigned nbin = data->get_nbin();

  int max_bin = data->get_Profile(0)->find_max_bin();

  peak_phase = (max_bin+ 0.5)*2*M_PI / nbin;
  unsigned count = 0;

  for (unsigned ibin=0; ibin < nbin; ibin++)
  {
    if (linear[ibin].real().get_variance() > 0)
    {
      data_x.push_back ( state.get_Value(count) );
      data_y.push_back ( linear[ibin] );
      
      double phase = (ibin + 0.5)*2*M_PI / nbin;
      double L = sqrt( norm(linear[ibin]).val );
      
      // cerr << "adding phase=" << phase << " L=" << L << endl;
      get_model()->add_state (phase, L);
      count ++;
    }

    if (int(ibin) == max_bin)
      peak_pa = 0.5 * atan2 (linear[ibin].imag().val, linear[ibin].real().val);
  }

  cerr << "peak phase=" << peak_phase*0.5/M_PI
       << " turns; PA=" << peak_pa*180/M_PI << " deg" << endl;

  state.signal.connect (model, &MEAL::ComplexRVM::set_state);
  
  assert( count == model->get_nstate() );

  cerr <<"Pulsar::ComplexRVMFit::set_observation "<< count <<" bins"<< endl;
}

//! Get the data to which model will be fit
const Pulsar::PolnProfile* Pulsar::ComplexRVMFit::get_observation () const try
{
  return data;
}
catch (Error& error)
{
  throw error += "Pulsar::ComplexRVMFit::get_observation";
}

//! Get the model to be fit to the data
MEAL::ComplexRVM* Pulsar::ComplexRVMFit::get_model ()
{
  if (!model)
    model = new MEAL::ComplexRVM;

  return model;
}

template<typename T, typename U>
double chisq (const Estimate<T,U>& e)
{
  return e.val * e.val / e.var;
}

//! Fit data to the model
void Pulsar::ComplexRVMFit::solve ()
{
  if (!model)
    model = new MEAL::ComplexRVM;

  MEAL::LevenbergMarquardt< complex<double> > fit;
  fit.verbose = MEAL::Function::verbose;

  // MEAL::Function::verbose = true;

  chisq = fit.init (data_x, data_y, *model);

  // cerr << "initial chisq = " << chisq << endl;

  float close = 1e-3;
  unsigned iter = 1;
  unsigned not_improving = 0;
  while (not_improving < 25)
  {
    // cerr << "iteration " << iter << endl;
    float nchisq = fit.iter (data_x, data_y, *model);
    // cerr << "     chisq = " << nchisq << endl;

    if (!isfinite (nchisq))
      throw Error (InvalidState, "Pulsar::ComplexRVMFit::solve",
		   "non-finite chisq");

    if (nchisq < chisq)
    {
      float diffchisq = chisq - nchisq;
      chisq = nchisq;
      not_improving = 0;
      if (diffchisq/chisq < close && diffchisq > 0)
	break;
    }
    else
      not_improving ++;

    iter ++;
  }

  check_parameters ();

  if (chisq_map)
    return;

  std::vector<std::vector<double> > covariance;
  fit.result (*model, covariance);

  nfree = 2 * model->get_nstate();
  for (unsigned iparm=0; iparm < model->get_nparam(); iparm++)
  {
    if (model->get_infit(iparm))
      nfree --;

    if (nfree == 0)
      throw Error (InvalidState, "Pulsar::ComplexRVMFit::solve",
		   "nfree <= 0");

    model->set_variance (iparm, 2.0*covariance[iparm][iparm]);
  }

#if 0

  double chisq2 = 0;
  double deg=180/M_PI;

  for (unsigned i=0; i < data_x.size(); i++)
  {
    data_x[i].apply();
    std::complex<double> model = cRVM->evaluate();
    double gain = cRVM->get_linear(i).get_value();

    cerr << i << " data=" << data_y[i] << " arg=" << arg(data_y[i])*deg/2
	 << " model=" << model << " PA=" << RVM->evaluate()*deg 
	 << " Q=" << gain*cos(2*RVM->evaluate())
	 << " U=" << gain*sin(2*RVM->evaluate()) << endl;

    data_y[i] -= model;

    chisq2 += ::chisq(data_y[i].real()) + ::chisq(data_y[i].imag());
  }

  cerr << "CHISQ=" << chisq2 << endl;
#endif

}

void Pulsar::ComplexRVMFit::check_parameters ()
{
  MEAL::ComplexRVM* cRVM = get_model();
  MEAL::RotatingVectorModel* RVM = cRVM->get_rvm();

  const unsigned nstate = cRVM->get_nstate();

  double total_linear = 0.0;
  for (unsigned i=0; i<nstate; i++)
    total_linear += cRVM->get_linear(i).get_value();

  if (total_linear < 0.0)
  {
    cerr << "correcting negative gain" << endl;
    for (unsigned i=0; i<nstate; i++)
      cRVM->set_linear(i,-cRVM->get_linear(i).get_value());

    double PA0 = RVM->reference_position_angle->get_value().get_value();
    if (PA0 < 0)
      PA0 += M_PI/2;
    else
      PA0 -= M_PI/2;

    RVM->reference_position_angle->set_value (PA0);
  }
}

void Pulsar::ComplexRVMFit::global_search (unsigned nstep)
{
  MEAL::ComplexRVM* cRVM = get_model();
  MEAL::RotatingVectorModel* RVM = cRVM->get_rvm();

  const unsigned nstate = cRVM->get_nstate();
  if (!nstate)
    throw Error (InvalidState, "Pulsar::ComplexRVMFit::global_search",
		 "no data");

  vector<double> linear (nstate);
  for (unsigned i=0; i<nstate; i++)
    linear[i] = cRVM->get_linear(i).get_value();

  double alpha_step = M_PI/(nstep-1);
  double zeta_step = M_PI/(nstep-2);

  float best_chisq = 0.0;
  float best_alpha = 0.0;
  float best_zeta = 0.0;
  
  if (chisq_map)
  {
    RVM->magnetic_axis->set_infit (0, false);
    RVM->line_of_sight->set_infit (0, false);
  }

  for (double alpha=alpha_step/2; alpha < M_PI; alpha += alpha_step)
    for (double zeta=zeta_step/2; zeta < M_PI; zeta += zeta_step) try
    {
      RVM->magnetic_axis->set_value (alpha);
      RVM->line_of_sight->set_value (zeta);

      // ensure that each attempt starts with the same guess
      RVM->magnetic_meridian->set_value (peak_phase);
      RVM->reference_position_angle->set_value (peak_pa);
      for (unsigned i=0; i<nstate; i++)
	cRVM->set_linear(i, linear[i]);

      solve ();

      if (chisq_map)
	cout << alpha << " " << zeta << " " << chisq << endl;

      if (best_chisq == 0 || chisq < best_chisq)
      {
	// cerr << "current best chisq=" << chisq << endl;
	best_chisq = chisq;
	best_alpha = alpha;
	best_zeta = zeta;
      }
    }
    catch (Error& error)
    {
      if (MEAL::Function::verbose)
	cerr << "exception thrown alpha=" << alpha << " zeta=" << zeta << endl
	     << error.get_message() << endl;
    }


  // cerr << "BEST chisq=" << best_chisq << endl;

  RVM->magnetic_axis->set_value (best_alpha);
  RVM->line_of_sight->set_value (best_zeta);

  chisq_map = false;

  solve ();

  for (unsigned i=0; i<nstate; i++)
    if (cRVM->get_linear(i).get_value() < 0)
      cerr << "orthogonal mode L=" << cRVM->get_linear(i).val 
	   << " phase=" << cRVM->get_phase(i)*0.5/M_PI  << endl;
}