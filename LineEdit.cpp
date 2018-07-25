#include "LineEdit.h"

#include <QKeyEvent>
#include <QValidator>

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent)
{}

LineEdit::LineEdit(const QString &str) : QLineEdit(str)
{}

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
