//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/Attic/ChainParameters.h,v $
   $Revision: 1.1 $
   $Date: 2008/06/14 12:56:30 $
   $Author: straten $ */

#ifndef __MEAL_ChainParameters_H
#define __MEAL_ChainParameters_H

#include "MEAL/ProjectGradient.h"
#include "MEAL/Composite.h"
#include "MEAL/Scalar.h"

namespace MEAL {

  //! A parameter and its constraining Scalar instance
  class ConstrainedParameter {

  public:
    ConstrainedParameter (unsigned iparam, bool infit, Scalar* func)
      : scalar (func)
    {
      previously_infit = infit;
      parameter = iparam;
    }

    //! The index of the Function parameter constrained by scalar
    unsigned parameter;

    //! The value of the fit flag before the parameter was constrained
    bool previously_infit;

    //! The constraining Scalar instance
    Project<Scalar> scalar;

    //! The last calculated gradient of the Scalar
    std::vector<double> gradient;

  };

  //! Parameterizes a Function by one or more Scalar ordinates.

  /*! Given any Function, \f$ M({\bf a}) \f$, one or more parameters may
    be constrained by a Scalar, \f$ f({\bf b}) \f$.  That is,
    \f$a_i=f({\bf b})\f$.  The fit flag for \f$a_i\f$ is set to false,
    and the partial derivatives of \f$ M \f$ with respect to the
    Scalar parameters, \f$\bf b\f$, are given by the chain rule,
    \f${\partial M\over\partial b_k} = {\partial M\over\partial
    a_i}{\partial f\over\partial b_k}\f$. */

  template<class T>
  class ChainParameters : public T
  {

  public:

    typedef typename T::Result Result;

    //! Default constructor
    ChainParameters () : composite (this) { }

    //! Copy constructor
    ChainParameters (const ChainParameters& rule) : composite (this) { operator = (rule); }

    //! Assignment operator
    ChainParameters& operator = (const ChainParameters& rule);

    //! Set the Function to be constrained by Scalar ordinates
    void set_model (T* model);

    //! Set the Scalar instance used to constrain the specified parameter
    void set_constraint (unsigned iparam, Scalar* scalar);

    // ///////////////////////////////////////////////////////////////////
    //
    // Function implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

  protected:

    //! Return the Result and its gradient
    void calculate (Result& result, std::vector<Result>* gradient);

    //! Scalars used to constrain the parameters
    std::vector<ConstrainedParameter> constraints;

    //! The Function to be constrained by Scalar ordinates
    Project<T> model;

  private:

    //! Composite parameter policy
    Composite composite;

  };

}

template<class T>
std::string MEAL::ChainParameters<T>::get_name () const
{
  return "ChainParameters<" + std::string(T::Name)+ ">";
}

template<class T>
MEAL::ChainParameters<T>&
MEAL::ChainParameters<T>::operator = (const ChainParameters& rule)
{
  if (this == &rule)
    return *this;

  set_model (rule.model);

  for (unsigned ic=0; ic < rule.constraints.size(); ic++)
    set_constraint (rule.constraints[ic].parameter,
		    rule.constraints[ic].scalar);

  return *this;
}


template<class T>
void MEAL::ChainParameters<T>::set_constraint (unsigned iparam, Scalar* scalar)
{
  if (T::verbose) 
    std::cerr << "MEAL::ChainParameters::set_constraint iparam=" << iparam
	      << std::endl;

  // only one scalar may control a parameter
  for (unsigned ifunc=0; ifunc<constraints.size(); ifunc++) {

    if (constraints[ifunc].parameter == iparam) {

      if (T::verbose)
	std::cerr << "MEAL::ChainParameters::set_constraint"
	  " remove param=" << iparam << std::endl;

      composite.unmap (constraints[ifunc].scalar);

      if (model)
	model->set_infit (iparam, constraints[ifunc].previously_infit);

      constraints.erase (constraints.begin() + ifunc);
      break;

    }
  }

  if (!scalar)
    return;

  bool infit = false;

  if (model)
    infit = model->get_infit (iparam);

  constraints.push_back (ConstrainedParameter (iparam, infit, scalar));

  if (T::verbose)
    std::cerr << "MEAL::ChainParameters::set_constraint"
      " add param=" << iparam << std::endl;

  composite.map (constraints.back().scalar);

  if (model)
    model->set_infit (iparam, false);

}

template<class T>
void MEAL::ChainParameters<T>::set_model (T* _model)
{
  if (!_model)
    return;

  if (model)
  {
    if (T::verbose)
      std::cerr << "MEAL::ChainParameters::set_model"
	" unmap old model" << std::endl;
    composite.unmap (model);
  }

  model = _model;

  if (T::verbose)
    std::cerr << "MEAL::ChainParameters::set_model"
      " map new model" << std::endl;

  composite.map (model);

  for (unsigned ifunc=0; ifunc<constraints.size(); ifunc++)
  {
    unsigned iparam = constraints[ifunc].parameter;
    constraints[ifunc].previously_infit = model->get_infit (iparam);
    model->set_infit (iparam, false);
  }
}

template<class T>
void MEAL::ChainParameters<T>::calculate (Result& result,
				    std::vector<Result>* grad)
{
  if (!model)
    throw Error (InvalidState, "MEAL::ChainParameters::calculate","no model");

  if (T::verbose)
    std::cerr << "MEAL::ChainParameters::calculate" << std::endl;

  for (unsigned ifunc=0; ifunc<constraints.size(); ifunc++) {

    if (T::very_verbose)
      std::cerr << "MEAL::ChainParameters::calculate iconstraint="<< ifunc <<std::endl;

    std::vector<double>* fgrad = 0;
    if (grad)
      fgrad = &(constraints[ifunc].gradient);

    model->set_param (constraints[ifunc].parameter,
		      constraints[ifunc].scalar->evaluate(fgrad));

  }

  std::vector<Result> model_grad;
  std::vector<Result>* model_grad_ptr = 0;
  if (grad)
    model_grad_ptr = & model_grad;

  result = model->evaluate (model_grad_ptr);
  
  if (grad) {

    unsigned ngrad = this->get_nparam();
    grad->resize (ngrad);

    unsigned igrad;
    for (igrad=0; igrad<ngrad; igrad++)
      (*grad)[igrad] = 0.0;

    // map the model gradient
    ProjectGradient (model, model_grad, *(grad));

    // map the scalar gradients
    std::vector<Result> fgrad;

    for (unsigned ifunc=0; ifunc<constraints.size(); ifunc++) {

      unsigned iparam = constraints[ifunc].parameter;

      ngrad = constraints[ifunc].gradient.size();
      fgrad.resize (ngrad);

      // dM/dxi = dM/df * df/dxi, where M=model, f=func, xi=func.param[i]
      for (igrad=0; igrad<ngrad; igrad++)
	fgrad[igrad] = model_grad[iparam] * constraints[ifunc].gradient[igrad];


      ProjectGradient (constraints[ifunc].scalar, fgrad, *(grad));

    }
  
  }

  if (T::verbose) {
    std::cerr << "MEAL::ChainParameters::calculate result\n"
      "   " << result << std::endl;
    if (grad) {
      std::cerr << "MEAL::ChainParameters::calculate gradient\n";
      for (unsigned i=0; i<grad->size(); i++)
	std::cerr << "   " << i << ":" << this->get_infit(i) << "=" 
                  << (*grad)[i] << std::endl;
    }
  }

}

#endif