#include "Tab2Tone.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <qcombobox.h>

const QMap<QString, char> codeMap = {
  {"AB", 0}, {"AC", 1}, {"AD", 2}, {"BA", 3}, {"BC", 4},
  {"BD", 5}, {"CA", 6}, {"CB", 7}, {"CD", 8}, {"DA", 9},
  {"DB", 10}, {"DC", 11}, {"AA", 12}, {"BB", 13},
  {"CC", 14}, {"DD", 15}
};
const QMap<QString, char> actionMap = {
  {"None", 0}, {"Squelch Off", 1},
  {"Beep Tone", 2}, {"Beep Tone & Respond", 3}
};

Tab2Tone::Tab2Tone(QWidget *parent) :
  QWidget(parent), m_pretime(new QSpinBox),
  m_a(new QSpinBox), m_b(new QSpinBox),
  m_c(new QSpinBox), m_d(new QSpinBox),
  m_response(new QDoubleSpinBox), m_start(new QComboBox),
  m_end(new QComboBox)
{
  QHBoxLayout *hl = new QHBoxLayout(this);
  QGroupBox *box = new QGroupBox(tr("Tone freq"));
  QFormLayout *fl = new QFormLayout(box);
  fl->addRow(tr("A"), m_a);
  fl->addRow(tr("B"), m_b);
  fl->addRow(tr("C"), m_c);
  fl->addRow(tr("D"), m_d);
  QVBoxLayout *vl = new QVBoxLayout;
  hl->addLayout(vl);
  vl->addWidget(box);
  box = new QGroupBox(tr("Decode"));
  vl->addWidget(box);
  QGridLayout *gl = new QGridLayout(box);
  for(int i=0; i<4; i++)
  {
    QComboBox *code = new QComboBox;
    code->addItems(codeMap.keys());
    gl->addWidget(code, i, 0);
    QComboBox *action = new QComboBox;
    action->addItems(actionMap.keys());
    gl->addWidget(action, i, 1);
    m_decode[i].first = code;
    m_decode[i].second = action;
  }
  vl->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
  fl = new QFormLayout;
  hl->addLayout(fl);
  fl->addRow(tr("Pretime"), m_pretime);
  fl->addRow(tr("Time to response"), m_response);
  fl->addRow(tr("PTT ID Starting"), m_start);
  fl->addRow(tr("PTT ID Ending"), m_end);

  m_a->setMinimum(380);
  m_a->setMaximum(3000);
  m_a->setSuffix(tr("Hz"));
  m_b->setMinimum(380);
  m_b->setMaximum(3000);
  m_b->setSuffix(tr("Hz"));
  m_c->setMinimum(380);
  m_c->setMaximum(3000);
  m_c->setSuffix(tr("Hz"));
  m_d->setMinimum(380);
  m_d->setMaximum(3000);
  m_d->setSuffix(tr("Hz"));
  m_pretime->setMinimum(500);
  m_pretime->setMaximum(1000);
  m_pretime->setSingleStep(100);
  m_pretime->setSuffix(tr("ms"));
  m_response->setMinimum(1);
  m_response->setMaximum(3);
  m_response->setSingleStep(0.5);
  m_response->setSuffix(tr("s"));
  m_start->addItems(codeMap.keys());
  m_start->setCurrentText("AB");
  m_end->addItems(codeMap.keys());
  m_end->setCurrentText("AB");
}
