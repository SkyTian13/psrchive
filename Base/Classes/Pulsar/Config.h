//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Classes/Pulsar/Config.h,v $
   $Revision: 1.2 $
   $Date: 2006/03/17 13:34:50 $
   $Author: straten $ */

#ifndef __Pulsar_Config_h
#define __Pulsar_Config_h

#include "Configuration.h"

namespace Pulsar {

  //! Stores PSRCHIVE configuration parameters
  class Config : public Configuration {

  public:

    //! Default constructor loads $PSRCHIVE/psrchive.cfg
    Config ();

  };

  //! Global configuration
  extern Config config;

}

#endif
