#ifndef _FILEHEAD_H
#define _FILEHEAD_H
#define OTS_DEV_OSO_N           0x0001
#define OTS_FLAG                "OTS>"
#define OTS_DATATYPE_Int32      0x1
#define OTS_DATATYPE_Int24      0x2
#define OTS_DATATYPE_Int16      0x3
#define OTS_DATATYPE_IEEEFloat  0x4
#define OTS_DATATYPE_IBMFloat   0x5
#define OTS_DATATYPE_Mixed      0x0

#define OTS_ADCUNIT_Volt        0x1
#define OTS_ADCUNIT_mVolt       0x2
#define OTS_ADCUNIT_Amp         0x3
#define OTS_ADCUNIT_mAmp        0x4
#define OTS_ADCUNIT_nTesla      0x5
#define OTS_ADCUNIT_pTesla      0x6
#define OTS_ADCUNIT_Unknown     0x0

#define OTS_MAX_CHAN            10 // 最多10通道
//#pragma pack(1)
typedef struct OTS_CHAN{//桔灯时间序列通道信息
  unsigned int   iGain;     // 增益倍数，如无增益，设置为 0x1
  int            iADShift;  // AD 偏置，如无偏置，设置为 0
  int            iADMax;    // AD 最大值，即正满幅AD值，比如 24 位AD为 2^23
  int            iADMin;    // AD 最小值，即负满幅AD值
  int            iAD_para;  // AD设置，复制 para_AD（可选）
  int            iAD_unused;// AD设置扩展，未使用
  int            iADC;      // AD转换系数，OSO-N 为 0x5D1420;
  unsigned short iADCUnit;  // 转换单位，OSO-N 为 OTS_ADCUNIT_Volt
  float          iRefVolot; // 参考电压（可选）
}__attribute__ ((packed,aligned(1)))CHAN;

typedef struct OTS_FileHead{// 桔灯时间序列文件头信息
  char            Flag[4];   // 标志，必须为 OTS_FLAG
  unsigned int    iOTSVer;   // OTS 文件格式版本。当前为 0x1，如读取程序发现版本高于自己所支持的，应给与提示
  unsigned short  nChan;     // 通道个数
  unsigned short  nFileOrder;// 文件顺序（本次采集开始记为1，每存满一个文件，开新文件则+1。停止采集重新记为1）
  short           iChOrder;  // 通道顺序；1 -- 正序；0 -- 倒序
  int             iChanMask; // 通道掩码。比如开启了1,2,3,5通道，则为 0x00000017  
  char            Proj[32];  // 工程名，可选留空
  unsigned int    iFileSize; // 全部文件大小，单位 Byte；
  unsigned short  iHeadSize; // 文件头大小，单位 Byte ；
  unsigned short  iDevID;    // 设备类型ID； OSO-N 为 OTS_DEV_OSO_N ;将来可扩展其他设备类型
  char            DevSN[8];  // 设备序列号；字符串
  unsigned int    iFs;       // 采样率
  long long       fTime;     // 总采样时间长度，单位 mSec
  unsigned short  iDataType; // 数据类型，参考宏定义;OSO-N 为 OTS_DATATYPE_Mixed
  float           fDelay;    // 数据延迟，单位 mSec
  unsigned int    iTimeStamp;// 第一个采样的 Unix 时间戳
 CHAN chans[OTS_MAX_CHAN];  // 各通道信息
	unsigned int    iTimeus;
	unsigned int gps_lable1;
	unsigned int gps_lable2;
  short           unused[35];    // 保留扩展
  unsigned short  nNextHeadSize;//下一Head大小
}__attribute__ ((packed,aligned(1)))iws_head;


/*
typedef struct OTS_FileHead{// 桔灯时间序列文件头信息
  char           Flag[4];   // 标志，必须为 OTS_FLAG
  unsigned int   iOTSVer;   // OTS 文件格式版本。当前为 0x1，如读取程序发现版本高于自己所支持的，应给与提示
  char           Proj[32];  // 工程名，可选留空
  unsigned int   iFileSize; // 全部文件大小，单位 Byte；
  unsigned short iHeadSize; // 文件头大小，单位 Byte ；
  unsigned short iDevID;    // 设备类型ID； OSO-N 为 OTS_DEV_OSO_N ;将来可扩展其他设备类型
  char           DevSN[8];  // 设备序列号；字符串
  unsigned int   iFs;       // 采样率
  unsigned short nChan;     // 通道个数
  short chorder;
  int            iChanMask; // 通道掩码。比如开启了1,2,3,5通道，则为 0x00000017
  unsigned long long          uTime;     // 总采样时间长度，单位 mSec
  unsigned int   iGain;     // 增益倍数，如无增益，设置为 0x1
  int            iADShift;  // AD 偏置，如无偏置，设置为 0
  int            iADMax;    // AD 最大值，即正满幅AD值，比如 24 位AD为 2^23
  int            iADMin;    // AD 最小值，即负满幅AD值
  int            iAD_para;  // AD设置，复制 para_AD（可选）
  int            iAD_unused;// AD设置扩展，未使用
  int            iADC;      // AD转换系数，OSO-N 为 0x5D1420;
  unsigned short iADCUnit;  // 转换单位，OSO-N 为 OTS_ADCUNIT_Volt
  float          iRefVolot; // 参考电压（可选）
  unsigned short iDataType; // 数据类型，参考宏定义;OSO-N 为 OTS_DATATYPE_Mixed
  float          fDelay;    // 数据延迟，单位 mSec
  unsigned int   iTimeStamp;// 第一个采样的 Unix 时间戳
  short unuse[3];
short           nextheadsize;    // 保留扩展
}__attribute__ ((packed,aligned(1)))oso_head;*/
#endif