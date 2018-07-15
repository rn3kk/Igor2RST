#include "MemoryTableModel.h"

MemoryTableModel::MemoryTableModel(): QAbstractTableModel() {}

int MemoryTableModel::rowCount(const QModelIndex &) const
{
  return ITEM_COUNT + 1;
}

int MemoryTableModel::columnCount(const QModelIndex &) const
{
  return 17;
}

QVariant MemoryTableModel::data(const QModelIndex &index, int role) const
{
  if(role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();
  int row = index.row();
  int column = index.column();
  if(row == 0)
  {
    switch(column)
    {
    case 0: return "Ch num";
    case 1: return "RX freq";
    case 2: return "TX freq";
    case 3: return "W/N";
    case 4: return "CH. Name";
    case 5: return "Pout";
    case 6: return "QT/DQT Dec";
    case 7: return "QT/DQT Enc";
    case 8: return "Tone Mode";
    case 9: return "Begin";
    case 10: return "End";
    case 11: return "Call";
    case 12: return "Compander";
    case 13: return "Scrambler";
    case 14: return "Scan";
    case 15: return "Del ch";
    case 16: return "Lock";
    }
  }
  if(column == 0)
  {
    if(row < 200)
      return QString::number(row);
    return "C" + QString::number(row - 199);
  }
#warning not released
  return QVariant();
}

bool MemoryTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
#warning not released
  return false;
}

Qt::ItemFlags MemoryTableModel::flags(const QModelIndex &index) const
{
  if(!index.isValid())
    return Qt::NoItemFlags;
  int column = index.column();
  if(index.row() == 0 || column == 0)
  {
    return Qt::ItemIsEnabled;
  }
  if(column < 11 && column > 8 && m_memory[index.row()].toneMode == "OFF")
  {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

