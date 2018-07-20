#include "CheckBox.h"

#include <QCheckBox>
#include <QHBoxLayout>

CheckBox::CheckBox(QWidget *parent) :
  QWidget(parent), m_value(new QCheckBox(this))
{
  QHBoxLayout *hl = new QHBoxLayout(this);
  hl->setMargin(0);
  hl->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
  hl->addWidget(m_value);
  hl->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
}

void CheckBox::setValue(bool value)
{
  m_value->setChecked(value);
}

bool CheckBox::value() const
{
  return m_value->isChecked();
}
