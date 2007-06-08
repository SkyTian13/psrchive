//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Plotting/Pulsar/PhaseVsPlot.h,v $
   $Revision: 1.13 $
   $Date: 2007/06/08 00:40:24 $
   $Author: straten $ */

#ifndef __Pulsar_PhaseVsPlot_h
#define __Pulsar_PhaseVsPlot_h

#include "Pulsar/PhasePlot.h"
#include "ColourMap.h"

namespace Pulsar {

  class Profile;

  //! Plots images of something as a function of pulse phase vs something
  class PhaseVsPlot : public PhasePlot {

  public:

    //! Default constructor
    PhaseVsPlot ();

    //! Get the text interface to the configuration attributes
    TextInterface::Class* get_interface ();

    // Text interface to the PhaseVsPlot class
    class Interface : public TextInterface::To<PhaseVsPlot> {
    public:
      Interface (PhaseVsPlot* = 0);
    };

    //! Derived classes must provide the number of rows
    virtual unsigned get_nrow (const Archive* data) = 0;

    //! Derived classes must provide the Profile for the specified row
    virtual const Profile* get_Profile (const Archive* data, unsigned row) = 0;

    //! Draw in the current viewport
    void draw (const Archive*);

    //! Provide access to the z-scale
    PlotScale* get_z_scale () { return &z_scale; }

    //! Provide access to the colour map
    pgplot::ColourMap* get_colour_map () { return &colour_map; }

    //! Set the style
    void set_style (const std::string&);
    //! Get the style
    std::string get_style () const { return style; }

  protected:

    PlotScale z_scale;

    pgplot::ColourMap colour_map;

    //! The style in which the data will be plotted
    std::string style;

  };

}

#endif
