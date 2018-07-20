#ifndef MEMORYDELEGATE_H
#define MEMORYDELEGATE_H

#include <QItemDelegate>

class MemoryDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  MemoryDelegate();

  // QAbstractItemDelegate interface
public:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
  QBrush m_header;  
};

#endif // MEMORYDELEGATE_H
