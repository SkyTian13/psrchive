//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/Error.h,v $
   $Revision: 1.1 $
   $Date: 2004/11/22 09:52:36 $
   $Author: straten $ */

#ifndef __Error_h
#define __Error_h

class Error;

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "psr_cpp.h"

//! List of error types
enum ErrorCode {
  //! undefined error
  Undefined,
  //! operator new has returned null
  BadAllocation,
  //! General bad pointer
  InvalidPointer,
  //! general bad parameter
  InvalidParam,
  //! general invalid state
  InvalidState,
  //! index out of range, or range mismatch
  InvalidRange,
  //! invalid state of Profile for requested operation
  InvalidPolnMeasure,
  //! invalid state of Integration for requested operation
  InvalidPolnState,
  //! file not found
  FileNotFound,
  //! failure of a function called from another lib
  FailedCall,
  //! failure of a system call that sets errno
  FailedSys,
  //! signal received from kernel
  SignalReceived
};

//! A convenient exception handling class
/*
  It is useful to throw an exception with both an error code and a
  text message.  As well, on catching an exception, it is useful if a
  function or method can add its name to an "exception stack",
  enabling the error to be traced back to its origin.  The static
  method, Error::handle_signals(), may also be invoked so that signal
  interrupts, such as SIGFPE and SIGSEGV, will be intercepted and an
  Error exception will be thrown. 
*/
class Error {

  public:

  //! convert an error code to a character string
  static const char* err2str (ErrorCode code);

  //! During constructor, print messages
  static bool verbose;

  //! After construction raise a sigkill
  static bool complete_abort;

  //! Error with optional printf-style message
  Error (ErrorCode c, string func, const char* msg=0, ...);

  //! Error with string message
  Error (ErrorCode c, string func, string msg);
  
  //! Virtual destructor enables inheritance
  virtual ~Error ();

  //! Add function name to the list
  const Error& operator += (const char* func);
  
  //! Add function name to the list
  const Error& operator += (const string& func);
  
  //! Write error and auxilliary info to ostream
  virtual void report (ostream& ostr) const;
  
  //! Write function name and error message to screen
  virtual string warning () const;

  //! Get the error message
  const string get_message() const;
  
  //! Add to the Error message
  template<class T>
  friend Error& operator<< (Error& error, const T& t)
  {
    ostringstream ostr;
    ostr << t;
    error.message += ostr.str();
    return error;
  }

protected:

  //! the error code
  ErrorCode code;
  //! auxilliary message
  string message;
  //! function name stack
  vector<string> functions;

  //! Null constructor is protected
  Error ();

  //! If errno is set, adds a string to message
  void errno_check ();
  
  //! Contruct the basic deal
  void construct (ErrorCode c, const string& func, const char* msg);
};

//! Convenience output operator in global namespace
ostream& operator<< (ostream& ostr, const Error& error);


#endif
