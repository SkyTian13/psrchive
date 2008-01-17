//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2007 by Paul Demorest
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Formats/ASP/Pulsar/ASPArchive.h,v $
   $Revision: 1.1 $
   $Date: 2008/01/01 18:08:38 $
   $Author: demorest $ */

#ifndef __ASPArchive_h
#define __ASPArchive_h

#include "Pulsar/BasicArchive.h"
#include "Pulsar/Agent.h"

// File version codes
#define ASP_FITS_V10  1
#define ASP_FITS_V11  2
#define ASP_FITS_V101 3

namespace Pulsar {

  //! Loads and unloads ASP Pulsar archives
  class ASPArchive : public BasicArchive {

  public:
    
    //! Default constructor
    ASPArchive ();

    //! Copy constructor
    ASPArchive (const ASPArchive& archive);

    //! Destructor
    ~ASPArchive ();

    //! Assignment operator
    const ASPArchive& operator = (const ASPArchive& archive);
    
    //! Base copy constructor
    ASPArchive (const Archive& archive);

    //! Base extraction constructor
    ASPArchive (const Archive&, const std::vector<unsigned>& subint);

    //! Copy all of the class attributes and the selected Integration data
    void copy (const Archive& archive, const std::vector<unsigned>& subints);

    //! Return a new copy-constructed ASPArchive instance
    ASPArchive* clone () const;

    //! Return a new extraction-constructed ASPArchive instance
    ASPArchive* extract (const std::vector<unsigned>& subints) const;
    
  protected:

    //! Load the ASP header information from filename
    virtual void load_header (const char* filename);

    //! Load the specified Integration from filename, returning new instance
    virtual Integration*
    load_Integration (const char* filename, unsigned subint);

    //! Unload the ASPArchive (header and Integration data) to filename
    virtual void unload_file (const char* filename) const;

    // Advocates the use of the ASPArchive plugin
    class Agent;

    //! Enable Advocate template class to access protected Agent class
    friend class Archive::Advocate<ASPArchive>;

  private:

    //! Initialize all values to null
    void init ();

    //! ASP file version
    int asp_file_version;

  };
 

  // Advocates the use of the ASPArchive plugin
  class ASPArchive::Agent : public Archive::Advocate<ASPArchive> {
    
  public:
    
    Agent () { } 
    
    //! Advocate the use of ASPArchive to interpret filename
    bool advocate (const char* filename);
    
    //! Return the name of the ASPArchive plugin
    std::string get_name () { return "ASP"; }
    
    //! Return description of this plugin
    std::string get_description ();
    
  };

}

#endif