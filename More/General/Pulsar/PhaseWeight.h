//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/PhaseWeight.h,v $
   $Revision: 1.11 $
   $Date: 2007/07/30 06:44:50 $
   $Author: straten $ */

#ifndef __Pulsar_PhaseWeight_h
#define __Pulsar_PhaseWeight_h

#include "Reference.h"
#include "Estimate.h"

namespace Pulsar {

  class Profile;

  //! Stores a weight for each Profile phase bin
  /*! This class implements a useful set of methods required to create,
    store, and manipulate a weighted set of Profile phase bins. */
  class PhaseWeight : public Reference::Able {

  public:

    //! Default constructor
    PhaseWeight ();

    PhaseWeight (unsigned nbin);
    PhaseWeight (unsigned nbin, float set_all);

    //! Copy constructor
    PhaseWeight (const PhaseWeight& weight);

    //! Copy vector of float constructor
    PhaseWeight (const std::vector<float>& weight);

    //! Destructor
    ~PhaseWeight ();
    
    //! Assignment operator
    const PhaseWeight& operator = (const PhaseWeight& weight);

    //! Forms the sum of this and weight
    const PhaseWeight& operator += (const PhaseWeight& weight);

    //! Forms the product of this and weight
    const PhaseWeight& operator *= (const PhaseWeight& weight);

    //! Array operator
    float& operator [] (unsigned i) { built = false; return weight[i]; }

    //! Array operator
    const float& operator [] (unsigned i) const { return weight[i]; }

    //! Resize the weights array
    void resize (unsigned nbin) { weight.resize(nbin); }

    //! Get the number of weights in the array
    unsigned get_nbin () const { return weight.size(); }

    //! Get the reference frequency
    double get_frequency () const { return reference_frequency; }

    //! Set the reference frequency
    void set_frequency (double f) { reference_frequency = f; }

    //! Set all weights to the specified value
    void set_all (float weight);
  
    //! Retrieve the sum of all weights
    double get_weight_sum () const;

    //! Retrieve the max of all weights
    double get_weight_max () const;

    //! Retrieve the weights
    void get_weights (std::vector<float>& weights) const;

    //! Weight the Profile amplitudes by the weights
    void weight_Profile (Profile* profile) const;

    //! Set the Profile from which statistics are calculated
    void set_Profile (const Profile* profile);

    //! Get the weighted total of the amplitudes
    double get_weighted_sum () const;

    //! Get the weighted mean of the Profile amplitudes
    Estimate<double> get_mean () const;

    //! Get the weighted variance of the Profile amplitudes
    Estimate<double> get_variance () const;

    //! Get the statistics of the weighted phase bins (deprecated interface)
    void stats (const Profile* profile,
		double* mean, double* variance=0, 
		double* varmean=0, double* varvar=0) const;

    //! Return the base address of the weights array
    float* get_weights () { return &(weight[0]); }

    //! Return the base address of the weights array
    const float* get_weights() const { return &(weight[0]); }

  protected:

    //! The reference frequency
    double reference_frequency;

    //! The weights
    std::vector<float> weight;

    //! The Profile to which the weights apply
    Reference::To<const Profile> profile;

    //! Flag set when the statistics have been calculated
    bool built;

    //! The weighted mean of the Profile
    Estimate<double> mean;

    //! The weighted variance of the Profile
    Estimate<double> variance;

    //! Compute the mean and variance attributes
    void build ();

    //! Initialize attributes to default values
    void init ();

  };

}


#endif // !defined __Pulsar_PhaseWeight_h
