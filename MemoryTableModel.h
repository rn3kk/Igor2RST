#ifndef MEMORYTABLEMODEL_H
#define MEMORYTABLEMODEL_H

#include <QAbstractTableModel>

class MemoryTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  MemoryTableModel();
  ~MemoryTableModel();

  QByteArray memoryItem(int number);

  static QStringList variants(int column);

  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  struct MemoryItem *m_memory;
};

#endif // MEMORYTABLEMODEL_H
