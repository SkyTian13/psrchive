//-*-C++-*-

#ifndef __Pulsar_Archive_utils_h_
#define __Pulsar_Archive_utils_h_

#include "Pulsar/Profile.h"

namespace Pulsar {

  //! Sum the flux in the specified bins
  float sum_flux(const Profile* prof, int rise, int fall,
		 float min_phase = -1.0, float dc=Profile::default_duty_cycle);

  float sum_flux(const Profile* prof, float& sigma, int rise, int fall,
		 float min_phase = -1.0, float dc=Profile::default_duty_cycle);

  /*! Finding the bin numbers at which the flux falls below a threshold, 
    and sum the flux in those bins */
  //! Assumes profile is delta function
  float sum_flux(const Profile* prof,
		 float dropoff=Profile::default_amplitude_dropoff,
		 float min_phase = -1.0, float dc=Profile::default_duty_cycle);

  float sum_flux(const Profile* prof,float& sigma,
		 float dropoff=Profile::default_amplitude_dropoff,
		 float min_phase = -1.0, float dc=Profile::default_duty_cycle);
  
}

#endif
