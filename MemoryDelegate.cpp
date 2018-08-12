#include "CheckBox.h"
#include "LineEdit.h"
#include "MemoryDelegate.h"
#include "MemoryTableModel.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
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
  QVariant data = index.data();
  if(index.row() == 0 || column == 0)
  {
    resultOption.backgroundBrush = m_header;
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem,
                                       &resultOption, painter);
  }
  else if(column > 11 || column == 10 || column == 9)
  {
//    QItemDelegate::drawCheck(painter, option, option.rect,
//                             data.toBool() ? Qt::Checked
//                                           : Qt::Unchecked);
//    return;
    QStyleOptionButton style;
    style.rect = option.rect;
    style.state = index.data().toBool() ? QStyle::State_On
                                        : QStyle::State_Off;
    style.state |= index.flags() & Qt::ItemIsEnabled ? QStyle::State_Enabled
                                                     : QStyle::State_None;
    QApplication::style()->drawControl(QStyle::CE_CheckBox,
                                       &style, painter);
    return;
  }
  QItemDelegate::paint(painter, resultOption, index);
  return;
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
    widget->setSingleStep(0.00025);
    widget->setDecimals(5);
    return widget;
  }
  case 4:
  {
    LineEdit *widget = new LineEdit(parent);
    static QRegExpValidator validator(QRegExp("[A-Z0-9 ]{,7}"));
    widget->setValidator(&validator);
//    widget->setInputMask(">nnnnnnn");
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
    QCheckBox *widget = new QCheckBox(parent);
    return widget;
  }
  default:
  {
    QComboBox *widget = new QComboBox(parent);
    MemoryTableModel *model = qobject_cast<MemoryTableModel*>(
          const_cast<QAbstractItemModel*>(index.model()));
    widget->addItems(model->variants(index.column(), index.row() - 1));
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
  QCheckBox *check = qobject_cast<QCheckBox*>(editor);
  if(check != nullptr)
    check->setChecked(!index.data(Qt::EditRole).toBool());
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
  QCheckBox *check = qobject_cast<QCheckBox*>(editor);
  if(check != nullptr)
    value = check->isChecked();
  model->setData(index, value);
}
