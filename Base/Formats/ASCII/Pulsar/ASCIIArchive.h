//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Base/Formats/ASCII/Pulsar/ASCIIArchive.h,v $
   $Revision: 1.1 $
   $Date: 2006/01/06 22:28:25 $
   $Author: straten $ */

#ifndef __ASCIIArchive_h
#define __ASCIIArchive_h

#include "Pulsar/BasicArchive.h"

namespace Pulsar {

  //! Loads and unloads ASCII Pulsar archives
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

    //! Base extraction constructor
    ASCIIArchive (const Archive& archive, const vector<unsigned>& subint);

    //! Copy all of the class attributes and the selected Integration data
    void copy (const Archive& archive, const vector<unsigned>& subints);

    //! Return a new copy-constructed ASCIIArchive instance
    ASCIIArchive* clone () const;

    //! Return a new extraction-constructed ASCIIArchive instance
    ASCIIArchive* extract (const vector<unsigned>& subints) const;
    
  protected:

    //! Load the ASCII header information from filename
    virtual void load_header (const char* filename);

    //! Load the specified Integration from filename, returning new instance
    virtual Integration*
    load_Integration (const char* filename, unsigned subint);

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
        string get_name () { return "ASCIIArchive"; }
    
        //! Return description of this plugin
        string get_description ();

    };

    //! The integration length
    double integration_length;

  private:

    //! Initialize all values to null
    void init ();

  };
 

}

#endif
