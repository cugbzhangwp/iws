typedef struct ABL_TX_CMD{ // 阿波罗发射配置 
int pak_type;
unsigned char order; // 阶数 
 unsigned char freq; // 频率 
unsigned short m_code_len; // m序列长度（供截断）
 int m_init_phz; // m序列初相 
int m_polynomial; // m序列多项式
 unsigned short cycle; // 周期
 int m_sync_Sec; //m序列同步秒 
short m_sync_mSec; // m序列同步毫秒
 short m_sync_uSec; // m序列同步微秒
 unsigned short m_sync_Flag; // m序列同步标记
 unsigned short pwm_freq; // m序列同步标记
 unsigned short pwm_ratio; // m序列同步标记
 unsigned short pwm_mode; // m序列同步标记
 unsigned short r_mode; // 
}__attribute__ ((packed,aligned(1)))ABL_TX_CMD;




typedef struct ABL_M_PARA{ // 阿波罗发射配置 
   unsigned char order; // m0阶数 
   unsigned char freq; // m0频率 
   unsigned short m_code_len; // m0序列长度（供截断）
   int m_init_phz; // m0序列初相 
   int m_polynomial; // m0序列多项式
   unsigned short cycle; // m0周期
   int m_sync_Sec; //m0序列同步秒 
   short m_sync_mSec; // m0序列同步毫秒
   short m_sync_uSec; // m0序列同步微秒
   unsigned short m_sync_Flag; // m0序列同步标记
   unsigned short pwm_freq; // 
   unsigned short pwm_ratio; // 
   unsigned short pwm_mode; // 
   unsigned short r_mode; //
   unsigned short m_freq_h; //
   unsigned short m_freq_l; //
   char unusr[28];
}__attribute__ ((packed,aligned(1)))ABL_M_PARA;









typedef struct ABL_TX_CMD_M2{ // 阿波罗发射配置 
   int pak_type;
   ABL_M_PARA m_para[2];
   unsigned short m_mode_ctrl; //
   char unuse[122];

}__attribute__ ((packed,aligned(1)))ABL_TX_CMD_M2;










typedef struct ABL_TX_CMD_NOPWD{ // 阿波罗发射配置 
int pak_type;
unsigned char order; // 阶数 
 unsigned char freq; // 频率 
unsigned short m_code_len; // m序列长度（供截断）
 int m_init_phz; // m序列初相 
int m_polynomial; // m序列多项式
 unsigned short cycle; // 周期
 int m_sync_Sec; //m序列同步秒 
short m_sync_mSec; // m序列同步毫秒
 short m_sync_uSec; // m序列同步微秒
 unsigned short m_sync_Flag; // m序列同步标记

}__attribute__ ((packed,aligned(1)))ABL_TX_CMD_NOPWD;
/*

Type , public , Bind( C ) :: Abl_Tx_cmd !// ABL发射命令 28Bytes
   Integer(Kind=C_INT)   :: pak_type
   Integer(Kind=C_CHAR)  :: order     !// 阶数    
   Integer(Kind=C_CHAR)  :: freq      !// 频率
   Integer(Kind=C_SHORT) :: m_code_len!// m序列长度（供截断）
   Integer(Kind=C_INT)   :: m_init_phz!// m序列初相          
   Integer(Kind=C_INT)   :: m_polynomial!// m序列多项式        
   Integer(Kind=C_SHORT) :: cycle       !// 周期               
   Integer(Kind=C_INT)   :: m_sync_Sec; !//m序列同步秒         
   Integer(Kind=C_SHORT) :: m_sync_mSec !// m序列同步毫秒      
   Integer(Kind=C_SHORT) :: m_sync_uSec !// m序列同步微秒      
   Integer(Kind=C_SHORT) :: m_sync_Flag !// m序列同步标记
 End Type Abl_Tx_Cmd*/
