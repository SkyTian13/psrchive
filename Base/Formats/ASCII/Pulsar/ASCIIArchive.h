//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Formats/ASCII/Pulsar/ASCIIArchive.h,v $
   $Revision: 1.9 $
   $Date: 2009/05/23 10:54:10 $
   $Author: straten $ */

#ifndef __ASCIIArchive_h
#define __ASCIIArchive_h

#include "Pulsar/BasicArchive.h"
#include "Pulsar/Agent.h"

namespace Pulsar {

  //! Loads ASCII data files also known as SIGPROC format
  class ASCIIArchive : public BasicArchive {

  public:
    
    //! Default constructor
    ASCIIArchive ();

    //! Copy constructor
    ASCIIArchive (const ASCIIArchive& archive);

    //! Destructor
    ~ASCIIArchive ();

    //! Assignment operator
    const ASCIIArchive& operator = (const ASCIIArchive& archive);
    
    //! Base copy constructor
    ASCIIArchive (const Archive& archive);

    //! Copy all of the class attributes and the selected Integration data
    void copy (const Archive& archive);

    //! Return a new copy-constructed ASCIIArchive instance
    ASCIIArchive* clone () const;

    //! Load the ASCII header information from filename
    virtual void load_header (const char* filename);
    
  protected:

    //! Load the specified Integration from filename, returning new instance
    virtual Integration*
    load_Integration (const char* filename, unsigned subint);

    //! The unload_file method is not implemented
    bool can_unload () const { return false; }

    //! Unload the ASCIIArchive (header and Integration data) to filename
    virtual void unload_file (const char* filename) const;

    friend class Archive::Advocate<ASCIIArchive>;

    //! This class registers the ASCIIArchive plugin class for use
    class Agent : public Archive::Advocate<ASCIIArchive> {

      public:

        Agent () { } 

        //! Advocate the use of TimerArchive to interpret filename
        bool advocate (const char* filename);

        //! Return the name of the TimerArchive plugin
        std::string get_name () { return "ASCIIArchive"; }
    
        //! Return description of this plugin
        std::string get_description ();

    };

    //! The integration length
    double integration_length;

  private:

    //! Initialize all values to null
    void init ();

  };
 

}

#endif
