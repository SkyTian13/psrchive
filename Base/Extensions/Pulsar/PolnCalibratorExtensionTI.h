//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/Attic/PolnCalibratorExtensionTI.h,v $
   $Revision: 1.2 $
   $Date: 2007/10/02 04:50:09 $
   $Author: straten $ */

#ifndef __Pulsar_PolnCalibratorExtensionTI_h
#define __Pulsar_PolnCalibratorExtensionTI_h

#include "Pulsar/PolnCalibratorExtension.h"
#include "TextInterface.h"

namespace Pulsar
{

  //! Provides a text interface to get and set PolnCalibratorExtension attributes
  class PolnCalibratorExtensionTI : public TextInterface::To<PolnCalibratorExtension>
  {

  public:

    //! Constructor
    PolnCalibratorExtensionTI ();
    PolnCalibratorExtensionTI ( PolnCalibratorExtension *c );
    void SetupMethods( void );

    virtual std::string get_interface_name() { return "PolnCalibratorExtensionTI"; }

    TextInterface::Parser *clone();
  };

}

#endif
