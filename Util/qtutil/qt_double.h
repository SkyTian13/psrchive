#ifndef __QT_DOUBLE_H
#define __QT_DOUBLE_H

#include "qt_value.h"
#include "psr_cpp.h"

class qt_double : public qt_value
{
  Q_OBJECT

 public:
  qt_double (bool error=false, QWidget *parent=NULL, const char *name=NULL);

  // error given in days
  void   setDouble (double d_val = 0.0);
  double getDouble () const { return valset; };

 protected:
  double  valset;

 protected slots:
  void value_Entered_CB ();
};

#endif
