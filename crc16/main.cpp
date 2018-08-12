#include <QCoreApplication>
#include <QtEndian>
#include <QDebug>
#include <QDataStream>

int bits(unsigned __int16 w)
{
  int bits;
  for(bits = 0; w != 0; ++bits)
  {
    w >>= 1;
  }
  return bits;
}

void crc(unsigned __int16* arr)
{
  unsigned __int16 w0, w1, w2, w3, w4;
  w0= 0xffff;
  for(int i=0; i<14; ++i) //i-> w4
  {
    w1 = arr[i];
    w2 = 0x00ff;

    w1 = w1 & w2;
    w0 = w0 ^ w1;

    bool b(false);
    for(int j = 9; j!=0; --j )//j->w3
    {
      b = true;
       unsigned __int16 c = (w0 >> 0) & 1U;
       int bitSize=bits(w0);
       w0 = w0 >> 1;
       if(c) //в старший бит w0 пишем с
         w0 |= 1 << (bitSize - 1);
       else
         w0 &= ~(1 << (bitSize - 1));

       c = (w0 >> 0) & 1U; //lowBit to c
       if(c==1)
       {
         w2 = 0x8005;
         w0 = w0 ^ w2;
       }
    }

    if(b)break;
  }
   w0 = ((w0 << 8) & 0xff00) | ((w0 >> 8) & 0x00ff);
}

quint16 crc(const QByteArray &data, int numByte)
{
  quint16 w0 = 0xffff, w1, w2, w3, w4 = 0;
  do
  {
    w1 = data[w4++];
    w2 = 0x00ff;
    w1 &= w2;
    w0 ^= w1;
    w3 = 0x0009;
    while(--w3)
    {
      bool c = w0 & 0x0001;
      w0 = (w0 >> 1) & 0x7fff;
      if(c)
      {
        w2 = 0x8005;
        w0 ^= w2;
      }
    }
  }
  while(--numByte);
  w0 = ((w0 << 8) & 0xff00) | ((w0 >> 8) & 0x00ff);
  return w0;
}

//"21090c090014ffffffffffffffffffffffff2020202020202000a8d7"
//05.08.2018 00:12:59.359 [DEBUG  ] packet crc  "ffffffffffffa8d7"  calculate crc  "ffffffffffffa8d7"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QByteArray data = QByteArray::fromHex("2111000a41522d343376322e3035ede2");
  quint16 result = crc(data, 14);


/*  const char dataPtr[] = {
      '\x21', '\x11', '\x00', '\x0a', '\x41', '\x52',
                       '\x2d', '\x34', '\x33', '\x76', '\x32', '\x2e',
                       '\x30', '\x35', '\xed', '\xe2'
  //    '\x21', '\x09', '\x01', '\xBF', '\x00', '\x14', '\x00', '\x24', '\x20',
//      '\xA8', '\x20', '\xA8', '\x02', '\xED', '\x02', '\xED', '\x00', '\x00',
  //    '\x53', '\x4B', '\x4F', '\x52', '\x41', '\x4A', '\x41', '\x00', '\x9A', '\xB0'
    };
   data = QByteArray(dataPtr, 16);*/
   qint16 crcC = crc(data, 16);


//  QByteArray ar= QByteArrayLiteral("\x21\x11\x00\x0a\x041\x52\x2d\x34\x33\x76\x32\x2e\x30\x35");
  quint16* arr = new quint16[14];
  arr[0] = 0x21;
  arr[1] = 0x11;
  arr[2] = 0x00;
  arr[3] = 0x0a;
  arr[4] = 0x41;
  arr[5] = 0x52;
  arr[6] = 0x2d;
  arr[7] = 0x34;
  arr[8] = 0x33;
  arr[9] = 0x76;
  arr[10] = 0x32;
  arr[11] = 0x2e;
  arr[12] = 0x30;
  arr[13] = 0x35;

  QDataStream stream;
  for(int i=0; i<14; ++i)
  {
    qDebug() << "i:" << i << " " << QString::number(arr[i], 16);
  }

 // crc(arr);

  int count = 14;
  quint16 w0,w1,w2,w3,w4;

  w0= 0xffff;
  w4= 0;

CRC01:
  w1=arr[w4];
  w4 = w4+1;
  goto CRC_BYTE;

CRC_BYTE_RETURN:
  --count;
  if(count != 0)
  {
    goto CRC01;
  }
  //поменять местами младший и старшй байт
  qint16 z =  qFromBigEndian(w0);
  qint16 z2 =  ((w0 << 8) & 0xff00) | ((w0 >> 8) & 0x00ff);
  return a.exec();

CRC_BYTE:
  w2 = 0x00FF;
  w1 = w1 & w2;
  w0 = w0 ^ w1;
  w3 = 0x0009;

CRC00:
  w3 = w3-1;
  if(w3 == 0)
    goto CRC_BYTE_RETURN;

  bool c = (w0 >> 0) & 1U;
  int bitSize=bits(w0);
  w0 = w0 >> 1; //shift
  w0 = (w0 >> 1) & 0x7fff;
//  if(c) //в старший бит w0 пишем с
//    w0 |= 1 << (bitSize - 1);
//  else
//    w0 &= ~(1 << (bitSize - 1));

  if(c!=1)
    goto CRC00;

  w2 = 0x8005;
  w0 = w0 ^ w2;
  goto CRC00;

END:
  qDebug ()<< "END";




}
