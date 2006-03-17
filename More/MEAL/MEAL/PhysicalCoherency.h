//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/PhysicalCoherency.h,v $
   $Revision: 1.2 $
   $Date: 2006/03/17 13:35:26 $
   $Author: straten $ */

#ifndef __MEAL_PhysicalCoherency_H
#define __MEAL_PhysicalCoherency_H

#include "MEAL/Coherency.h"

namespace MEAL {

  //! A coherency matrix with determinant greater than zero
  /*! This class enforces the det(rho) > 0 restriction. */
  class PhysicalCoherency : public Coherency {

  public:

    //! Default constructor
    PhysicalCoherency ();

    //! Set the Stokes parameters of the model
    virtual void set_stokes (const Stokes<double>& stokes);
    
    //! Set the Stokes parameters and their estimated errors
    virtual void set_stokes (const Stokes< Estimate<double> >& stokes);

    //! Get the Stokes parameters and their estimated errors
    virtual Stokes< Estimate<double> > get_stokes () const;

    // ///////////////////////////////////////////////////////////////////
    //
    // Function implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

  protected:

    //! Calculate the Jones matrix and its gradient
    void calculate (Jones<double>& result, std::vector<Jones<double> >*);

  };

}

#endif
