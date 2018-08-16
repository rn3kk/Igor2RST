#ifndef MEMORYTABLEMODEL_H
#define MEMORYTABLEMODEL_H

#include <QAbstractTableModel>
class Tab5Tone;

class MemoryTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  MemoryTableModel(Tab5Tone &tab5);
  ~MemoryTableModel();

  void read(const QByteArrayList &data, bool import);
  QByteArrayList toWrite(bool toFile) const;

  QStringList variants(int column, int row);

  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  struct MemoryItem *m_memory;
  Tab5Tone &m_5tone;

  QByteArray memoryItem(int number) const;
  void setMemoryItem(const QByteArray &data, int number, bool import);
};

#endif // MEMORYTABLEMODEL_H
