#include "LineEdit.h"
#include "Tab5Tone.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
//#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>

const QMap<QString, char> actionMap = {
  {"None", 0}, {"Squelch Off", 1},
  {"Beep Tone", 2}, {"Beep Tone & Respond", 3}
};
typedef QPair<qint16, QRegExp> DecodeStandart;
const QMap<QString, DecodeStandart> decodeStandartMap = {
  {"EEA", {1, QRegExp("[0-9A-F]{,12}")}},
  {"EIA", {2, QRegExp("[0-9A-E]{,12}")}},
  {"CCIT", {3, QRegExp("[0-9A-E]{,12}")}},
  {"ZVEI1", {4, QRegExp("[0-9A-F]{,12}")}},
  {"ZVEI3", {5, QRegExp("[0-9AE]{,12}")}},
  {"PZVEI1", {6, QRegExp("[0-9A-F]{,12}")}},
  {"DVEI1", {7, QRegExp("[0-9A-F]{,12}")}},
  {"PDVEI1", {8, QRegExp("[0-9A-E]{,12}")}},
  {"CCIR1", {9, QRegExp("[0-9A-F]{,12}")}},
  {"CCIR2", {10, QRegExp("[0-9A-F]{,12}")}},
  {"PCCIR", {11, QRegExp("[0-9A-E]{,12}")}},
  {"EUROSIGNAL", {12, QRegExp("[0-9E]{,12}")}},
  {"NATEL", {13, QRegExp("[0-9A-F]{,12}")}},
  {"MODAL", {14, QRegExp("[0-9A-E]{,12}")}},
};

Tab5Tone::Tab5Tone(QWidget *parent) : QWidget(parent), m_decodeStandart(new QComboBox),
  m_pretime(new QSpinBox), m_response(new QDoubleSpinBox),
  m_start(new LineEdit("12345")), m_end(new LineEdit("12345")),
  m_validator(new QRegExpValidator(QRegExp("[0-9A-F]{,12}")))
{

  QHBoxLayout *hl = new QHBoxLayout(this);
  QVBoxLayout *vl = new QVBoxLayout;
  hl->addLayout(vl);
  QFormLayout *fl = new QFormLayout;
  vl->addLayout(fl);
  fl->addRow(tr("Decode Standart"), m_decodeStandart);
  fl->addRow(tr("Pretime"), m_pretime);
  fl->addRow(tr("Time to response"), m_response);
  fl->addRow(tr("PTT ID Starting"), m_start);
  fl->addRow(tr("PTT ID Ending"), m_end);
  QGroupBox *box = new QGroupBox(tr("Decode"));
  vl->addWidget(box);
  QGridLayout *gl = new QGridLayout(box);
  for(int i=0; i<4; i++)
  {
    LineEdit *code = new LineEdit;
    static QRegExpValidator validator(QRegExp("[0-9A-F]{,5}"));
    code->setValidator(&validator);
//    code->setInputMask(">HHHHH");
    code->setText("12345");
    gl->addWidget(code, i, 0);
    QComboBox *action = new QComboBox;
    action->addItems(actionMap.keys());
    gl->addWidget(action, i, 1);
    m_decode[i].first = code;
    m_decode[i].second = action;
  }
  QScrollArea *scrol = new QScrollArea;
  hl->addWidget(scrol);
  QWidget *w = new QWidget;
  scrol->setWidget(w);
  scrol->setWidgetResizable(true);
  fl = new QFormLayout(w);
  fl->addRow(tr("No"), new QLabel(tr("Special Call")));
  static QRegExpValidator validator(QRegExp("[0-9A-F]{,12}"));
  for(int i=1; i<101; i++)
  {
    LineEdit *edit = new LineEdit;
    edit ->setValidator(m_validator);
//    edit->setInputMask(">HHHHHHHHHHHH");
    fl->addRow(QString::number(i), edit);
    m_specialCall[i - 1] = edit;
  }
  m_start->setValidator(&validator);
//  m_start->setInputMask(">HHHHHHHHHHHH");
  m_end->setValidator(&validator);
//  m_end->setInputMask(">HHHHHHHHHHHH");
  m_pretime->setMinimum(500);
  m_pretime->setMaximum(1000);
  m_pretime->setSingleStep(100);
  m_pretime->setSuffix(tr("ms"));
  m_response->setMinimum(1);
  m_response->setMaximum(3);
  m_response->setSingleStep(0.5);
  m_response->setSuffix(tr("s"));

  m_decodeStandart->addItems(decodeStandartMap.keys());
  connect(m_decodeStandart, SIGNAL(currentTextChanged(QString)),
          this, SLOT(newDecodeStandart(QString)));
  m_decodeStandart->setCurrentText("EEA");
}

QStringList Tab5Tone::numbers()
{
  QStringList list;
  for(int i=0; i<100; i++)
    if(!m_specialCall[i]->text().isEmpty())
      list << QString::number(i + 1);
  return list;
}

void Tab5Tone::newDecodeStandart(const QString &value)
{
  m_validator->setRegExp(decodeStandartMap[value].second);
}
