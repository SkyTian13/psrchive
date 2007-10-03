//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Classes/Pulsar/Calibrator.h,v $
   $Revision: 1.4 $
   $Date: 2007/10/03 06:14:13 $
   $Author: straten $ */

#ifndef __Calibrator_H
#define __Calibrator_H

#include "Pulsar/Config.h"

#include <vector>

#include "Estimate.h"
#include "Jones.h"
#include "MJD.h"

namespace Pulsar {

  class Archive;
  class Integration;
  class CalibratorExtension;

  //! Polarization and flux calibrators
  class Calibrator : public Reference::Able {
    
  public:

    //! Types of Models supported
    enum Type {
      //! Flux calibrator
      Flux,
      //! Instrumental corrections
      Corrections,
      //! Gain, differential gain and differential phase
      SingleAxis,
      //! Gain, 3-D boost, and two rotations (van Straten 2002)
      Polar,
      //! Polar decomposition (Hamaker 2000)
      Hamaker,
      //! Phenomenological decomposition, (Britton 2000)
      Britton,
      //! Hybrid combines SingleAxis and Britton/Hamaker (Ord et al. 2004)
      Hybrid,
      //! Degree of Polarization Calibrator (P236)
      DoP,
      //! Off-pulse Calibrator (P236)
      OffPulse
    };

    
    /** @name global linkage 
     *  In order that these static methods may be linked into plugin
     *  libraries, they are defined in Extension/PolnCalibratorExtension.C */
    //@{

    //! Convert Type to string
    static const char* Type2str (Type type);

    //! Convert string to Type
    static Type str2Type (const char* s);

    //@}

    //! Verbosity level
    static unsigned verbose;

    //! Fractional bandwidth of the window used in median filter
    static Pulsar::Option<float> median_smoothing;

    //! Fractional bandwidth over which interpolation will be performed
    static Pulsar::Option<float> interpolating;

    //! Tolerance to non-physical values of the coherency matrix
    static Pulsar::Option<float> det_threshold;

    //! Default constructor
    Calibrator ();
    
    //! Destructor
    virtual ~Calibrator ();

    //! Calibrate the Pulsar::Archive
    virtual void calibrate (Archive* archive) = 0;

    //! Return the Calibrator::Type of derived class
    virtual Type get_type () const = 0;

    //! Get the number of frequency channels in the calibrator
    virtual unsigned get_nchan () const = 0;

    //! Return a new Calibrator Extension
    virtual CalibratorExtension* new_Extension () const = 0;

    //! Return a new processed calibrator Archive with a CalibratorExtension
    Archive* new_solution (const std::string& archive_class) const;

    //! Return the reference epoch of the calibration experiment
    virtual MJD get_epoch () const;

    //! Return a string containing the file information
    virtual std::string get_filenames () const;

    //! Return a const reference to the calibrator archive
    const Archive* get_Archive () const;

    //! Communicates Calibrator parameters to plotting routines
    class Info : public Reference::Able {

    public:

      //! Destructor
      virtual ~Info () {}

      //! Return the number of parameter classes
      virtual unsigned get_nclass () const = 0;

      //! Return the name of the specified class
      virtual const char* get_name (unsigned iclass) const = 0;

      //! Return the number of parameters in the specified class
      virtual unsigned get_nparam (unsigned iclass) const = 0;

      //! Return the estimate of the specified parameter
      virtual Estimate<float> get_param (unsigned ichan, unsigned iclass,
					 unsigned iparam) const = 0;

      /** @name Optional Graphing Features
       *  These optional attributes make the graph look nice
       */
      //@{

      //! Return the colour index
      virtual int get_colour_index (unsigned iclass, unsigned iparam) const
      { return iparam + 1; }

      //! Return the graph marker
      virtual int get_graph_marker (unsigned iclass, unsigned iparam) const
      { return -1; }

      //@}

    };

    //! Return the Calibrator::Info information
    /*! By default, derived classes need not necessarily define Info */
    virtual Info* get_Info () const { return 0; }

  protected:

    //! The CalibratorExtension of the Archive passed during construction
    Reference::To<const CalibratorExtension> extension;

    //! Filenames of Pulsar::Archives from which instance was created
    std::vector<std::string> filenames;

    //! Provide derived classes with access to the calibrator
    const Archive* get_calibrator () const;

    //! Provide derived classes with access to the calibrator
    virtual void set_calibrator (const Archive* archive);

    //! Return true if the calibrator attribute has been set
    bool has_calibrator () const;

  private:

    //! Reference to the Pulsar::Archive from which this instance was created
    Reference::To<const Archive> calibrator;

  };

}

//! Calibrator::Type output operator
std::ostream& operator << (std::ostream&, Pulsar::Calibrator::Type);

//! Calibrator::Type input operator
std::istream& operator >> (std::istream&, Pulsar::Calibrator::Type&);

#endif

