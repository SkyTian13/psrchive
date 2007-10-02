//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/PulsarCalibrator.h,v $
   $Revision: 1.24 $
   $Date: 2007/10/02 05:44:12 $
   $Author: straten $ */

#ifndef __Pulsar_PulsarCalibrator_H
#define __Pulsar_PulsarCalibrator_H

#include "Pulsar/PolnCalibrator.h"

#include "MEAL/Complex2Value.h"
#include "MEAL/Mean.h"

#include <stdio.h>

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
    PulsarCalibrator (Calibrator::Type model = Calibrator::Britton);

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

    //! Get the number of harmonics in use
    unsigned get_nharmonic () const;

    //! Ignore fluctuations in pulsar flux
    void set_normalize_gain (bool flag = true);

    //! Set the standard to which pulsar profiles will be fit
    void set_standard (const Archive* data);

    //! Add the observation to the set of constraints
    void add_observation (const Archive* data);
    
    //! Set the flag to return the mean solution or the last fit
    void set_return_mean_solution (bool return_mean = true);

    //! Set the solution to the mean
    void update_solution ();

    //! File to which arrival time estimates should be written
    void set_tim_file (FILE* fptr) { tim_file = fptr; }

    //! The model used to fit the specified channel
    const PolnProfileFit* get_model (unsigned ichan) const;

    //! Set the number of channels that may be simultaneously solved
    void set_nthread (unsigned nthread);

  protected:
    
    //! Initialize the PolnCalibration::transformation attribute
    virtual void calculate_transformation ();

    //! Return a pointer to a newly constructed/initialized transformation
    MEAL::Complex2* new_transformation () const;

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

    //! The maximum harmonic chosen
    unsigned chosen_maximum_harmonic;

    //! When true, calculate_transformation will return the mean solution
    bool mean_solution;

    //! When true, set the gain to unity after the best fit has been found
    bool normalize_gain;

    //! Epoch of the solution
    MJD epoch;

    //! Solve the measurement equation for the given channel
    void solve (const Integration* data, unsigned ichan);

  private:

    // used to communicate between solve and add_observation
    unsigned big_difference;

    //! File to which arrival time estimates should be written
    FILE* tim_file;

    //! Archive instance that is currently in use
    const Archive* archive;

    //! The number of channels that may be simultaneously solved
    unsigned nthread;

    //! Build the arrays
    void build (unsigned nchan);

  };

}

#endif
