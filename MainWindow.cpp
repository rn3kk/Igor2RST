#include "MainWindow.h"
#include "MemoryDelegate.h"
#include "MemoryTableModel.h"
#include "Tab2Tone.h"
#include "Tab5Tone.h"

#include <QHeaderView>
#include <QStackedLayout>
#include <QTabBar>
#include <QTableView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  QWidget *central = new QWidget(this);
  setCentralWidget(central);
  QVBoxLayout *vl = new QVBoxLayout(central);
  vl->setSpacing(0);
  vl->setContentsMargins(0, 0, 0, 0);
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
