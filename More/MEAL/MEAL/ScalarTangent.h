//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/ScalarTangent.h,v $
   $Revision: 1.2 $
   $Date: 2004/11/22 16:00:09 $
   $Author: straten $ */

#ifndef __Model_ScalarTangent_H
#define __Model_ScalarTangent_H

#include "MEPL/UnaryScalar.h"

namespace Model {

  //! The tangent of a Scalar function
  class ScalarTangent : public UnaryScalar
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
    string get_name () const;

  };

}


#endif
