//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/PolnCalibratorExtension.h,v $
   $Revision: 1.24 $
   $Date: 2007/10/02 04:50:09 $
   $Author: straten $ */

#ifndef __PolnCalibratorExtension_h
#define __PolnCalibratorExtension_h

#include "Pulsar/CalibratorExtension.h"

namespace Pulsar {

  class PolnCalibrator;

  //! Stores PolnCalibrator parameters in an Archive instance
  /*! This Archive::Extension implements the storage of PolnCalibrator
    data. */  
  class PolnCalibratorExtension : public CalibratorExtension {
    
  public:
    
    //! Default constructor
    PolnCalibratorExtension ();

    //! Copy constructor
    PolnCalibratorExtension (const PolnCalibratorExtension&);

    //! Operator =
    const PolnCalibratorExtension& operator= (const PolnCalibratorExtension&);

    //! Destructor
    ~PolnCalibratorExtension ();

    //! Clone method
    PolnCalibratorExtension* clone () const
    { return new PolnCalibratorExtension( *this ); }
    
    //! get the text ineterface 
    TextInterface::Parser* get_interface();
    
    //! set the ncpar
    void set_ncpar( int s_ncpar ) { ncpar = s_ncpar; }
    
    //! get the ncpar
    int get_ncpar( void ) const { return ncpar; }
    
    //! set the epoch
    //void set_epoch( 

    //! Construct from a PolnCalibrator instance
    PolnCalibratorExtension (const PolnCalibrator*);

    //! Set the type of the instrumental response parameterization
    void set_type (Calibrator::Type type);

    //! Set the number of frequency channels
    void set_nchan (unsigned nchan);

    //! Set the weight of the specified channel
    void set_weight (unsigned ichan, float weight);

    //! Get the number of parameters describing each transformation
    unsigned get_nparam () const;

    //! Get if the covariances of the transformation parameters
    bool get_has_covariance () const;
    //! Set if the covariances of the transformation parameters
    void set_has_covariance (bool);

    //! Return true if the transformation for the specified channel is valid
    bool get_valid (unsigned ichan) const;
    void set_valid (unsigned ichan, bool valid);

    class Transformation;

    //! Get the transformation for the specified frequency channel
    Transformation* get_transformation (unsigned c);
    //! Get the transformation for the specified frequency channel
    const Transformation* get_transformation (unsigned c) const;

  protected:

    //! The instrumental response as a function of frequency
    std::vector<Transformation> response;

    //! The number of parameters that describe the transformation
    unsigned nparam;

    //! The covariances of the transformation parameters are available
    bool has_covariance;

    //! Construct the response array according to the current attributes
    void construct ();

  private:
    
    int ncpar;

    void init ();

  };
 
  //! Intermediate storage of MEAL::Complex parameters 
  class PolnCalibratorExtension::Transformation {

  public:

    //! Default constructor
    Transformation ();

    //! Get the number of model parameters
    unsigned get_nparam() const;
    //! Set the number of model parameters
    void set_nparam (unsigned);

    //! Get the value of the specified model parameter
    double get_param (unsigned) const;
    //! Set the value of the specified model parameter
    void set_param (unsigned, double);

    //! Get the variance of the specified model parameter
    double get_variance (unsigned) const;
    //! Set the variance of the specified model parameter
    void set_variance (unsigned, double);

    //! Get the covariance matrix of the model paramters
    std::vector< std::vector<double> > get_covariance () const;
    //! Set the covariance matrix of the model paramters
    void set_covariance (const std::vector< std::vector<double> >&);

    //! Get the covariance matrix efficiently
    void get_covariance (std::vector<double>&) const;
    //! Set the covariance matrix efficiently
    void set_covariance (const std::vector<double>&);

    //! Get the value and variance of the specified model parameter
    Estimate<double> get_Estimate (unsigned) const;
    //! Set the value and variance of the specified model parameter
    void set_Estimate (unsigned, const Estimate<double>&);

    //! Get the model validity flag
    bool get_valid () const;
    //! Set the model validity flag
    void set_valid (bool);

  protected:

    std::vector< Estimate<double> > params;

    std::vector<double> covariance;

    bool valid;

  };

}

#endif
