/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "MEAL/ScalarMath.h"
#include "MEAL/ScalarParameter.h"
#include "MEAL/Value.h"

#include "MEAL/ScalarRoot.h"
#include "MEAL/ScalarSine.h"
#include "MEAL/ScalarCosine.h"
#include "MEAL/ScalarTangent.h"
#include "MEAL/ScalarHypSine.h"
#include "MEAL/ScalarHypCosine.h"
#include "MEAL/ScalarHypTangent.h"
#include "MEAL/ScalarErrorFunction.h"
#include "MEAL/ScalarExponential.h"
#include "MEAL/ScalarLogarithm.h"
#include "MEAL/ScalarAbsolute.h"
#include "MEAL/ScalarInverseHypTangent.h"
#include "MEAL/ScalarInverseTangent.h"
#include "MEAL/ScalarInverseTangent2.h"
#include "MEAL/ScalarPower.h"

#include "MEAL/SumRule.h"
#include "MEAL/NegationRule.h"
#include "MEAL/ProductRule.h"

inline double inv (double x) { return 1.0/x; }

#include "MEAL/RuleMath.h"

MEAL::ScalarMath::ScalarMath (const Estimate<double>& value)
{
  expression = new ScalarParameter (value);
}
    
MEAL::ScalarMath::ScalarMath (double value)
{
  expression = new Value<Scalar> (value);
}

MEAL::ScalarMath::ScalarMath (const ScalarMath& s)
{
  expression = s.expression;
}

MEAL::ScalarMath::ScalarMath (const Reference::To<Scalar>& s)
{
  expression = s;
}

MEAL::ScalarMath::ScalarMath (Scalar& s)
{
  expression = &s;
}

const MEAL::ScalarMath 
MEAL::ScalarMath::operator = (const ScalarMath& s)
{
  if (&s != this)
    expression = s.expression;

  return *this;
}
 
const MEAL::ScalarMath
MEAL::ScalarMath::operator += (const ScalarMath& b)
{
  return operator = (*this + b);
}

const MEAL::ScalarMath
MEAL::ScalarMath::operator -= (const ScalarMath& b)
{
  return operator = (*this - b);
}
    
const MEAL::ScalarMath
MEAL::ScalarMath::operator *= (const ScalarMath& b)
{
  return operator = (*this * b);
}
    
const MEAL::ScalarMath
MEAL::ScalarMath::operator /= (const ScalarMath& b)
{
  return operator = (*this / b);
}

const MEAL::ScalarMath
MEAL::operator + (const ScalarMath& a, const ScalarMath& b)
{
  return *Sum<Scalar> (a.get_expression(), b.get_expression());
}

const MEAL::ScalarMath
MEAL::operator - (const ScalarMath& a, const ScalarMath& b)
{
  return *Difference<Scalar> (a.get_expression(), b.get_expression());
}

const MEAL::ScalarMath
MEAL::operator * (const ScalarMath& a, const ScalarMath& b)
{
  return *Product<Scalar> (a.get_expression(), b.get_expression());
}

const MEAL::ScalarMath
MEAL::operator / (const ScalarMath& a, const ScalarMath& b)
{
  return *Quotient<Scalar> (a.get_expression(), b.get_expression());
}

const MEAL::ScalarMath
MEAL::operator - (const ScalarMath& a)
{
  return *Negation<Scalar> (a.get_expression());
}

const MEAL::ScalarMath
MEAL::operator + (const ScalarMath& a)
{
  return a;
}

bool MEAL::operator == (const ScalarMath& a, const ScalarMath& b)
{
  return a.evaluate() == b.evaluate();
}

bool MEAL::operator != (const ScalarMath& a, const ScalarMath& b)
{
  return a.evaluate() != b.evaluate();
}

bool MEAL::operator < (const ScalarMath& a, const ScalarMath& b)
{
  return a.evaluate() < b.evaluate();
}

bool MEAL::operator > (const ScalarMath& a, const ScalarMath& b)
{
  return a.evaluate() > b.evaluate();
}

const MEAL::ScalarMath
MEAL::sqrt (const ScalarMath& x)
{
  ScalarRoot* result = new ScalarRoot;
  result->set_model( x.get_expression() );
  return *result;
}


const MEAL::ScalarMath
MEAL::sin (const ScalarMath& x)
{
  ScalarSine* result = new ScalarSine;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::cos (const ScalarMath& x)
{
  ScalarCosine* result = new ScalarCosine;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::tan (const ScalarMath& x)
{
  ScalarTangent* result = new ScalarTangent;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::sinh (const ScalarMath& x)
{
  ScalarHypSine* result = new ScalarHypSine;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::cosh (const ScalarMath& x)
{
  ScalarHypCosine* result = new ScalarHypCosine;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::tanh (const ScalarMath& x)
{
  ScalarHypTangent* result = new ScalarHypTangent;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::abs (const ScalarMath& x)
{
  ScalarAbsolute* result = new ScalarAbsolute;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::erf (const ScalarMath& x)
{
  ScalarErrorFunction* result = new ScalarErrorFunction;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::exp (const ScalarMath& x)
{
  ScalarExponential* result = new ScalarExponential;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::log (const ScalarMath& x)
{
  ScalarLogarithm* result = new ScalarLogarithm;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::atanh (const ScalarMath& x)
{
  UnaryScalar* result = new ScalarInverseHypTangent;
  result->set_model( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::atan (const ScalarMath& x)
{
  UnaryScalar* result = new ScalarInverseTangent;
  result->set_model( x.get_expression() );
  return *result;
}
const MEAL::ScalarMath
MEAL::atan2 (const ScalarMath& y, const ScalarMath& x)
{
  BinaryScalar* result = new ScalarInverseTangent2;
  result->set_arg1( y.get_expression() );
  result->set_arg2( x.get_expression() );
  return *result;
}

const MEAL::ScalarMath
MEAL::pow (const ScalarMath& x, const ScalarMath& y)
{
  BinaryScalar* result = new ScalarPower;
  result->set_arg1( x.get_expression() );
  result->set_arg2( y.get_expression() );
  return *result;
}

//! Evaluate the expression
double MEAL::ScalarMath::evaluate () const
{
  return expression->evaluate();
}

//! Evaluate the expression and its estimated error
Estimate<double> MEAL::ScalarMath::get_Estimate () const
{
  return expression->estimate ();
}

using namespace MEAL;

bool operator <= (const ScalarMath& a, const ScalarMath& b)
{ return a.evaluate() <= b.evaluate(); }

bool operator <  (const ScalarMath& a, const ScalarMath& b)
{ return a.evaluate() < b.evaluate(); }

bool operator >= (const ScalarMath& a, const ScalarMath& b)
{ return a.evaluate() >= b.evaluate(); }

bool operator >  (const ScalarMath& a, const ScalarMath& b)
{ return a.evaluate() > b.evaluate(); }

bool operator == (const ScalarMath& a, const ScalarMath& b)
{ return a.evaluate() == b.evaluate(); }

