#ifndef TAB5TONE_H
#define TAB5TONE_H

#include <QObject>
#include <QWidget>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QLineEdit;

class Tab5Tone : public QWidget
{
  Q_OBJECT
public:
  explicit Tab5Tone(QWidget *parent = nullptr);

  QStringList numbers();

signals:

public slots:

private:
  QComboBox *m_decodeStandart;
  QSpinBox *m_pretime;
  QDoubleSpinBox *m_response;
  QPair<QLineEdit*, QComboBox*> m_decode[4];
  QLineEdit *m_start, *m_end;
  QLineEdit *m_specialCall[100];
};

#endif // TAB5TONE_H
