//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/PolarCalibrator.h,v $
   $Revision: 1.21 $
   $Date: 2007/12/06 05:49:36 $
   $Author: straten $ */

#ifndef __PolarCalibrator_H
#define __PolarCalibrator_H

#include "Pulsar/ReferenceCalibrator.h"
#include "MEAL/Polar.h"

namespace Pulsar {

  //! Represents the system as a polar decomposition
  class PolarCalibrator : public ReferenceCalibrator {
    
  public:

    //! Construct from an single PolnCal Pulsar::Archive
    PolarCalibrator (const Archive* archive);

    //! Destructor
    ~PolarCalibrator ();

    //! Return Calibrator::Polar
    Type get_type () const;

    //! Communicates PolarCalibrator parameters to plotting routines
    class Info : public PolnCalibrator::Info {

    public:

      //! Constructor
      Info (const PolnCalibrator* calibrator);
      
      //! Return the number of parameter classes
      unsigned get_nclass () const;

      //! Return the name of the specified class
      std::string get_name (unsigned iclass) const;
      
      //! Return the number of parameters in the specified class
      unsigned get_nparam (unsigned iclass) const;

    };

    //! Return the PolarAxisCalibrator information
    Info* get_Info () const;

  protected:

    // ///////////////////////////////////////////////////////////////////
    //
    // Pulsar::ReferenceCalibrator implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return a new Calibration::Polar instance
    MEAL::Complex2* solve (const std::vector<Estimate<double> >& hi,
                           const std::vector<Estimate<double> >& lo);

  };

}

#endif
