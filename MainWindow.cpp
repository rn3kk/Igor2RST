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
#include <QTabBar>
#include <QTableView>
#include <QTextStream>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), m_memory(nullptr),
  m_name(new QLabel), m_version(new QLabel),
  m_device(new RadioDevice)
{
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
  fl->addRow(tr("Version"), m_version);
  hl->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
  QTabBar *tabs = new QTabBar;
  vl->addWidget(tabs);
  QStackedLayout *sl = new QStackedLayout;
  vl->addLayout(sl);
  connect(tabs, SIGNAL(currentChanged(int)),
          sl, SLOT(setCurrentIndex(int)));
  QTableView *memoryView = new QTableView(this);
  sl->addWidget(memoryView);
  Tab2Tone *tone2 = new Tab2Tone;
  sl->addWidget(tone2);
  Tab5Tone *tone5 = new Tab5Tone;
  sl->addWidget(tone5);

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

  m_memory = new MemoryTableModel(*tone5);
  memoryView->setModel(m_memory);
  memoryView->horizontalHeader()->hide();
  memoryView->verticalHeader()->hide();
  memoryView->setEditTriggers(QTableView::CurrentChanged |
                              QTableView::SelectedClicked);
  memoryView->setItemDelegate(new MemoryDelegate);
  memoryView->resizeColumnsToContents();

  setMinimumSize(1040, 500);
}

MainWindow::~MainWindow()
{

}

void MainWindow::read()
{
#warning not released
}

void MainWindow::write()
{
#warning not released
}

void MainWindow::radioConnect(const QString &name, const QString &version)
{
  m_name->setText(name);
  m_version->setText(version);
}
