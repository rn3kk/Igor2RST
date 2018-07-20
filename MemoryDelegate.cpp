#include "CheckBox.h"
#include "MemoryDelegate.h"
#include "MemoryTableModel.h"

#include <QApplication>
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
  else if(column > 11 || column == 10 || column == 9)
  {
    QItemDelegate::drawCheck(painter, option, option.rect,
                             index.data().toBool() ? Qt::Checked
                                                   : Qt::Unchecked);
    return;
//    QStyleOptionButton style;
//    style.rect = option.rect;
//    style.state = index.data().toBool() ? QStyle::State_On
//                                        : QStyle::State_Off;
//    QApplication::style()->drawControl(QStyle::CE_CheckBox,
//                                       &style, painter);
  }
  resultOption.text = index.data().toString();
  QApplication::style()->drawControl(QStyle::CE_ItemViewItem,
                                       &resultOption, painter);
}

QWidget *MemoryDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
  if(!index.isValid() || index.column() == 0 || index.row() == 0)
    return new QWidget(parent);
#warning not full released
  QVariant variant = index.data();
  int column = index.column();
  switch(column)
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
    static QRegExpValidator *validator = new QRegExpValidator(QRegExp("[A-Z0-9]{,7}"));
    widget->setValidator(validator);
    return widget;
  }
  case 9:
  case 10:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  {
    CheckBox *widget = new CheckBox(parent);
    return widget;
  }
  default:
  {
    QComboBox *widget = new QComboBox(parent);
    widget->addItems(MemoryTableModel::variants(index.column()));
    return widget;
  }
  }
}

void MemoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  if(comboBox != nullptr)
    comboBox->setCurrentText(index.data(Qt::EditRole).toString());
  QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
  if(lineEdit != nullptr)
    lineEdit->setText(index.data(Qt::EditRole).toString());
  QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
  if(spinBox != nullptr)
    spinBox->setValue(index.data(Qt::EditRole).toDouble());
  CheckBox *check = qobject_cast<CheckBox*>(editor);
  if(check != nullptr)
    check->setValue(index.data(Qt::EditRole).toBool());
}

void MemoryDelegate::setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
  QVariant value;
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  if(comboBox != nullptr)
    value = comboBox->currentText();
  QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
  if(lineEdit != nullptr)
    value = lineEdit->text();
  QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
  if(spinBox != nullptr)
    value = spinBox->value();
  CheckBox *check = qobject_cast<CheckBox*>(editor);
  if(check != nullptr)
    value = check->value();
  model->setData(index, value);
}
