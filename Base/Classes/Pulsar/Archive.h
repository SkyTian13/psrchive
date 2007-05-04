//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2002 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Classes/Pulsar/Archive.h,v $
   $Revision: 1.159 $
   $Date: 2007/05/04 23:33:47 $
   $Author: straten $ */

#ifndef __Pulsar_Archive_h
#define __Pulsar_Archive_h

#define PULSAR_ARCHIVE_REVISION "$Revision: 1.159 $"

#include "IntegrationManager.h"
#include "psrephem.h"
#include "sky_coord.h"
#include "Estimate.h"

#include "Types.h"

#include "toa.h"

template<typename T> class Jones;

namespace Pulsar {

  class Receiver;
  class Integration;
  class Profile;
  class Predictor;

  //! The primary interface to pulsar observational data
  /*! This virtual base class implements the primary interface to pulsar
    observational data, including the pulse profiles, integrations, and all
    auxilliary data.  All file I/O and various data reduction algorithms are
    accessed via this class. */
  class Archive : public IntegrationManager {

  public:

    // //////////////////////////////////////////////////////////////////
    //
    //
    // //////////////////////////////////////////////////////////////////

    //! Default constructor
    Archive ();

    //! Copy constructor
    Archive (const Archive& archive);
    
    //! Destructor
    virtual ~Archive ();

    //! Assignment operator
    Archive& operator = (const Archive& a);


    // //////////////////////////////////////////////////////////////////
    //
    // Factories, loading, unloading, and refreshing
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Factories
     * 
     * These factories return pointers to new instances of derived classes.
     */
    //@{

    //! Factory returns a null-constructed instance of the named class
    static Archive* new_Archive (const std::string& class_name);

    //! Factory returns a new instance loaded from filename
    static Archive* load (const std::string& name);

    //@}

    /** @name File I/O
     *  
     * These methods deal with loading and unloading data to and from file.
     */
    //@{

    //! Write the archive to filename
    void unload (const char* filename = 0) const;

    //! Convenience interface to Archive::unload (const char*)
    void unload (const std::string& filename) const
    { unload (filename.c_str()); }

    //! Get the name of the file to which the archive will be unloaded
    std::string get_filename () const { return unload_filename; }

    //! Set the filename of the Archive
    /*! The filename is the name of the file to which the archive will be 
      written on the next call to Archive::unload, if no arguments are given
      to the Archive::unload method. */
    void set_filename (const std::string& filename)
    { unload_filename = filename; }

    //! Update the current archive, saving current Integration data
    void update ();

    //! Completely reload the archive, deleting all data
    void refresh ();

    //@}

    // //////////////////////////////////////////////////////////////////
    //
    // Copying and cloning
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Copying and Cloning
     *
     *  The copy methods copy data from other Archive instances; the
     *  clone, extract, and total methods return pointers to new
     *  copy-constructed instances.
     */
    //@{

    //! Copy all base class attributes, Extensions, and Integrations
    void copy (const Archive& archive);

    //! Copy all base class attributes, Extensions, and selected Integrations
    virtual void copy (const Archive&, const std::vector<unsigned>& subints);

    //! Return a new copy constructed instance equal to this
    /*! This pure virtual method must be implemented by derived classes */
    virtual Archive* clone () const = 0;

    //! Return a new extraction constructed instance equal to this
    /*! This pure virtual method must be implemented by derived classes */
    virtual Archive* extract (const std::vector<unsigned>& subints) const = 0;

    //! Return pointer to a new fscrunched, tscrunched and pscrunched clone
    Archive* total () const;

    //@}

    // //////////////////////////////////////////////////////////////////
    //
    // Dimension Attributes
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Dimension Attributes
     *
     *  The pure virtual methods return the dimension attributes stored 
     *  by derived classes.  Note that the dimensions of an Archive may be 
     *  changed only through the resize method.
     */
    //@{

    //! Get the number of pulsar phase bins used
    virtual unsigned get_nbin () const = 0;

    //! Get the number of frequency channels used
    virtual unsigned get_nchan () const = 0;

    //! Get the number of frequency channels used
    virtual unsigned get_npol () const = 0;

    //! Resize the Integration vector with new_Integration instances
    virtual void resize (unsigned nsubint,
                         unsigned npol  = 0,
                         unsigned nchan = 0,
                         unsigned nbin  = 0);

    //@}

    // //////////////////////////////////////////////////////////////////
    //
    // Common Attributes
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Common Attributes
     *
     *  These pure virtual methods provide access to the common attributes
     *  stored by all derived classes.
     */
    //@{

    //! Get the tempo code of the telescope used
    virtual char get_telescope_code () const = 0;
    //! Set the tempo code of the telescope used
    virtual void set_telescope_code (char tempo_isite) = 0;

    //! Get the state of the profile data
    virtual Signal::State get_state () const = 0;
    //! Set the state of the profile data
    virtual void set_state (Signal::State state) = 0;

    //! Get the scale in which flux density is measured
    virtual Signal::Scale get_scale () const = 0;
    //! Set the scale in which flux density is measured
    virtual void set_scale (Signal::Scale scale) = 0;

    //! Get the observation type (psr, cal)
    virtual Signal::Source get_type () const = 0;
    //! Set the observation type (psr, cal)
    virtual void set_type (Signal::Source type) = 0;

    //! Get the source name
    virtual std::string get_source () const = 0;
    //! Set the source name
    virtual void set_source (const std::string& source) = 0;

    //! Get the coordinates of the source
    virtual sky_coord get_coordinates () const = 0;
    //! Set the coordinates of the source
    virtual void set_coordinates (const sky_coord& coordinates) = 0;

    //! Get the centre frequency of the observation
    virtual double get_centre_frequency () const = 0;
    //! Set the centre frequency of the observation
    virtual void set_centre_frequency (double cf) = 0;

    //! Get the overall bandwidth of the observation
    virtual double get_bandwidth () const = 0;
    //! Set the overall bandwidth of the observation
    virtual void set_bandwidth (double bw) = 0;

    //! Get the dispersion measure (in \f${\rm pc\, cm}^{-3}\f$)
    virtual double get_dispersion_measure () const = 0;
    //! Set the dispersion measure (in \f${\rm pc\, cm}^{-3}\f$)
    virtual void set_dispersion_measure (double dm) = 0;

    //! Inter-channel dispersion delay has been removed
    virtual bool get_dedispersed () const = 0;
    //! Set the value to be returned by get_dedispersed
    virtual void set_dedispersed (bool done = true) = 0;

    //! Get the rotation measure (in \f${\rm rad\, m}^{-2}\f$)
    virtual double get_rotation_measure () const = 0;
    //! Set the rotation measure (in \f${\rm rad\, m}^{-2}\f$)
    virtual void set_rotation_measure (double rm) = 0;

    //! Data has been corrected for ISM faraday rotation
    virtual bool get_faraday_corrected () const = 0;
    //! Set the value to be returned by get_ism_rm_corrected
    virtual void set_faraday_corrected (bool done = true) = 0;

    //! Data has been calibrated for polarimetric response of instrument
    virtual bool get_poln_calibrated () const = 0;
    //! Set the value to be returned by get_poln_calibrated
    virtual void set_poln_calibrated (bool done = true) = 0;

    //@}


    // //////////////////////////////////////////////////////////////////
    //
    // Access to data
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Data Access
     *
     *  These methods provide access to the data contained in the Archive.
     *  Note that get_Integration is implemented by the IntegrationManager
     *  base class.
     */
    //@{

    //! Return pointer to the specified profile
    Profile* get_Profile (unsigned subint, unsigned pol, unsigned chan);

    //! Return pointer to the specified profile
    const Profile*
    get_Profile (unsigned subint, unsigned pol, unsigned chan) const;

    //@}


    // //////////////////////////////////////////////////////////////////
    //
    // Pulsar Ephemeris and Predictor
    //
    // //////////////////////////////////////////////////////////////////

    /** @name TEMPO Interface
     *
     *  These methods provide access to the pulsar ephemeris and predictor
     *  as used by TEMPO.
     */
    //@{

    //! Install the given ephemeris and call update_model
    void set_ephemeris (const psrephem& ephemeris, bool update = true);

    //! Return a copy of the current archive ephemeris
    const psrephem get_ephemeris() const;

    //! Install the given predictor and shift profiles to align
    void set_model (const Predictor* model);

    //! Return a copy of the current archive predictor
    const Predictor* get_model() const;

    //! Returns true if the archive has a model
    bool has_model() const { return model; }

    //! Create a new predictor and align the Integrations to the new model
    void update_model ();

    //@}

    // //////////////////////////////////////////////////////////////////
    //
    // Basic algorithms
    //
    // //////////////////////////////////////////////////////////////////

    //! Integrate pulse profiles in phase
    void bscrunch (unsigned nscrunch);

    //! Integrate neighbouring sections of the pulse profiles
    void fold (unsigned nfold);

    //! Integrate profiles in polarization
    void pscrunch();

    //! Integrate profiles in frequency
    void fscrunch (unsigned nscrunch=0);

    //! Integrate profiles in time
    void tscrunch (unsigned nscrunch=0);

    //! Phase rotate each profile by time seconds
    void rotate (double time);

    //! Rotate each profile by phase
    void rotate_phase (double phase);

    //! Append the Integrations from the specifed archive
    void append (const Archive* archive);

    //! Phase rotate pulsar Integrations so that pulse phase zero is centred
    void centre ();

    //! Phase rotate pulsar Integrations so centre the maximum amplitude
    void centre_max_bin (); 

    //! Convert data to the specified state
    void convert_state (Signal::State state);

    //! Convenience interface to Receiver::get_basis
    Signal::Basis get_basis () const;

    //! Rotate the Profiles to remove dispersion delays b/w chans
    void dedisperse ();

    //! Correct the Faraday rotation of Q into U
    void defaraday ();
    
    //! Fit Profiles to the standard and return toas
    void toas (std::vector<Tempo::toa>& toas, const Archive* std,
		       std::string arguments = "",
		       Tempo::toa::Format fmt = Tempo::toa::Parkes) const;
    
    //! Perform the transformation on each polarimetric profile
    void transform (const Jones<float>& transformation);

    //! Perform frequency response on each polarimetric profile
    void transform (const std::vector< Jones<float> >& response);

    //! Perform the time and frequency response on each polarimetric profile
    void transform (const std::vector< std::vector< Jones<float> > >& response);

    //! Transform Stokes I,Q,U,V into the polarimetric invariant interval
    void invint ();
 
    //! Remove the baseline from all profiles
    void remove_baseline (float phase = -1.0, float dc = 0.15);

    //! Set the weight of each profile to the given number
    void uniform_weight (float new_weight = 1.0);

    //! Test if arch matches (enough for a pulsar - standard match)
    bool standard_match (const Archive* arch, std::string& reason) const;

    //! Test if arch matches (enough for a pulsar - calibrator match)
    bool calibrator_match (const Archive* arch, std::string& reason) const;

    //! Test if arch matches (enough for a pulsar - pulsar match)
    bool processing_match (const Archive* arch, std::string& reason) const;

    //! Test if arch is mixable (enough for append)
    bool mixable (const Archive* arch, std::string& reason) const;

    //! Computes the weighted channel frequency over an Integration interval.
    double weighted_frequency (unsigned ichan,
			       unsigned start, unsigned end) const;

    //! Call bscrunch with the appropriate value
    void bscrunch_to_nbin (unsigned new_nbin);

    //! Call fscrunch with the appropriate value
    void fscrunch_to_nchan (unsigned new_nchan);

    //! Call tscrunch with the appropriate value
    void tscrunch_to_nsub (unsigned new_nsub);
    
    //! Return the MJD at the start of the first sub-integration
    MJD  start_time () const;

    //! Return the MJD at the end of the last sub-integration
    MJD  end_time () const;

    //! Returns the total time integrated into all Integrations (in seconds)
    double integration_length() const;

    //! Return true if the observation is a calibrator
    bool type_is_cal () const;

    //! Find the transitions between high and low states in total intensity
    void find_transitions (int& hi2lo, int& lo2hi, int& buffer) const;

    //! Find the bins in which the total intensity exceeds a threshold
    void find_peak_edges (int& rise, int& fall) const;

    //! Returns the centre phase of the region with maximum total intensity
    float find_max_phase () const;

    //! Returns the centre phase of the region with minimum total intensity
    float find_min_phase (float dc = 0.15) const;

    //! Returns geometric mean of baseline RMS for each Integration
    float rms_baseline (float dc = 0.4);

    //! A dsp::Transformation into an Archive must be able to call this
    //! This calls Signal::valid_state() to see if the state is consistent with the ndim, npol
    bool state_is_valid (std::string& reason) const;


    // //////////////////////////////////////////////////////////////////
    //
    // Archive Extension interface
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Extension Interface 
     *
     * Derived classes can provide access to the additional
     * information available in their associated file format through
     * use of Extension classes. 
     */
    //@{
    
    //! Adds features or data to Archive instances
    /* Archive-derived classes may provide access to additional informaton
       through Extension-derived objects. */
    class Extension : public Reference::Able {

    public:

      //! Construct with a name
      Extension (const char* name);

      //! Destructor
      virtual ~Extension ();

      //! Return a new copy-constructed instance identical to this instance
      virtual Extension* clone () const = 0;

      //! Return the name of the Extension
      std::string get_extension_name () const;

    protected:

      //! Extension name - useful when debugging
      std::string extension_name;

    };

    //! Return the number of extensions available
    virtual unsigned get_nextension () const;

    //! Return a pointer to the specified extension
    virtual const Extension* get_extension (unsigned iextension) const;

    //! Return a pointer to the specified extension
    virtual Extension* get_extension (unsigned iextension);

    //! Template method searches for an Extension of the specified type
    template<class ExtensionType>
    const ExtensionType* get () const;

    //! Template method searches for an Extension of the specified type
    template<class ExtensionType>
    ExtensionType* get ();

    //! Template method returns an Extension of the specified type
    template<class ExtensionType>
    ExtensionType* getadd ();

    //! Add an Extension to the Archive instance
    /*! This method ensures that only one instance of the Extension
      type is stored in the Archive. */
    virtual void add_extension (Extension* extension);

    //@}

    // //////////////////////////////////////////////////////////////////
    //
    // Class members
    //
    // //////////////////////////////////////////////////////////////////

    //! Return the revision number of the Archive base class definition
    /*! This string is automatically generated by CVS.  Do not edit. */
    static std::string get_revision ()
    { return get_revision(PULSAR_ARCHIVE_REVISION); }

    //! Report on the status of the plugins
    static void agent_report ();

    //! List the successfully loaded plugins
    static void agent_list ();

    //! Set the verbosity level (0 to 3)
    static void set_verbosity (unsigned level);

    //! Sanity checks such as verification and correction
    class Check;

    // //////////////////////////////////////////////////////////////////
    //
    // Control flags
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Control Flags
     *
     * These flags control the default behaviour of Archive methods.
     */
    //@{

    //! Archive::append should enforce chronological order
    static bool append_chronological;

    //! Archive::append should throw exception if Archive::match fails
    static bool append_must_match;

    //! Amount by which integration intervals may overlap in Archive::append
    static double append_max_overlap;

    //@}

    //! Provides access to certain protected and private methods
    class Expert;

    //! Provide access to the expert interface
    Expert* expert ();

  protected:

    friend class Expert;

    //! Parses the revision number out of the CVS Revision string
    static std::string get_revision (const char* revision);
 
    // //////////////////////////////////////////////////////////////////
    //
    // File I/O
    //
    // //////////////////////////////////////////////////////////////////

    /** @name File I/O
     *  
     * These pure virtual methods must be defined by derived classes
     * in order to handle loading and unloading data to and from
     * specific file formats.
     */
    //@{

    //! Load the header information from filename
    virtual void load_header (const char* filename) = 0;

    //! Load the specified Integration from filename, returning new instance
    virtual Integration*
    load_Integration (const char* filename, unsigned subint) = 0;

    //! Unload the Archive (header and Integration data) to filename
    virtual void unload_file (const char* filename) const = 0;

    //@}

    //! Name of file to which the archive will be written on call to unload()
    std::string unload_filename;

    // //////////////////////////////////////////////////////////////////
    //
    // Dimension Attributes
    //
    // //////////////////////////////////////////////////////////////////

    /** @name Dimension Attributes
     *
     *  These pure virtual methods are used by the Archive class to set
     *  the dimension attributes stored by the derived classes.  
     */
    //@{

    //! Set the number of pulsar phase bins
    virtual void set_nbin (unsigned numbins) = 0;

    //! Set the number of frequency channels
    virtual void set_nchan (unsigned numchan) = 0;

    //! Set the number of polarization measurements
    virtual void set_npol (unsigned numpol) = 0;

    //@}

    //! The pulsar ephemeris, as used by TEMPO
    Reference::To<psrephem> ephemeris;

    //! The pulsar phase model, as created using TEMPO
    Reference::To<Predictor> model;

    //! Initialize an Integration to reflect Archive attributes.
    void init_Integration (Integration* subint);

    //! Provide Integration::resize access to Archive-derived classes
    void resize_Integration (Integration* integration);
    
    //! Apply the current model to the Integration
    void apply_model (Integration* subint, const Predictor* old = 0);

    //! Update the predictor model and correct the Integration set
    void update_model (unsigned old_nsubint);

    //! Update the predictor to include the specifed MJD
    void update_model (const MJD& time, bool clear_model = false);

    //! Creates polynomials to span the Integration set
    void create_updated_model (bool clear_model);

    //! Returns true if the given model spans the Integration set
    bool good_model (const Predictor* test_model) const;

    //! Expert interface
    Reference::To<Expert> expert_interface;

  private:

    //! The Extensions added to this Archive instance
    std::vector< Reference::To<Extension> > extension;

    //! This flag may be raised only by Archive::update_model.
    /*!
      As it is set only during run-time, this flag makes it known that
      the current predictor was created by the currently available
      version of tempo and its run-time configuration files.
    */
    bool runtime_model;

    //! Store the name of the file from which the current instance was loaded
    /*! Although the logical name of the file may be changed with
      Archive::set_filename, the base class must keep track of the
      original file in order to read unloaded information from this
      file when it is required. */
    std::string __load_filename;

    //! Load a new instance of the specified integration from __load_filename
    Integration* load_Integration (unsigned isubint);

    //! Set all values to null
    void init ();

    //! Perform all known correction operations listed in Check::registry
    void correct ();

    //! Perform all known verification operations listed in Check::registry
    void verify () const;

    // Advocates the use of an Archive derived class
    class Agent;

  protected:

    // Advocates the use of Plugin
    template<class Plugin> class Advocate;

  };

  /*! e.g. MyExtension* ext = archive->get<MyExtension>(); */
  template<class ExtensionType>
  const ExtensionType* Archive::get () const
  {
    const ExtensionType* extension = 0;

    for (unsigned iext=0; iext<get_nextension(); iext++) {

      const Extension* ext = get_extension (iext);

      if (verbose == 3)
	std::cerr << "Pulsar::Archive::get<Ext> name=" 
	          << ext->get_extension_name() << std::endl;

      extension = dynamic_cast<const ExtensionType*>( ext );

      if (extension)
	return extension;
    }

    if (verbose==3)
      std::cerr << "Pulsar::Archive::get<Ext> failed to find extension of type "
	        << typeid(extension).name() << std::endl;

    return extension;
  }

  template<class ExtensionType>
  ExtensionType* Archive::get ()
  {
    const Archive* thiz = this;
    return const_cast<ExtensionType*>( thiz->get<ExtensionType>() );
  }

  /*! If the specified ExtensionType does not exist, an atempt is made to
      add it using add_extension.  If this fails, NULL is returned. */
  template<class ExtensionType>
  ExtensionType* Archive::getadd ()
  {
    const Archive* thiz = this;
    ExtensionType* retv = 0;
    retv = const_cast<ExtensionType*>( thiz->get<ExtensionType>() );

    if (retv)
      return retv;

    try {
      Reference::To<ExtensionType> add_ext = new ExtensionType;
      add_extension (add_ext);
      return add_ext;
    }
    catch (Error& error) {
      return retv;
    }

  }

}

#endif

