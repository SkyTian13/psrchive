//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2011 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/JonesSpinor.h,v $
   $Revision: 1.2 $
   $Date: 2008/06/15 17:09:05 $
   $Author: straten $ */

#ifndef __MEAL_JonesSpinor_H
#define __MEAL_JonesSpinor_H

#include "MEAL/SingularCoherency.h"
#include "MEAL/Spinor.h"
#include "MEAL/Convert.h"

namespace MEAL {

  //! Returns the Spinor representation of a singular coherency matrix

  class JonesSpinor : public Convert<SingularCoherency,Spinor>
  {

  public:

    JonesSpinor ();
    std::string get_name () const;

  protected:

    //! Calculate the Mueller matrix and its gradient
    virtual void calculate (Spinor::Result& result,
			    std::vector<Spinor::Result>*);

    unsigned pole;
  };

}

#endif

