//-*-C++-*-

// Class for holding processing history

#ifndef __ProcHistory_h
#define __ProcHistory_h

#include "Pulsar/Archive.h"
#include "Pulsar/Integration.h"
#include "Pulsar/Profile.h"

#include "environ.h"

namespace Pulsar {
  
  class ProcHistory : public Pulsar::Archive::Extension {
    
  public:
    
    //Default constructor
    ProcHistory () { init(); }
    
    // Copy constructor
    ProcHistory (const Pulsar::ProcHistory& extension);
    
    // Operator =
    const ProcHistory& operator= (const ProcHistory& extension);
    
    // Destructor
    ~ProcHistory ();
    
    ///////////////////////////////////////////////////////////////
    // The information container
    
    class row {
      
    public:
      
      // Default constructor
      row () { init(); }
      
      // Row label
      int index;
      
      // Information in the row
      string date_pro;
      string proc_cmd;
      string pol_type;
      int npol;
      int nbin;
      int nbin_prd;
      double tbin;
      double ctr_freq;
      int nchan;
      double chanbw;
      int par_corr;
      int rm_corr;
      int dedisp;
      string sc_mthd;
      string cal_mthd;
      string cal_file;
      string rfi_mthd;
      
      //Destructor
      ~row ();
      
      //Initialisation
      void init ();
      
    };
    
    // The storage array
    vector<row> rows;
    
    ProcHistory::row& get_last ();
    void add_blank_row ();
    
  private:
    
    void init ();
    
  };
  
}

#endif
