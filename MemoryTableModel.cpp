#include "MemoryTableModel.h"
#include "Tab5Tone.h"

#include <QDebug>

struct MemoryItem
{
  qint16 decode = 0x0000;
  qint16 encode = 0x0000;
  bool rxFrStep625;
  double rxFrequence = 33.0;
  bool txFrStep625;
  double txFrequence = 33.0;
  qint8 compander = 0;
  qint8 scrambler = 0;
  qint8 scan = 0;
  qint8 del = 0;
  qint8 lock = 0;
  qint8 bandwidth = 0;
  qint8 powerOut = 0;
  qint8 toneMode = 0;
  qint8 call;
  bool begin = false;
  bool end = false;
  QString chName;
};

static const int ITEM_COUNT = 208;

static const QMap<QString, qint8> bandwidthMap = {{"W", 0}, {"N", 0x16}};
static const QMap<QString, qint8> powerOutMap = {{"H", 2}, {"M", 1}, {"L", 0}};
static const QMap<QString, qint16>  decodeMap =
{
  {" OFF", 0x0000}, {"01-67.0", 0x02C9}, {"02-71.9", 0x02CA}, {"03-74.4", 0x02CB},
  {"04-77.0", 0x02CC}, {"05-79.7", 0x02CD}, {"06-82.5", 0x02CE}, {"07-85.4", 0x02CF},
  {"08-88.5", 0x02D0}, {"09-91.5", 0x02D1}, {"10-94.8", 0x02D2}, {"11-97.4", 0x02D3},
  {"12-100.0", 0x02D4}, {"13-103.5", 0x02D5}, {"14-107.2", 0x02D6},
  {"15-110.9", 0x02D7}, {"16-114.8", 0x02D8}, {"17-118.8", 0x02D9},
  {"18-123.0", 0x02DA}, {"19-127.3", 0x02DB}, {"20-131.8", 0x02DC},
  {"21-136.5", 0x02DD}, {"22-141.3", 0x02DE}, {"23-146.2", 0x02DF},
  {"24-151.4", 0x02E0}, {"25-156.7", 0x02E1}, {"26-162.2", 0x02E2},
  {"27-167.9", 0x02E3}, {"28-173.8", 0x02E4}, {"29-179.9", 0x02E5},
  {"30-186.2", 0x02E6}, {"31-192.8", 0x02E7}, {"32-203.5", 0x02E8},
  {"33-210.7", 0x02E9}, {"34-218.1", 0x02EA}, {"35-225.7", 0x02EB},
  {"36-233.6", 0x02EC}, {"37-241.8", 0x02ED}, {"38-250.3", 0x02EE},
  {"39-69.3", 0x02EF}, {"40-62.5", 0x02F0}, {"41-159.8", 0x02F1},
  {"42-165.5", 0x02F2}, {"43-171.3", 0x02F3}, {"44-177.3", 0x02F4},
  {"45-183.5", 0x02F5}, {"46-189.9", 0x02F6}, {"47-196.6", 0x02F7},
  {"48-199.5", 0x02F8}, {"49-206.5", 0x02F9}, {"50-229.1", 0x02FA},
  {"51-254.1", 0x02FB}, {"D023N", 0x0301}, {"D025N", 0x0302}, {"D026N", 0x0303},
  {"D031N", 0x0304}, {"D032N", 0x0305}, {"D043N", 0x0306}, {"D047N", 0x0307},
  {"D051N", 0x0308}, {"D054N", 0x0309}, {"D065N", 0x030a}, {"D071N", 0x030b},
  {"D072N", 0x030c}, {"D073N", 0x030d}, {"D074N", 0x030e}, {"D114N", 0x030f},
  {"D115N", 0x0310}, {"D116N", 0x0311}, {"D125N", 0x0312}, {"D131N", 0x0313},
  {"D132N", 0x0314}, {"D134N", 0x0315}, {"D143N", 0x0316}, {"D152N", 0x0317},
  {"D155N", 0x0318}, {"D156N", 0x0319}, {"D162N", 0x031a}, {"D165N", 0x031b},
  {"D172N", 0x031c}, {"D174N", 0x031d}, {"D205N", 0x031e}, {"D223N", 0x031f},
  {"D226N", 0x0320}, {"D243N", 0x0321}, {"D244N", 0x0322}, {"D245N", 0x0323},
  {"D251N", 0x0324}, {"D261N", 0x0325}, {"D263N", 0x0326}, {"D265N", 0x0327},
  {"D271N", 0x0328}, {"D306N", 0x0329}, {"D311N", 0x032a}, {"D315N", 0x032b},
  {"D331N", 0x032c}, {"D343N", 0x032d}, {"D346N", 0x032e}, {"D351N", 0x032f},
  {"D364N", 0x0330}, {"D365N", 0x0331}, {"D371N", 0x0332}, {"D411N", 0x0333},
  {"D412N", 0x0334}, {"D413N", 0x0335}, {"D423N", 0x0336}, {"D431N", 0x0337},
  {"D432N", 0x0338}, {"D445N", 0x0339}, {"D464N", 0x033a}, {"D465N", 0x033b},
  {"D466N", 0x033c}, {"D503N", 0x033d}, {"D506N", 0x033e}, {"D516N", 0x033f},
  {"D532N", 0x0340}, {"D546N", 0x0341}, {"D565N", 0x0342}, {"D606N", 0x0343},
  {"D612N", 0x0344}, {"D624N", 0x0345}, {"D627N", 0x0346}, {"D631N", 0x0347},
  {"D632N", 0x0348}, {"D654N", 0x0349}, {"D662N", 0x034a}, {"D664N", 0x034b},
  {"D703N", 0x034c}, {"D712N", 0x034d}, {"D723N", 0x034e}, {"D731N", 0x034f},
  {"D732N", 0x0350}, {"D734N", 0x0351}, {"D743N", 0x0352}, {"D754N", 0x0353},
  {"D023I", 0x0365}, {"D025I", 0x0366}, {"D026I", 0x0367}, {"D031I", 0x0368},
  {"D032I", 0x0369}, {"D043I", 0x036a}, {"D047I", 0x036b}, {"D051I", 0x036c},
  {"D054I", 0x036d}, {"D065I", 0x036e}, {"D071I", 0x036f}, {"D072I", 0x0370},
  {"D073I", 0x0371}, {"D074I", 0x0372}, {"D114I", 0x0373}, {"D115I", 0x0374},
  {"D116I", 0x0375}, {"D125I", 0x0376}, {"D131I", 0x0377}, {"D132I", 0x0378},
  {"D134I", 0x0379}, {"D143I", 0x037a}, {"D152I", 0x037b}, {"D155I", 0x037c},
  {"D156I", 0x037d}, {"D162I", 0x037e}, {"D165I", 0x037f}, {"D172I", 0x0380},
  {"D174I", 0x0381}, {"D205I", 0x0382}, {"D223I", 0x0383}, {"D226I", 0x0384},
  {"D243I", 0x0385}, {"D244I", 0x0386}, {"D245I", 0x0387}, {"D251I", 0x0388},
  {"D261I", 0x0389}, {"D263I", 0x038a}, {"D265I", 0x038b}, {"D271I", 0x038c},
  {"D306I", 0x038d}, {"D311I", 0x038e}, {"D315I", 0x038f}, {"D331I", 0x0390},
  {"D343I", 0x0391}, {"D346I", 0x0392}, {"D351I", 0x0393}, {"D364I", 0x0394},
  {"D365I", 0x0395}, {"D371I", 0x0396}, {"D411I", 0x0397}, {"D412I", 0x0398},
  {"D413I", 0x0399}, {"D423I", 0x039a}, {"D431I", 0x039b}, {"D432I", 0x039c},
  {"D445I", 0x039d}, {"D464I", 0x039e}, {"D465I", 0x039f}, {"D466I", 0x03a0},
  {"D503I", 0x03a1}, {"D506I", 0x03a2}, {"D516I", 0x03a3}, {"D532I", 0x03a4},
  {"D546I", 0x03a5}, {"D565I", 0x03a6}, {"D606I", 0x03a7}, {"D612I", 0x03a8},
  {"D624I", 0x03a9}, {"D627I", 0x03aa}, {"D631I", 0x03ab}, {"D632I", 0x03ac},
  {"D654I", 0x03ad}, {"D662I", 0x03ae}, {"D664I", 0x03af}, {"D703I", 0x03b0},
  {"D712I", 0x03b1}, {"D723I", 0x03b2}, {"D731I", 0x03b3}, {"D732I", 0x03b4},
  {"D734I", 0x03b5}, {"D743I", 0x03b6}, {"D754I", 0x03b7}
};
static const QMap<QString, qint16>  encodeMap =
{
  {"OFF", 0x0000}, {"1-67.0", 0x01C9}, {"2-71.9", 0x01CA}, {"3-74.4", 0x01CB},
  {"4-77.0", 0x01CC}, {"5-79.7", 0x01CD}, {"6-82.5", 0x01CE}, {"7-85.4", 0x01CF},
  {"8-88.5", 0x01D0}, {"9-91.5", 0x01D1}, {"10-94.8", 0x01D2}, {"11-97.4", 0x01D3},
  {"12-100.0", 0x01D4}, {"13-103.5", 0x01D5}, {"14-107.2", 0x01D6},
  {"15-110.9", 0x01D7}, {"16-114.8", 0x01D8}, {"17-118.8", 0x01D9},
  {"18-123.0", 0x01DA}, {"19-127.3", 0x01DB}, {"20-131.8", 0x01DC},
  {"21-136.5", 0x01DD}, {"22-141.3", 0x01DE}, {"23-146.2", 0x01DF},
  {"24-151.4", 0x01E0}, {"25-156.7", 0x01E1}, {"26-162.2", 0x01E2},
  {"27-167.9", 0x01E3}, {"28-173.8", 0x01E4}, {"29-179.9", 0x01E5},
  {"30-186.2", 0x01E6}, {"31-192.8", 0x01E7}, {"32-203.5", 0x01E8},
  {"33-210.7", 0x01E9}, {"34-218.1", 0x01EA}, {"35-225.7", 0x01EB},
  {"36-233.6", 0x01EC}, {"37-241.8", 0x01ED}, {"38-250.3", 0x01EE},
  {"39-69.3", 0x01EF}, {"40-62.5", 0x01F0}, {"41-159.8", 0x01F1},
  {"42-165.5", 0x01F2}, {"43-171.3", 0x01F3}, {"44-177.3", 0x01F4},
  {"45-183.5", 0x01F5}, {"46-189.9", 0x01F6}, {"47-196.6", 0x01F7},
  {"48-199.5", 0x01F8}, {"49-206.5", 0x01F9}, {"50-229.1", 0x01FA},
  {"51-254.1", 0x01FB}, {"D023N", 0x0301}, {"D025N", 0x0302}, {"D026N", 0x0303},
  {"D031N", 0x0304}, {"D032N", 0x0305}, {"D043N", 0x0306}, {"D047N", 0x0307},
  {"D051N", 0x0308}, {"D054N", 0x0309}, {"D065N", 0x030a}, {"D071N", 0x030b},
  {"D072N", 0x030c}, {"D073N", 0x030d}, {"D074N", 0x030e}, {"D114N", 0x030f},
  {"D115N", 0x0310}, {"D116N", 0x0311}, {"D125N", 0x0312}, {"D131N", 0x0313},
  {"D132N", 0x0314}, {"D134N", 0x0315}, {"D143N", 0x0316}, {"D152N", 0x0317},
  {"D155N", 0x0318}, {"D156N", 0x0319}, {"D162N", 0x031a}, {"D165N", 0x031b},
  {"D172N", 0x031c}, {"D174N", 0x031d}, {"D205N", 0x031e}, {"D223N", 0x031f},
  {"D226N", 0x0320}, {"D243N", 0x0321}, {"D244N", 0x0322}, {"D245N", 0x0323},
  {"D251N", 0x0324}, {"D261N", 0x0325}, {"D263N", 0x0326}, {"D265N", 0x0327},
  {"D271N", 0x0328}, {"D306N", 0x0329}, {"D311N", 0x032a}, {"D315N", 0x032b},
  {"D331N", 0x032c}, {"D343N", 0x032d}, {"D346N", 0x032e}, {"D351N", 0x032f},
  {"D364N", 0x0330}, {"D365N", 0x0331}, {"D371N", 0x0332}, {"D411N", 0x0333},
  {"D412N", 0x0334}, {"D413N", 0x0335}, {"D423N", 0x0336}, {"D431N", 0x0337},
  {"D432N", 0x0338}, {"D445N", 0x0339}, {"D464N", 0x033a}, {"D465N", 0x033b},
  {"D466N", 0x033c}, {"D503N", 0x033d}, {"D506N", 0x033e}, {"D516N", 0x033f},
  {"D532N", 0x0340}, {"D546N", 0x0341}, {"D565N", 0x0342}, {"D606N", 0x0343},
  {"D612N", 0x0344}, {"D624N", 0x0345}, {"D627N", 0x0346}, {"D631N", 0x0347},
  {"D632N", 0x0348}, {"D654N", 0x0349}, {"D662N", 0x034a}, {"D664N", 0x034b},
  {"D703N", 0x034c}, {"D712N", 0x034d}, {"D723N", 0x034e}, {"D731N", 0x034f},
  {"D732N", 0x0350}, {"D734N", 0x0351}, {"D743N", 0x0352}, {"D754N", 0x0353},
  {"D023I", 0x0365}, {"D025I", 0x0366}, {"D026I", 0x0367}, {"D031I", 0x0368},
  {"D032I", 0x0369}, {"D043I", 0x036a}, {"D047I", 0x036b}, {"D051I", 0x036c},
  {"D054I", 0x036d}, {"D065I", 0x036e}, {"D071I", 0x036f}, {"D072I", 0x0370},
  {"D073I", 0x0371}, {"D074I", 0x0372}, {"D114I", 0x0373}, {"D115I", 0x0374},
  {"D116I", 0x0375}, {"D125I", 0x0376}, {"D131I", 0x0377}, {"D132I", 0x0378},
  {"D134I", 0x0379}, {"D143I", 0x037a}, {"D152I", 0x037b}, {"D155I", 0x037c},
  {"D156I", 0x037d}, {"D162I", 0x037e}, {"D165I", 0x037f}, {"D172I", 0x0380},
  {"D174I", 0x0381}, {"D205I", 0x0382}, {"D223I", 0x0383}, {"D226I", 0x0384},
  {"D243I", 0x0385}, {"D244I", 0x0386}, {"D245I", 0x0387}, {"D251I", 0x0388},
  {"D261I", 0x0389}, {"D263I", 0x038a}, {"D265I", 0x038b}, {"D271I", 0x038c},
  {"D306I", 0x038d}, {"D311I", 0x038e}, {"D315I", 0x038f}, {"D331I", 0x0390},
  {"D343I", 0x0391}, {"D346I", 0x0392}, {"D351I", 0x0393}, {"D364I", 0x0394},
  {"D365I", 0x0395}, {"D371I", 0x0396}, {"D411I", 0x0397}, {"D412I", 0x0398},
  {"D413I", 0x0399}, {"D423I", 0x039a}, {"D431I", 0x039b}, {"D432I", 0x039c},
  {"D445I", 0x039d}, {"D464I", 0x039e}, {"D465I", 0x039f}, {"D466I", 0x03a0},
  {"D503I", 0x03a1}, {"D506I", 0x03a2}, {"D516I", 0x03a3}, {"D532I", 0x03a4},
  {"D546I", 0x03a5}, {"D565I", 0x03a6}, {"D606I", 0x03a7}, {"D612I", 0x03a8},
  {"D624I", 0x03a9}, {"D627I", 0x03aa}, {"D631I", 0x03ab}, {"D632I", 0x03ac},
  {"D654I", 0x03ad}, {"D662I", 0x03ae}, {"D664I", 0x03af}, {"D703I", 0x03b0},
  {"D712I", 0x03b1}, {"D723I", 0x03b2}, {"D731I", 0x03b3}, {"D732I", 0x03b4},
  {"D734I", 0x03b5}, {"D743I", 0x03b6}, {"D754I", 0x03b7}
};
static const QMap<QString, qint8> toneModeMap = {{"OFF", 0}, {"2TONE", 0x01},
                                                 {"5TONE", 0x02}};
const QMap<QString, char> codeMap = {
  {"AB", 0}, {"AC", 1}, {"AD", 2}, {"BA", 3}, {"BC", 4},
  {"BD", 5}, {"CA", 6}, {"CB", 7}, {"CD", 8}, {"DA", 9},
  {"DB", 10}, {"DC", 11}, {"AA", 12}, {"BB", 13},
  {"CC", 14}, {"DD", 15}
};

MemoryTableModel::MemoryTableModel(Tab5Tone &tab5) :
  QAbstractTableModel(),
  m_memory(new MemoryItem[ITEM_COUNT]),
  m_5tone(tab5)
{}

MemoryTableModel::~MemoryTableModel()
{
  delete[] m_memory;
}

QStringList MemoryTableModel::variants(int column, int row)
{
  switch(column)
  {
  case 3: return bandwidthMap.keys();
  case 5: return powerOutMap.keys();
  case 6: return decodeMap.keys();
  case 7: return encodeMap.keys();
  case 8: return toneModeMap.keys();
  case 11:
    switch(m_memory[row].toneMode)
    {
    case 1: return codeMap.keys();
    case 2: return m_5tone.numbers();
    }
  default: return QStringList();
  }
}

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
    case 0: return tr("Ch num");
    case 1: return tr("RX freq         ");
    case 2: return tr("TX freq         ");
    case 3: return tr("W/N");
    case 4: return tr("CH. Name     ");
    case 5: return tr("Pout");
    case 6: return tr("QT/DQT Dec");
    case 7: return tr("QT/DQT Enc");
    case 8: return tr("Tone Mode");
    case 9: return tr("Begin");
    case 10: return tr("End");
    case 11: return tr("Call        ");
    case 12: return tr("Compander");
    case 13: return tr("Scrambler");
    case 14: return tr("Scan");
    case 15: return tr("Del ch");
    case 16: return tr("Lock");
    }
  }
  if(column == 0)
  {
    if(row < 200)
      return QString::number(row);
    return "C" + QString::number(row - 199);
  }
  if(role == Qt::DisplayRole && m_memory[row - 1].chName.isEmpty())
    return QVariant();
  if(role == Qt::EditRole && m_memory[row - 1].chName.isEmpty())
    m_memory[row - 1].chName = tr("CHNAME");
  switch(column)
  {
  case 1:
    if(role == Qt::EditRole)
      return m_memory[row - 1].rxFrequence;
    return QString::number(m_memory[row - 1].rxFrequence, 'f', 5) + tr("MHz");
  case 2:
    if(role == Qt::EditRole)
      return m_memory[row - 1].txFrequence;
    return QString::number(m_memory[row - 1].txFrequence, 'f', 5) + tr("MHz");
  case 3: return bandwidthMap.key(m_memory[row - 1].bandwidth);
  case 4:
    return m_memory[row - 1].chName;
  case 5: return powerOutMap.key(m_memory[row - 1].powerOut);
  case 6: return decodeMap.key(m_memory[row - 1].decode);
  case 7: return encodeMap.key(m_memory[row - 1].encode);
  case 8: return toneModeMap.key(m_memory[row - 1].toneMode);
  case 9: return m_memory[row - 1].begin;
  case 10: return m_memory[row - 1].end;
  case 11:
    switch(m_memory[row - 1].toneMode)
    {
    case 1: return codeMap.key(m_memory[row - 1].call);
    case 2: return QString::number(m_memory[row - 1].call);
    default: return QVariant();
    }
  case 12: return m_memory[row - 1].compander;
  case 13: return m_memory[row - 1].scrambler;
  case 14: return m_memory[row - 1].scan;
  case 15: return m_memory[row - 1].del;
  case 16: return m_memory[row - 1].lock;
  }
  return QVariant();
}

void calculateFR(double value, bool rx, MemoryItem &memory)
{
  value *= 1000;
  double n5 = value / 5;
  n5 = round(n5);
  n5 = 5 * n5;
  double n625 = value / 6.25;
  n625 = round(n625);
  n625 = 6.25 * n625;
  double k = pow((value - n5) / (value - n625), 2);
  n5 /= 1000;
  n625 /= 1000;
  if(n5 < 33.0)
    n5 = 33.0;
  if(n625 < 33.0)
    n625 = 33.0;
  if(n5 > 48.5)
    n5 = 48.5;
  if(n625 > 48.5)
    n625 = 48.5;
  if(rx)
  {
    if(k > 1)
    {
      memory.rxFrequence = n625;
      memory.rxFrStep625 = true;
    }
    else
    {
      memory.rxFrequence = n5;
      memory.rxFrStep625 = false;
    }
  }
  else
  {
    if(k > 1)
    {
      memory.txFrequence = n625;
      memory.txFrStep625 = true;
    }
    else
    {
      memory.txFrequence = n5;
      memory.txFrStep625 = false;
    }
  }
}

bool MemoryTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int column = index.column();
  if(row == 0 || column == 0)
    return false;

  QModelIndex start = MemoryTableModel::index(row, 7),
      end = MemoryTableModel::index(row + 1, 16);
  row--;
  switch(column)
  {
  case 1:
    calculateFR(value.toDouble(), true, m_memory[row]);
    return true;
  case 2:
    calculateFR(value.toDouble(), false, m_memory[row]);
  case 3:
    m_memory[row].bandwidth = bandwidthMap[value.toString()];
    emit layoutChanged();
    return true;
  case 4:
    m_memory[row].chName = value.toString();
    emit layoutChanged();
    return true;
  case 5:
    m_memory[row].powerOut = powerOutMap[value.toString()];
    emit layoutChanged();
    return true;
  case 6:
    m_memory[row].decode = decodeMap[value.toString()];
    emit layoutChanged();
    return true;
  case 7:
    m_memory[row].encode = encodeMap[value.toString()];
    emit layoutChanged();
    return true;
  case 8:
    m_memory[row].toneMode = toneModeMap[value.toString()];
    emit dataChanged(index, index);
    return true;
  case 9:
    m_memory[row].begin = value.toBool() ? 0x04 : 0;
    emit layoutChanged();
    return true;
  case 10: m_memory[row].end = value.toBool() ? 0x08 : 0;
  case 11:
    switch(m_memory[row].toneMode)
    {
    case 1:
      m_memory[row].call = codeMap[value.toString()];
      emit dataChanged(index, index);
      return true;
    case 2:
      m_memory[row].call = value.toInt();
      emit dataChanged(index, index);
      return true;
    }
  case 12:
    m_memory[row].compander = value.toBool() ? 0x04 : 0;
    emit layoutChanged();
    return true;
  case 13:
    m_memory[row].scrambler = value.toBool() ? 0x08 : 0;
    emit layoutChanged();
    return true;
  case 14:
    m_memory[row].scan = value.toBool() ? 0x20 : 0;
    emit layoutChanged();
    return true;
  case 15:
    m_memory[row].del = value.toBool() ? 0x80 : 0;
    emit dataChanged(start, end);
    return true;
  case 16:
    m_memory[row].lock = value.toBool() ? 0x40 : 0;
    emit dataChanged(start, end);
    return true;
  }
  emit layoutChanged();
  return false;
}

Qt::ItemFlags MemoryTableModel::flags(const QModelIndex &index) const
{
  if(!index.isValid())
    return Qt::NoItemFlags;
  int column = index.column();
  int row = index.row();
  if(row == 0 || column == 0)
  {
    return Qt::ItemIsEnabled;
  }  
  row--;
  if(column <= 11 && column > 8 && m_memory[row].toneMode == 0)
  {
    return Qt::ItemIsAutoTristate;
  }
//  if(column != 4 && m_memory[row].chName.isEmpty())
//  {
//    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//  }
  return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

