//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/CalibratorExtension.h,v $
   $Revision: 1.13 $
   $Date: 2009/02/20 06:24:18 $
   $Author: straten $ */

#ifndef __CalibratorExtension_h
#define __CalibratorExtension_h

#include "Pulsar/ArchiveExtension.h"
#include "Pulsar/Calibrator.h"

namespace Pulsar {

  //! Stores Calibrator parameters in an Archive
  /*! This Archive::Extension class implements the storage of Calibrator
    data, enabling a calibrator to be constructed from file. */
  class CalibratorExtension : public Archive::Extension
  {   
  public:
    
    //! Default constructor
    CalibratorExtension (const char* name = "CalibratorExtension");

    //! Copy constructor
    CalibratorExtension (const CalibratorExtension&);

    //! Operator =
    const CalibratorExtension& operator= (const CalibratorExtension&);

    //! Destructor
    ~CalibratorExtension ();

    //! Clone method
    CalibratorExtension* clone () const
    { return new CalibratorExtension( *this ); }

    //! Construct from a Calibrator instance
    void build (const Calibrator*);

    //! Set the type of the calibrator
    virtual void set_type (Calibrator::Type type);
    //! Get the type of the calibrator
    virtual Calibrator::Type get_type () const;

    //! Set the reference epoch of the calibration experiment
    virtual void set_epoch (const MJD& epoch);
    //! Get the reference epoch of the calibration experiment
    virtual MJD get_epoch () const;

    //! Set the number of frequency channels
    virtual void set_nchan (unsigned nchan);
    //! Get the number of frequency channels
    virtual unsigned get_nchan () const;

    //! Set the weight of the specified channel
    virtual void set_weight (unsigned ichan, float weight);
    //! Get the weight of the specified channel
    virtual float get_weight (unsigned ichan) const;

    //! Set the centre frequency of the specified channel
    virtual void set_centre_frequency (unsigned ichan, double frequency);
    //! Get the centre frequency of the specified channel
    virtual double get_centre_frequency (unsigned ichan) const;

    //! Return a short name
    std::string get_short_name () const { return "cal"; }

    class Interface;

    //! Return a text interfaces that can be used to access this instance
    TextInterface::Parser* get_interface();

    protected:

    //! Type of the calibrator
    Calibrator::Type type;
    
    //! The reference epoch of the calibration experiment
    MJD epoch;

    //! The weight of each channel
    std::vector<float> weight;

    //! The centre frequency of each channel
    std::vector<double> centre_frequency;

    //! Ensure that ichan < get_nchan
    void range_check (unsigned ichan, const char* method) const;

  };
 

}

#endif
