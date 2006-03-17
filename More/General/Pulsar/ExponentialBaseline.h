//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/ExponentialBaseline.h,v $
   $Revision: 1.3 $
   $Date: 2006/03/17 13:34:50 $
   $Author: straten $ */

#ifndef __Pulsar_ExponentialBaseline_h
#define __Pulsar_ExponentialBaseline_h

#include "Pulsar/IterativeBaseline.h"

namespace Pulsar {

  //! Finds a baseline that contains gaussian white noise
  class ExponentialBaseline : public IterativeBaseline {

  public:

    //! Default constructor
    ExponentialBaseline ();

  protected:

    void get_bounds (PhaseWeight& weight, float& lower, float& upper);

  };

}

#endif
