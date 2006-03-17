//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/ExampleExtension.h,v $
   $Revision: 1.5 $
   $Date: 2006/03/17 13:34:45 $
   $Author: straten $ */

#ifndef __ExampleExtension_h
#define __ExampleExtension_h

#include "Pulsar/Archive.h"

namespace Pulsar {

  //! Example Archive::Extension
  /*! This class provides an example of how to inherit the
    Archive::Extension base class. By copying the files ExampleExtension.h
    and ExampleExtension.C and performing simple text-substitution, the
    skeleton of a new Extension may be easily developed. */
  class ExampleExtension : public Archive::Extension {

  public:
    
    //! Default constructor
    ExampleExtension ();

    //! Copy constructor
    ExampleExtension (const ExampleExtension&);

    //! Operator =
    const ExampleExtension& operator= (const ExampleExtension&);

    //! Destructor
    ~ExampleExtension ();

    //! Clone method
    ExampleExtension* clone () const { return new ExampleExtension( *this ); }

  };
 

}

#endif
