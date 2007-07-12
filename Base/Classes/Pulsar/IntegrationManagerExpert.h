//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Classes/Pulsar/IntegrationManagerExpert.h,v $
   $Revision: 1.4 $
   $Date: 2007/07/12 05:58:35 $
   $Author: straten $ */

#ifndef __Pulsar_IntegrationManagerExpert_h
#define __Pulsar_IntegrationManagerExpert_h

#include "Pulsar/IntegrationManager.h"

namespace Pulsar {

  //! Provides access to private and protected members of IntegrationManager
  /*! Certain methods, e.g. IntegrationManager::append, are protected
    so that they will not naively be called, e.g. instead of
    Archive::append, which performs all of the proper book keeping
    tasks.  However, some applications require the ability to append
    Integration instances to an Archive without the extra book
    keeping.  This class provides the required access without the need to:
    <UL>
    <LI> unprotect methods, or
    <LI> make friends with every class or function that requires access
    </UL>
  */

  class IntegrationManager::Expert : public Reference::Able {

  public:

    Expert (IntegrationManager* inst)
    { instance = inst; }

    //! Append new_Integration copies of Integration objects to instance
    void append (const IntegrationManager* more_subints)
    { instance->append (more_subints); }

    //! Append use_Integration modified Integration objects to instance
    void manage (IntegrationManager* more_subints)
    { instance->manage (more_subints); }

    //! Directly append the Integration to instance (no copy)
    void manage (Integration* integration)
    { instance->manage (integration); }

  private:

    //! instance
    Reference::To<IntegrationManager, false> instance;

  };

}

#endif
