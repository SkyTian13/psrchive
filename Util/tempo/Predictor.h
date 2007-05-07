//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2007 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Util/tempo/Attic/Predictor.h,v $
   $Revision: 1.4 $
   $Date: 2007/05/07 00:53:28 $
   $Author: straten $ */

#ifndef __PulsarPredictor_h
#define __PulsarPredictor_h

#include "Reference.h"
#include "Phase.h"
#include "MJD.h"

namespace Pulsar {

  //! Functions that predict pulse phase
  /*! This pure virtual base class defines the interface to pulse
    phase predictors */
  class Predictor : public Reference::Able {

  public:

    //! Return a new, copy constructed instance of self
    virtual Predictor* clone () const = 0;

    //! Add the information from the supplied predictor to self
    virtual void insert (const Predictor*) = 0;

    //! Return true if the supplied predictor is equal to self
    virtual bool equals (const Predictor*) const = 0;

    //! Set the observing frequency at which phase and time are related
    virtual void set_observing_frequency (long double MHz) = 0;

    //! Get the observing frequency at which phase and epoch are related
    virtual long double get_observing_frequency () const = 0;

    //! Return the phase, given the epoch
    virtual Phase phase (const MJD& t) const = 0;

    //! Return the epoch, given the phase and, optionally, a first guess
    virtual MJD iphase (const Phase& phase, const MJD* guess = 0) const = 0;

    //! Return the spin frequency, given the epoch
    virtual long double frequency (const MJD& t) const = 0;

    //! Return the phase correction for dispersion delay
    virtual Phase dispersion (const MJD &t, long double MHz) const = 0;

  };

}

#endif
