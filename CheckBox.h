#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QWidget>

class CheckBox : public QWidget
{
  Q_OBJECT
public:
  explicit CheckBox(QWidget *parent = nullptr);

  void setValue(bool value);
  bool value() const;

signals:

public slots:

private:
  class QCheckBox *m_value;
};

#endif // COMBOBOX_H
