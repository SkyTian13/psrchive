//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2008 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Util/genutil/Mount.h,v $
   $Revision: 1.1 $
   $Date: 2008/07/02 11:23:23 $
   $Author: straten $ */

#ifndef __Mount_H
#define __Mount_H

#include "MJD.h"
#include "sky_coord.h"
#include "Matrix.h"

//! Calculates horizon pointing parameters using SLALIB

class Mount  {
  
public:
  
  //! Default constructor
  Mount ();

  //! Destructor
  virtual ~Mount ();

  //! Set the coordinates of the source
  void set_source_coordinates (const sky_coord& coords);
  
  //! Set the latitude of the observatory in radians
  void set_observatory_latitude (double latitude);
  double get_observatory_latitude () const;

  //! Set the longitude of the observatory in radians
  void set_observatory_longitude (double longitude);
  double get_observatory_longitude () const;

  //! Set the epoch in Modified Julian Days
  void set_epoch (const MJD& epoch);
  MJD get_epoch () const;

  //! Get the LST in radians
  double get_local_sidereal_time () const;

  //! Get the hour_angle in radians
  double get_hour_angle () const;

  //! Get the vertical angle (rotation about the line of sight)
  virtual double get_vertical () const = 0;

protected:

  //! The declination of the source in radians
  double declination;

  //! The right ascension of the source in radians
  double right_ascension;

  //! The latitude of the observatory in radians
  double latitude;

  //! The latitude of the observatory in radians
  double longitude;

  //! The epoch
  MJD epoch;

  //! The LST in radians
  mutable double lst;

  //! The hour angle in radians
  mutable double hour_angle;

  //! Basis pointing to source in celestial reference frame
  mutable Matrix<3,3,double> source_basis;

  //! Basis pointing to observatory in celestial reference frame
  mutable Matrix<3,3,double> observatory_basis;

  //! Lazily evaluate lst and hour_angle
  virtual void build () const;

  //! Return true when outputs reflect inputs
  bool get_built () const;

};

#endif