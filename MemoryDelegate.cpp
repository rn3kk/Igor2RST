#include "ComboBox.h"
#include "MemoryDelegate.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

MemoryDelegate::MemoryDelegate() : QItemDelegate()
{
  m_header.setColor(QColor(159, 158, 95));
  m_header.setStyle(Qt::SolidPattern);
}

void MemoryDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
  QStyleOptionViewItem resultOption(option);
  int column = index.column();
  if(index.row() == 0 || column == 0)
  {
    resultOption.backgroundBrush = m_header;
//    QApplication::style()->drawControl(QStyle::CE_Header,
//                                       &option, painter);
  }
  else if(column > 11)
  {
    QItemDelegate::drawCheck(painter, option, option.rect,
                             index.data().toBool() ? Qt::Checked
                                                   : Qt::Unchecked);
    return;
    QStyleOptionButton style;
    style.rect = option.rect;
    style.state = index.data().toBool() ? QStyle::State_On
                                        : QStyle::State_Off;
    QApplication::style()->drawControl(QStyle::CE_CheckBox,
                                       &style, painter);
  }
  resultOption.text = index.data().toString();
  QApplication::style()->drawControl(QStyle::CE_ItemViewItem,
                                       &resultOption, painter);
}

QWidget *MemoryDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
#warning not full released
  switch(index.column())
  {
  case 2:
  case 1:
  {
    QDoubleSpinBox *widget = new QDoubleSpinBox(parent);
    widget->setMinimum(33.0);
    widget->setMaximum(48.5);
    widget->setSingleStep(0.05);
    return widget;
  }
  case 4:
  {
    QLineEdit *widget = new QLineEdit(parent);
    return widget;
  }
  case 16:
  case 12:
  case 13:
  case 14:
  case 15:
  {
    CheckBox *widget = new CheckBox(parent);
    return widget;
  }
  default:
  {
    QComboBox *widget = new QComboBox(parent);
    return widget;
  }
  }
}

void MemoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
#warning not released
}
