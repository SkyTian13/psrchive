#ifndef __QT_TOGLEPH_H
#define __QT_TOGLEPH_H

#include <vector>

#include <qcheckbox.h>
#include <psr_cpp.h>

class qt_toglParams : public QWidget
{
  Q_OBJECT

  friend class qt_psrParams;

 public:
  qt_toglParams (QWidget* parent=0, const char* name=0, int columns=4);

 signals:
  void getChecked ( int ephind, bool checked );

 protected slots:
  void setChecked ( int ephind, bool checked )
   { elements[ephind]->setChecked (checked); };

 private slots:
  void wasChecked ( int ephind )
    { emit getChecked ( ephind, elements[ephind]->isChecked() ); };

 private:
  vector<QCheckBox*> elements;

};

#endif
