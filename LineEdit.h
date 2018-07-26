#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>



class LineEdit : public QLineEdit
{
  Q_OBJECT
public:
  LineEdit(QWidget *parent = nullptr);
  LineEdit(const QString &str);

  void setValidator(const QValidator *validator);

private slots:
  void validatorCanged();

  // QWidget interface
protected:
  void keyPressEvent(QKeyEvent *event) override;
};

#endif // LINEEDIT_H
