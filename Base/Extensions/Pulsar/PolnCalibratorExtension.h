//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/PolnCalibratorExtension.h,v $
   $Revision: 1.13 $
   $Date: 2004/10/11 14:26:44 $
   $Author: straten $ */

#ifndef __PolnCalibratorExtension_h
#define __PolnCalibratorExtension_h

#include "Calibration/Complex2.h"
#include "Pulsar/CalibratorExtension.h"

namespace Pulsar {

  class PolnCalibrator;

  //! PolnCalibrator Extension
  /*! This Extension implements the storage of PolnCalibrator data. */
  
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

    //! Construct from a PolnCalibrator instance
    PolnCalibratorExtension (const PolnCalibrator*);

    //! Set the type of the instrumental response parameterization
    void set_type (Calibrator::Type type);

    //! Set the number of frequency channels
    void set_nchan (unsigned nchan);

    //! Set the weight of the specified channel
    void set_weight (unsigned ichan, float weight);

    //! Return true if the transformation for the specified channel is valid
    bool get_valid (unsigned ichan) const;
    void set_valid (unsigned ichan, bool valid);

    //! Get the transformation for the specified frequency channel
    ::Calibration::Complex2* get_transformation (unsigned c);
    //! Get the transformation for the specified frequency channel
    const ::Calibration::Complex2* get_transformation (unsigned c) const;

  protected:

    //! The instrumental response as a function of frequency
    vector< Reference::To< ::Calibration::Complex2 > > response;

    //! Return a new Calibration::Complex2 instance, based on type attribute
    ::Calibration::Complex2* new_transformation ();

    //! Construct the response array according to the current attributes
    void construct ();

  };
 

}

#endif
