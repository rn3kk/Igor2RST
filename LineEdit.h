#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>



class LineEdit : public QLineEdit
{
  Q_OBJECT
public:
  LineEdit(QWidget *parent = nullptr);
  LineEdit(const QString &str);

  // QWidget interface
protected:
  void keyPressEvent(QKeyEvent *event) override;
};

#endif // LINEEDIT_H
