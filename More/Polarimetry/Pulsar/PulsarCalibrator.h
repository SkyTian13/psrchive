//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/PulsarCalibrator.h,v $
   $Revision: 1.11 $
   $Date: 2006/01/12 21:35:43 $
   $Author: straten $ */

#ifndef __Pulsar_PulsarCalibrator_H
#define __Pulsar_PulsarCalibrator_H

#include "Pulsar/PolnCalibrator.h"

#include "MEAL/Complex2Value.h"
#include "MEAL/Mean.h"

namespace Pulsar {

  class Archive;
  class Integration;
  class PolnProfileFit;
  class ReferenceCalibrator;

  //! Uses PolnProfileFit to determine the system response
  /*! The PulsarCalibrator implements a technique of polarimetric
    calibration using a well-determined source.  This class requires a
    polarimetric standard and another observation of the same source.  */
  class PulsarCalibrator : public PolnCalibrator {
    
  public:
    
    //! Constructor
    PulsarCalibrator (Calibrator::Type model);

    //! Destructor
    ~PulsarCalibrator ();

    //! Return the reference epoch of the calibration experiment
    MJD get_epoch () const;

    //! Return Calibrator::Hamaker or Calibrator::Britton
    Type get_type () const;

    //! Return the Calibrator information
    Info* get_Info () const;

    //! Set the maximum number of harmonics to include in fit
    void set_maximum_harmonic (unsigned max);

    //! Allow software to choose the maximum harmonic
    void set_choose_maximum_harmonic (bool flag = true);

    //! Set the standard to which pulsar profiles will be fit
    void set_standard (const Archive* data);

    //! Add the observation to the set of constraints
    void add_observation (const Archive* data);
    
    //! Set the flag to return the mean solution or the last fit
    void set_return_mean_solution (bool return_mean = true);

    //! Set the solution to the mean
    void update_solution ();

  protected:
    
    //! Initialize the PolnCalibration::transformation attribute
    virtual void calculate_transformation ();

    //! The calibration model as a function of frequency
    std::vector< Reference::To<PolnProfileFit> > model;

    //! The array of transformation Model instances
    // vector< Reference::To<MEAL::Complex2> > transformation;

    typedef MEAL::Mean< MEAL::Complex2 > MeanXform;

    //! The array of transformation Model instances
    std::vector< Reference::To<MeanXform> > solution;

    //! The known instrumental corrections
    MEAL::Complex2Value corrections;

    //! The model specified on construction
    Calibrator::Type model_type;

    //! The maximum number of harmonics to include in the fit
    unsigned maximum_harmonic;

    //! When set, the software will choose the maximum harmonic
    bool choose_maximum_harmonic;

    //! When true, calculate_transformation will return the mean solution
    bool mean_solution;

    //! Solve the measurement equation for the given channel
    void solve (const Integration* data, unsigned ichan);

  };

}

#endif
