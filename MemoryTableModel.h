#ifndef MEMORYTABLEMODEL_H
#define MEMORYTABLEMODEL_H

#include <QAbstractTableModel>

const int ITEM_COUNT = 208;

class MemoryTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  MemoryTableModel();

  QByteArray memoryItem(int number);

  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  struct MemoryItem
  {
    bool compander;
    bool scrambler;
    bool scan;
    bool del;
    bool lock;
    bool bandwidth;
    double rxFrequence;
    double txFrequence;
    QString chName;
    QString powerOut;
    QString decode;
    QString encode;
    QString toneMode;
    QString begin;
    QString end;
    QString call;
  } m_memory[ITEM_COUNT];
};

#endif // MEMORYTABLEMODEL_H
