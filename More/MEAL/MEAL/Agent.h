//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/Agent.h,v $
   $Revision: 1.4 $
   $Date: 2004/11/23 17:12:24 $
   $Author: straten $ */

#ifndef __MEAL_Agent_H
#define __MEAL_Agent_H

#include "Registry.h"
#include "Reference.h"

namespace MEAL {

  class Function;

  //! Pure virtual base class of Advocate template base class
  class Agent : public Reference::Able {

  public:

    //! Destructor
    virtual ~Agent () { }

    //! Return the name of the derived class
    virtual std::string get_name () = 0;

    //! Return a null-constructed instance of the derived class
    virtual Function* new_Function () = 0;

  protected:

    //! Agents registered for creating derived classes in Function::load
    static Registry::List<Agent> registry;
      
    //! Declare friends with Registry::Entry<Agent> so it can access registry
    friend class Registry::Entry<Agent>;

    //! Declare friends with Function so Function::load can access registry
    friend class Function;

  };


  //! Classes derived from Function are registered for use via an Advocate
  /*! This abstract template base class may be used to register
    Function-derived classes for use with the Function::load factory. */

  template<class Type>
    class Advocate : public Agent {
    
    public:

    //! Constructor ensures that template entry is instantiated
    Advocate () { entry.get(); instance = new Type; }

    //! Return the name of the derived class
    std::string get_name () { return instance->get_name(); }

    //! Return a new instance of the Function derived class
    Function* new_Function () { return new Type; }

    private:

    //! Use an instance of the class to call get_name method
    Type* instance;

    //! Enter template constructor adds Advocate<Type> to Agent::registry
    static Registry::List<Agent>::Enter<Advocate> entry;
    
  };

  /*! Static instance of entry attribute. */
  template<class Type>
    Registry::List<Agent>::Enter< Advocate<Type> > Advocate<Type>::entry;

}

#endif
