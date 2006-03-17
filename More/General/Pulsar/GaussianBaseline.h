//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/GaussianBaseline.h,v $
   $Revision: 1.2 $
   $Date: 2006/03/17 13:34:51 $
   $Author: straten $ */

#ifndef __Pulsar_GaussianBaseline_h
#define __Pulsar_GaussianBaseline_h

#include "Pulsar/IterativeBaseline.h"

namespace Pulsar {

  //! Finds a baseline that contains gaussian white noise
  class GaussianBaseline : public IterativeBaseline {

  protected:

    void get_bounds (PhaseWeight& weight, float& lower, float& upper);

  };

}

#endif
