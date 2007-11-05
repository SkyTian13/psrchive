//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2007 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/RFIMitigation/Pulsar/LawnMower.h,v $
   $Revision: 1.3 $
   $Date: 2007/11/02 04:25:35 $
   $Author: straten $ */

#ifndef __Pulsar_LawnMower_h
#define __Pulsar_LawnMower_h

#include "Pulsar/Transformation.h"
#include "Pulsar/Integration.h"
#include "Functor.h"

namespace Pulsar {

  class BaselineEstimator;
  class OnPulseThreshold;
  class PhaseWeight;

  //! Removes broad-band impulsive spikes from pulse profiles
  /*! By adding preconditions, this class can be made interactive */
  class LawnMower : public Transformation<Integration> {

  public:

    //! Default constructor
    LawnMower ();

    //! Destructor
    ~LawnMower ();

    //! Set the Profile from which baseline PhaseWeight will be computed
    void transform (Integration*);

    //! Set the BaselineEstimator used to find the baseline
    void set_baseline_estimator (BaselineEstimator*);
    BaselineEstimator* get_baseline_estimator () const;

    //! Set the threshold above which samples are mown
    virtual void set_threshold (float sigma);

    //! Set the size of the window used during median smoothing
    virtual void set_median_smoothing (float turns);

    //! If set, search for spikes in fscrunched (DM=0) total
    virtual void set_broadband (bool);

    //! One or more preconditions can be added
    virtual void add_precondition( Functor< bool(Profile*,PhaseWeight*) > );

  protected:
    
    //! Masks all points above a threshold
    Reference::To<OnPulseThreshold> mower;

    //! Points to be mowed
    Reference::To<PhaseWeight> mowed;

    //! Points to be included in baseline estimator used by mower
    Reference::To<PhaseWeight> include;

    //! Create mask and return true if all preconditions are satisfied
    bool build_mask (Profile* profile);

    //! Search for spikes in fscrunch (DM=0) total
    bool broadband;

    float median_smoothing_turns;

    //! Preconditions
    std::vector< Functor< bool(Profile*,PhaseWeight*) > > precondition;

  };

}

#endif