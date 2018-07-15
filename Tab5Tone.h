#ifndef TAB5TONE_H
#define TAB5TONE_H

#include <QObject>
#include <QWidget>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class Tab5Tone : public QWidget
{
  Q_OBJECT
public:
  explicit Tab5Tone(QWidget *parent = nullptr);

signals:

public slots:

private:
  QComboBox *m_numbers[100];
  QComboBox *m_decodeStandart;
//  QComboBox *
};

#endif // TAB5TONE_H
