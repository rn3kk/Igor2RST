#include "MainWindow.h"
#include "MemoryDelegate.h"
#include "MemoryTableModel.h"
#include "RadioDevice.h"
#include "Tab2Tone.h"
#include "Tab5Tone.h"

#include <QFile>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
#include <QStatusBar>
#include <QTabBar>
#include <QTableView>
#include <QTextStream>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), m_tone2(new Tab2Tone),
  m_tone5(new Tab5Tone), m_memory(new MemoryTableModel(*m_tone5)),
  m_name(new QLabel), m_version(new QLabel),
  m_busy(new QLabel),
  m_device(new RadioDevice)
{
  QStatusBar *status = new QStatusBar(this);
  setStatusBar(status);
  status->addPermanentWidget(m_busy);
  QWidget *central = new QWidget(this);
  setCentralWidget(central);
  QVBoxLayout *vl = new QVBoxLayout(central);
  vl->setSpacing(0);
  vl->setContentsMargins(0, 0, 0, 0);
  QHBoxLayout *hl = new QHBoxLayout;
  vl->addLayout(hl);
  QFormLayout *fl = new QFormLayout;
  hl->addLayout(fl);
  fl->addRow(tr("Name"), m_name);
  fl->addRow(tr("Version  "), m_version);
  hl->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
  QTabBar *tabs = new QTabBar;
  vl->addWidget(tabs);
  QStackedLayout *sl = new QStackedLayout;
  vl->addLayout(sl);
  connect(tabs, SIGNAL(currentChanged(int)),
          sl, SLOT(setCurrentIndex(int)));
  QTableView *memoryView = new QTableView(this);
  sl->addWidget(memoryView);
  sl->addWidget(m_tone2);
  sl->addWidget(m_tone5);

  QPushButton *button = new QPushButton(tr("Find"));
  button->setObjectName("Find");
  connect(button, SIGNAL(clicked(bool)),
          m_device, SLOT(reconnect()));
  hl->addWidget(button);
  button = new QPushButton(tr("Read"));
  button->setObjectName("Read");
  connect(button, SIGNAL(clicked(bool)),
          this, SLOT(read()));
  hl->addWidget(button);
  button = new QPushButton(tr("Write"));
  button->setObjectName("Write");
  connect(button, SIGNAL(clicked(bool)),
          this, SLOT(write()));
  hl->addWidget(button);


  tabs->addTab(tr("CH Information"));
  tabs->addTab(tr("2 TONE"));
  tabs->addTab(tr("5 TONE"));

  memoryView->setModel(m_memory);
  memoryView->horizontalHeader()->hide();
  memoryView->verticalHeader()->hide();
  memoryView->setEditTriggers(QTableView::CurrentChanged |
                              QTableView::SelectedClicked);
  memoryView->setItemDelegate(new MemoryDelegate);
  memoryView->resizeColumnsToContents();

  connect(m_device, SIGNAL(connected(QString,QString)),
          SLOT(radioConnect(QString,QString)));

  setMinimumSize(1020, 500);
}

void MainWindow::read()
{
  QByteArrayList memory;
  QByteArray tone2, tone5;
  m_busy->setText(tr("read data from device"));
  m_device->read(memory, tone2, tone5);
  m_memory->read(memory);
  m_tone2->read(tone2);
  m_tone5->read(tone5);
  m_busy->setText("");
}

void MainWindow::write()
{
  m_busy->setText(tr("write data to device"));
  m_device->write(m_memory->toWrite(false),
                  m_tone2->toWrite(), m_tone5->toWrite());
  m_busy->setText("");
}

void MainWindow::radioConnect(const QString &name, const QString &version)
{
  m_busy->setText(tr("find device"));
  m_name->setText(name);
  m_version->setText(version);
  m_busy->setText("");
}
