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

void crc(int numByte, quint16* arr)
{
  qDebug() << "Packet";
  for(int i=0; i< numByte; ++i)
  {
    qDebug() << "index=" << i<< " "<< QString::number(arr[i],16);
  }
  quint16 w0, w1, w2, w3, w4;
  w0= 0xffff;
  for(int i=0; i<numByte; ++i) //i-> w4
  {
    //qDebug() << "W0=" << QString::number(w0, 16) << " Istep:="<< QString::number(i);
    w1 = arr[i];
    w2 = 0x00ff;

    w1 = w1 & w2;
    w0 = w0 ^ w1;

    for(int j = 1; j<9; ++j )//j->w3
    {
      //qDebug() << "      W0=" << QString::number(w0, 16) << " Jstep:="<< QString::number(j);
      quint16 c = (w0 >> 0) & 1U; //lowBit to c
      w0 = w0 >> 1;
      w0 = w0 & (~(1 << 15));
      if(c==1)
      {
        w2 = 0x8005;
        w0 = w0 ^ w2;
      }
    }
  }
  w0 = ((w0 << 8) & 0xff00) | ((w0 >> 8) & 0x00ff);
  qDebug() << "CRC=" << QString::number(w0, 16);
}

quint16 crc(const QByteArray &data, int numByte)
{
  quint16 w0 = 0xffff, w1, w2, w3, w4 = 0;
  do
  {
    qDebug() << "AAA W0=" << QString::number(w0, 16) << " Istep:="<< QString::number(w4);
    w1 = data[w4++];
    w2 = 0x00ff;
    w1 &= w2;
    w0 ^= w1;
    w3 = 0x0009;
    while(--w3)
    {
      //qDebug() << "      W0=" << QString::number(w0, 16) << " step:="<< QString::number(w3);
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
  qDebug() << "AAAbefore W0=" << QString::number(w0, 16);
  w0 = ((w0 << 8) & 0xff00) | ((w0 >> 8) & 0x00ff);
  qDebug() << "AAAafter W0=" << QString::number(w0, 16);
  return w0;
}

//210c00000005000a000103a7032602a402210981
//У меня CRC получается 0x5971 (ваша 0x8109)
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  int count = 6;
  quint16* arr = new quint16[count];
  arr[0] = 0x21;
  arr[1] = 0x0B;
  arr[2] = 0x00;
  arr[3] = 0x00;
  arr[4] = 0x00;
  arr[5] = 0x0A;
 // crc(count, arr);

  QByteArray array;
  for(int i=0; i<count; ++i)
  {
    array.append(qint16(arr[i]));
  }
  //crc(array, count);


  int count2=18;
  quint16* arr2 = new quint16[count2];
  arr2[0] = 0x21;
  arr2[1] = 0x0c;
  arr2[2] = 0x00;
  arr2[3] = 0x00;
  arr2[4] = 0x00;
  arr2[5] = 0x05;
  arr2[6] = 0x00;
  arr2[7] = 0x0a;
  arr2[8] = 0x00;
  arr2[9] = 0x01;
  arr2[10] = 0x03;
  arr2[11] = 0xa7;
  arr2[12] = 0x03;
  arr2[13] = 0x26;
  arr2[14] = 0x02;
  arr2[15] = 0xa4;
  arr2[16] = 0x02;
  arr2[17] = 0x21;

  crc(count2, arr2);
  array.clear();
  for(int i=0; i< count2; ++i)
  {
    array.append(qint16(arr2[i]));
  }
  crc(array, count2);

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
  w0 = w0 >> 1; //shift

  if(c!=1)
    goto CRC00;

  w2 = 0x8005;
  w0 = w0 ^ w2;
  qDebug() <<" CRC00 end. w0=" << QString::number(w0, 16);
  goto CRC00;

END:
  qDebug ()<< "END";




}
