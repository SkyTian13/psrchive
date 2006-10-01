//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/SingleAxisCalibrator.h,v $
   $Revision: 1.20 $
   $Date: 2006/10/01 13:41:06 $
   $Author: straten $ */

#ifndef __Pulsar_SingleAxisCalibrator_H
#define __Pulsar_SingleAxisCalibrator_H

#include "Pulsar/ReferenceCalibrator.h"
#include "Pulsar/SingleAxis.h"
#include "Pulsar/SingleAxisSolver.h"

namespace Pulsar {

  //! Represents the system as a Calibration::SingleAxis
  class SingleAxisCalibrator : public ReferenceCalibrator {
    
  public:

    //! Construct from an single PolnCal Pulsar::Archive
    SingleAxisCalibrator (const Archive* archive);

    //! Destructor
    ~SingleAxisCalibrator ();

    //! Return Calibrator::SingleAxis
    Type get_type () const;

    //! Communicates SingleAxisCalibrator parameters
    class Info : public PolnCalibrator::Info {

    public:

      //! Constructor
      Info (const PolnCalibrator* calibrator);
      
      //! Return the number of parameter classes
      unsigned get_nclass () const;

      //! Return the name of the specified class
      const char* get_name (unsigned iclass) const;
      
      //! Return the number of parameters in the specified class
      unsigned get_nparam (unsigned iclass) const;

      Estimate<float> get_param (unsigned ichan, 
				 unsigned iclass,
				 unsigned iparam) const;

    };

    //! Return the SingleAxisCalibrator information
    Info* get_Info () const;

  protected:

    // ///////////////////////////////////////////////////////////////////
    //
    // Pulsar::ReferenceCalibrator implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return a new Calibration::SingleAxis instance
    MEAL::Complex2* solve (const std::vector<Estimate<double> >& hi,
			   const std::vector<Estimate<double> >& lo);

    //! Optimization for SingleAxis solve
    Reference::To< Calibration::SingleAxisSolver > solver;

  };

}

#endif
