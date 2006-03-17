//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/ScalarExponential.h,v $
   $Revision: 1.4 $
   $Date: 2006/03/17 13:35:26 $
   $Author: straten $ */

#ifndef __MEAL_ScalarExponential_H
#define __MEAL_ScalarExponential_H

#include "MEAL/UnaryScalar.h"

namespace MEAL {

  //! The exponential of a Scalar function
  class ScalarExponential : public UnaryScalar
  {

  public:

    // ///////////////////////////////////////////////////////////////////
    //
    // UnaryScalar implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! The function
    double function (double x) const;

    //! And its first derivative
    double dfdx (double x) const;

    // ///////////////////////////////////////////////////////////////////
    //
    // Function implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

  };

}


#endif
