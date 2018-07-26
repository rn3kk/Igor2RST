#ifndef TAB5TONE_H
#define TAB5TONE_H

#include <QObject>
#include <QWidget>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class LineEdit;

class Tab5Tone : public QWidget
{
  Q_OBJECT
public:
  explicit Tab5Tone(QWidget *parent = nullptr);

  QStringList numbers();

signals:

public slots:
  void newDecodeStandart(const QString &value);

private:
  QComboBox *m_decodeStandart;
  QSpinBox *m_pretime;
  QDoubleSpinBox *m_response;
  QPair<LineEdit*, QComboBox*> m_decode[4];
  LineEdit *m_start, *m_end;
  LineEdit *m_specialCall[100];
  class QRegExpValidator *m_validator;
};

#endif // TAB5TONE_H
