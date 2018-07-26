#include "LineEdit.h"

#include <QKeyEvent>
#include <QValidator>

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent)
{}

LineEdit::LineEdit(const QString &str) : QLineEdit(str)
{}

void LineEdit::setValidator(const QValidator *validator)
{
  connect(validator, SIGNAL(changed()),
          this, SLOT(validatorCanged()));
  QLineEdit::setValidator(validator);
}

void LineEdit::validatorCanged()
{
  QString input = text(), out;
  int pos = 0;
  QRegExp regex = qobject_cast<const QRegExpValidator*>(validator())->regExp();
  int start = regex.indexIn(input, pos);
  int len = input.length();
  while(start != -1 && start != len)
  {
    QString cap =regex.cap();
    out += cap;
    pos = start + cap.length();
    if(cap.length() == 0)
      pos++;
    start = regex.indexIn(input, pos);
  }
  setText(out);
}

void LineEdit::keyPressEvent(QKeyEvent *event)
{
  int key = event->key();
  if(key == Qt::Key_Backspace || key == Qt::Key_Left ||
     key == Qt::Key_Delete || key == Qt::Key_Right
     )
  {
    QLineEdit::keyPressEvent(event);
    return;
  }
  QString value = text();
  int position = cursorPosition();
  value = value.insert(position, event->text()).toUpper();
  int pos = 0;
  if(validator()->validate(value, pos) == QValidator::Acceptable)
  {
    setText(value);
    setCursorPosition(++position);
  }
}
