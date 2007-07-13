//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/FrequencyAppend.h,v $
   $Revision: 1.1 $
   $Date: 2007/07/13 06:28:21 $
   $Author: straten $ */

#ifndef _Pulsar_FrequencyAppend_H
#define _Pulsar_FrequencyAppend_H

#include "Pulsar/Append.h"

namespace Pulsar {

  class Integration;

  //! Algorithms that combine Integration data
  class FrequencyAppend : public Append {

  public:

    //! Default constructor
    FrequencyAppend ();

  protected:

    //! Return true if there is a benign reason to stop
    virtual bool stop (Archive* into, const Archive* from);

    //! Throw an exception if there is a fatal reason to stop
    virtual void check (Archive* into, const Archive* from);

    //! Add the data in 'from' to 'into'
    virtual void combine (Archive* into, Archive* from);

    //! Add the data in 'from' to 'into'
    virtual void combine (Integration* into, Integration* from);

  };
  
}

#endif
