#ifndef TAB2TONE_H
#define TAB2TONE_H

#include <QObject>
#include <QWidget>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class Tab2Tone : public QWidget
{
  Q_OBJECT
public:
  explicit Tab2Tone(QWidget *parent = nullptr);

  QByteArray toWrite() const;
  void read(QByteArray &data);

signals:

private:
  QSpinBox *m_pretime;
  QSpinBox *m_a, *m_b, *m_c, *m_d;
  QDoubleSpinBox *m_response;
  QPair<QComboBox*, QComboBox*> m_decode[4];
  QComboBox *m_start;
  QComboBox *m_end;
};

#endif // TAB2TONE_H
