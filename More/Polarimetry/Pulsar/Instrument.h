//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/Instrument.h,v $
   $Revision: 1.7 $
   $Date: 2007/11/07 18:46:37 $
   $Author: straten $ */

#ifndef __CalibrationInstrument_H
#define __CalibrationInstrument_H

#include "MEAL/ProductRule.h"
#include "MEAL/ChainRule.h"
#include "MEAL/Complex2.h"
#include "MEAL/ScalarParameter.h"

#include "Estimate.h"
#include "Stokes.h"

namespace Calibration {

  class SingleAxis;
  class Feed;

  //! Phenomenological description of the instrument
  /*! The transformation is represented by the product of a SingleAxis
    transformation and Feed transformation. */
  class Instrument : public MEAL::ProductRule<MEAL::Complex2> {

  public:

    //! Default Constructor
    Instrument ();

    //! Copy Constructor
    Instrument (const Instrument& s);

    //! Assignment Operator
    const Instrument& operator = (const Instrument& s);

    //! Destructor
    ~Instrument ();

    //! Model the ellipticities of both receptors using the same value
    void equal_ellipticities ();

    //! Model the orientations of both receptors using the same value
    void equal_orientations ();

    //! Set cyclical limits on the model parameters
    void set_cyclic (bool flag = true);

    //! Get the instrumental gain, \f$ G \f$, in calibrator flux units
    Estimate<double> get_gain () const;

    //! Get the differential gain, \f$ \gamma \f$, in hyperbolic radians
    Estimate<double> get_diff_gain () const;
    
    //! Get the differential phase, \f$ \phi \f$, in radians
    Estimate<double> get_diff_phase () const;

    //! Get the orientation, \f$\theta\f$, of the specified receptor in radians
    Estimate<double> get_orientation (unsigned ireceptor) const;

    //! Get the ellipticity, \f$\chi\f$, of the specified receptor in radians
    Estimate<double> get_ellipticity (unsigned ireceptor) const;

    //! Set the instrumental gain, \f$ G \f$, in calibrator flux units
    void set_gain (const Estimate<double>& gain);

    //! Set the differential gain, \f$ \gamma \f$, in hyperbolic radians
    void set_diff_gain (const Estimate<double>& gamma);
    
    //! Set the differential phase, \f$ \phi \f$, in radians
    void set_diff_phase (const Estimate<double>& phi);

    //! Set the orientation, \f$\theta\f$, of the specified receptor in radians
    void set_orientation (unsigned ireceptor, const Estimate<double>& theta);

    //! Set the ellipticity, \f$\chi\f$, of the specified receptor in radians
    void set_ellipticity (unsigned ireceptor, const Estimate<double>& chi);

    //! Provide access to the SingleAxis model
    const SingleAxis* get_backend () const;
    SingleAxis* get_backend ();

    //! Provide access to the Feed model
    const Feed* get_feed () const;
    Feed* get_feed ();

    //! Set the instrumental gain variation
    void set_gain (MEAL::Scalar*);

    //! Set the differential gain variation
    void set_diff_gain (MEAL::Scalar*);
    
    //! Set the differential phase variation
    void set_diff_phase (MEAL::Scalar*);

    // ///////////////////////////////////////////////////////////////////
    //
    // Model implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

  protected:

    //! SingleAxis model: \f$G\f$, \f$\gamma\f$, and \f$\varphi\f$
    Reference::To<SingleAxis> backend;

    //! Feed model: \f$\theta_0-1\f$ and \f$\chi_0-1\f$
    Reference::To<Feed> feed;

    //! ChainRule used to model Backend parameter variations
    Reference::To< MEAL::ChainRule<MEAL::Complex2> > backend_chain;

    //! Scalar function used to model gain variation
    Reference::To<MEAL::Scalar> gain_variation;

    //! Scalar function used to model differential gain variation
    Reference::To<MEAL::Scalar> diff_gain_variation;

    //! Scalar function used to model differential phase variation
    Reference::To<MEAL::Scalar> diff_phase_variation;

    //! ChainRule used to bind Feed parameters
    Reference::To< MEAL::ChainRule<MEAL::Complex2> > feed_chain;

    //! ScalarParameter used to bind ellipticities
    Reference::To<MEAL::ScalarParameter> ellipticities;

    //! ScalarParameter used to bind orientations
    Reference::To<MEAL::ScalarParameter> orientations;

  private:

    //! Initialize function used by constructors
    void init ();

  };

}

#endif

