/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.zyinside.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          target.c
** Last modified Date: 2006-01-11 
** Last Version:       v2.0
** Descriptions:       Ŀ�������Ĵ��룬�����쳣���������Ŀ����ʼ������
**
**------------------------------------------------------------------------------------------------------
** Created By:         ������
** Created date:       2003-06-05
** Version:            v1.0
** Descriptions:       ����
**
**------------------------------------------------------------------------------------------------------
** Modified by:        �ʴ�
** Modified date:      2006-01-06
** Version:            v2.0
** Descriptions:       ����ע��
**
**------------------------------------------------------------------------------------------------------
** Modified by:      
** Modified date:     
** Version:           
** Descriptions:      
**
********************************************************************************************************/
#define IN_TARGET
#include "config.h"

/*********************************************************************************************************
** ��������: FIQ_Exception
** ��������: �����ж��쳣���������û�������Ҫ�Լ��ı����
**           
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: ���ܱ�
** �ա���: 2005��11��11��
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** �ա���: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void  FIQ_Exception(void)
{
    while(1);                   // ��һ���滻Ϊ�Լ��Ĵ���
}
/*********************************************************************************************************
** ��������: Timer0_Exception
** ��������: ��ʱ��0�жϷ������
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: OSTimeTick
**
** ������: ������
** �ա���: 2004��2��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void Timer0_Exception(void)
{
	rSRCPND = 1 << 10;
	rINTPND = rINTPND;
    OSTimeTick();
}

/*********************************************************************************************************
** ��������: Timer0Init
** ��������: ��ʱ��0��ʼ��
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: ������
** �ա���: 2002��4��4��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void Timer0Init(void)
{
	// ��ʱ������
    rTCFG0 = 0;	                        // Ԥ��Ƶ��0����Ϊ250��ȡ��200KHz
    rTCFG1 = 1;					        // TIMER0��ȡ1/4��Ƶ��ȡ��50KHz
    rTCNTB0 = (PCLK / (4 * OS_TICKS_PER_SEC)) - 1;
    rTCON = (1 << 1);                   // ���¶�ʱ������		
    rTCON = (1 << 0) | (1 << 3);        // ������ʱ��
 }

/*********************************************************************************************************
** ��������: VICInit
** ��������: �����жϿ�������ʼ��
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: ������
** �ա���: 2004��2��2��
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** �ա���: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void VICInit(void)
{
	// �����жϿ�����
	rPRIORITY = 0x00000000;		// ʹ��Ĭ�ϵĹ̶������ȼ�
	rINTMOD = 0x00000000;		// �����жϾ�ΪIRQ�ж�
	VICVectAddr[10] = (uint32) Timer0_Exception;
	rINTMSK = ~(1<<10);			// ��TIMER0�ж�����
 //**********************************************************
// Modbusʹ�õ��ж�����	
 	VICVectAddr[12] = (uint32) T15_Exception;
 	rINTMSK &= ~(1<<12);			// ��TIMER2�ж�����
 	VICVectAddr[13] = (uint32) T35_Exception;
 	rINTMSK &= ~(1<<13);			// ��TIMER3�ж�����


 	VICVectAddr[23] = (uint32) UART1_Exception;
    // Clear Int Pending and Unmask 
    rSUBSRCPND =(BIT_SUB_RXD1|BIT_SUB_ERR1);
    rINTSUBMSK &=~(BIT_SUB_RXD1|BIT_SUB_ERR1);
    ClearPending(BIT_UART1);
    rINTMSK &=~(BIT_UART1);
 //**********************************************************

 }


// �����ⲿͼƬ��Դ
#if  DISP_BGPIC!=0
extern uint16 gImage_bliss[];			
#endif
/*********************************************************************************************************
** ��������: TargetInit
** ��������: Ŀ����ʼ�����룬����Ҫ�ĵط����ã�������Ҫ�ı�
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: ������
** �ա���: 2004��2��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void TargetInit(void)
{
    uint32 temp;

    OS_ENTER_CRITICAL();
    srand((uint32) TargetInit);
    VICInit();
    Timer0Init();

    IniUART1(115200);
    TimersInit();
    MBSlaveIni(&SlaveDevice);  			


    rRTCCON = 0x00;                             // ��ֹRTC�ӿ�(ֻ����RTC�����Ͷ���)	
    temp = rBCDYEAR & 0xff;
    if(temp > 0x99)                             // ���ж�
    {
        goto ChangeRTC;
    }
    if (temp == 0)
    {
        goto ChangeRTC;
    }
    temp = rBCDMON & 0x1F;
    if (temp > 0x12)            // ���ж�		
    {
        goto ChangeRTC;
    }
    if (temp == 0)
    {
        goto ChangeRTC;
    }
    temp = rBCDDATE & 0x3F;
    if (temp > 0x31)            // ���ж�		
    {
        goto ChangeRTC;
    }
    if (temp == 0)
    {
        goto ChangeRTC;
    }
    temp = rBCDHOUR & 0x3F;
    if (temp > 0x24)            // ʱ�ж�		
    {
        goto ChangeRTC;
    }
    temp = rBCDMIN & 0x7F;
    if (temp > 0x59)            // ���ж�		
    {
        goto ChangeRTC;
    }
    temp = rBCDSEC & 0x7F;
    if (temp > 0x59)            // ���ж�
    {
        goto ChangeRTC;
    }

    OS_EXIT_CRITICAL();

    return;
    
ChangeRTC:	
	rRTCCON = 0x01;             // ��׼ģʽ��RTC�ӿ�ʹ��(����д)
	
   	rBCDYEAR = 0x05;
    rBCDMON  = 0x11;
    rBCDDATE = 0x28;         
    rBCDDAY  = 0x01;            // ���ڣ�MON:1  TUE:2  WED:3  THU:4  FRI:5  SAT:6  SUN:7
    rBCDHOUR = 0x17;
    rBCDMIN  = 0x01;
    rBCDSEC  = 0x00;
    
    rRTCCON  = 0x00;            // ��ֹRTC�ӿ�(ֻ����RTC�����Ͷ���)	
    
}

/*********************************************************************************************************
**                  ����ΪһЩ��ϵͳ��صĿ⺯����ʵ��
**                  ����������ads�Ĳο���������⺯���ֲ�
**                  �û����Ը����Լ���Ҫ���޸�        
********************************************************************************************************/
/*********************************************************************************************************
**                  The implementations for some library functions
**                  For more details, please refer to the ADS compiler handbook and The library 
**                  function manual
**                  User could change it as needed       
********************************************************************************************************/

#include "rt_sys.h"
#include "stdio.h"

#pragma import(__use_no_semihosting_swi)

        int __rt_div0(int a)
{
    a = a;
    return 0;
}

        int fputc(int ch,FILE *f)
{
    ch = ch;
    f = f;
    return 0;
}

    int fgetc(FILE *f)
{
    f = f;
    return 0;
}


        int _sys_close(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

        int _sys_write(FILEHANDLE fh, const unsigned char * buf,
                      unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    return 0;
}
        int _sys_read(FILEHANDLE fh, unsigned char * buf,
                     unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    
    return 0;
}

       void _ttywrch(int ch)
{
    ch = ch;
}

        int _sys_istty(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
        int _sys_seek(FILEHANDLE fh, long pos)
{
    fh = fh;
    pos = pos;
    return 0;
}
        int _sys_ensure(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

        long _sys_flen(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
       int _sys_tmpnam(char * name, int sig, unsigned maxlen)
{
    name = name;
    sig = sig;
    maxlen = maxlen;
    return 0;
}


        void _sys_exit(int returncode)
{
    returncode = returncode;
}

        char *_sys_command_string(char * cmd, int len)
{
    cmd = cmd;
    len = len;
    return 0;
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/