//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2011 by Andrew Jameson
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/SpectralKurtosis.h,v $
   $Revision:  $
   $Date:  $
   $Author: $ */

#ifndef __Pulsar_SpectralKurtosis_h
#define __Pulsar_SpectralKurtosis_h

#include "Pulsar/IntegrationExtension.h"
#include "TextInterface.h"

namespace Pulsar {
  
  //! Spectral Kurtosis Staticstics calculated during observation
  class SpectralKurtosis : public Pulsar::Integration::Extension {
    
  public:
    
    //! Default constructor
    SpectralKurtosis ();

    //! Copy constructor
    SpectralKurtosis (const SpectralKurtosis& extension);

    //! Assignment operator
    const SpectralKurtosis& operator= (const SpectralKurtosis& extension);
    
    //! Destructor
    ~SpectralKurtosis ();

    //! Clone method
    SpectralKurtosis* clone () const { return new SpectralKurtosis( *this ); }
    
    //! Return a text interfaces that can be used to access this instance
    TextInterface::Parser* get_interface();
    
    // Text interface to a SpectralKurtosis instance
    class Interface : public TextInterface::To<SpectralKurtosis>
    {
      public:
          Interface( SpectralKurtosis *s_instance = NULL );
    };

    //! Addition operator
    const SpectralKurtosis& operator += (const SpectralKurtosis& extension);

    //! Resize the extension
    void resize (unsigned npol, unsigned nchan);

    //! Integrate information from another Integration
    void integrate (const Integration* subint);

    //! Update information based on the provided Integration
    void update (const Integration* subint);

    //! Set the base integration factor used to calculate the SK statistic
    void set_M (unsigned _M);
    //! Get the base integration factor used in the SK statistic
    unsigned get_M() const;

    //! Set the excision threshold in terms of std deviations
    void set_excision_threshold (unsigned nsigma);
    //! Get the excision threshold in terms of std deviations
    unsigned get_excision_threshold () const;

    //! Get the filtered sum of the specified channel and polarization 
    float get_filtered_sum (unsigned ichan, unsigned ipol);
    //! Set the filtered sum of the specified channel and polarization 
    void set_filtered_sum (unsigned ichan, unsigned ipol, float sum);

    //! Get the filtered hits of the specified channel and polarization 
    uint64_t get_filtered_hits (unsigned ichan, unsigned ipol);
    //! Set the filtered hits of the specified channel and polarization 
    void set_filtered_hits (unsigned ichan, unsigned ipol, uint64_t hits);

    //! Get the unfiltered sum of the specified channel and polarization 
    float get_unfiltered_sum (unsigned ichan, unsigned ipol);
    //! Set the unfiltered sum of the specified channel and polarization 
    void set_unfiltered_sum (unsigned ichan, unsigned ipol, float sum);

    // Get the unfiltered hits, common to all channels and polarizations
    uint64_t get_unfiltered_hits ();
    // Set the unfiltered hits, common to all channels and polarizations
    void set_unfiltered_hits (uint64_t hits);

  protected:

    //! number of polarizations 
    unsigned npol;

    //! number of channels 
    unsigned nchan;

    //! Number of samples integrated into each atomic SK statistic
    unsigned M;
    
    //! Threshold used to excise RFI in SK Detection
    unsigned nsigma;

    //! Average SK statistic for each channel, post f/t scrunch zapping
    std::vector<float> filtered_sum;

    //! Hits on filtered average for each channel
    std::vector<uint64_t> filtered_hits;

    //! Average SK statistic for each channel formed from block length SK estimates
    std::vector<float> unfiltered_sum;

    //! Hits on unfiltered SK statistic, same for each channel/pol
    uint64_t unfiltered_hits;

    //! Ensure that ichan < nchan and ipol < npol
    void range_check (unsigned ichan, unsigned ipol, const char* method) const;

  };
  
}

#endif

