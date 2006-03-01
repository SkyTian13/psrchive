//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/Configuration.h,v $
   $Revision: 1.1 $
   $Date: 2006/03/01 19:17:57 $
   $Author: straten $ */

#ifndef __Configuration_h
#define __Configuration_h

#include "tostring.h"
#include <vector>

//! Stores keyword-value pairs from a configuration file
/*! The Configuration class enables convenient, distributed access to
  configuration parameters stored in a simple text file */
class Configuration
{
public:

  //! Construct from the specified file
  Configuration (const char* filename);

  //! Keyword-value pair
  class Entry;

  //! Get the value for the specified key
  template<typename T> T get (const char* key, T default_value) const;

protected:

  std::vector<Entry> entries;

};

class Configuration::Entry {
public:
  Entry (std::string& k, std::string& v) { key=k; value=v; }
  std::string key;
  std::string value;
};

//! Get the value for the specified key
template<typename T> 
T Configuration::get (const char* key, T default_value) const
{
  for (unsigned i=0; i<entries.size(); i++)
    if (entries[i].key == key)
      return fromstring<T> (entries[i].value);

  return default_value;
}

#endif
