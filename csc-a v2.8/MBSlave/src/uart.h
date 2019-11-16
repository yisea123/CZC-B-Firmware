/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.zyinside.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: UART.h
** Last modified Date: 2005-12-31 
** Last Version: v1.0
** Description: S3C2410�Ĵ��������� (ͷ�ļ�)
**
**------------------------------------------------------------------------------------------------------
** Created By: ���ܱ�
** Created date: 2005-12-31 
** Version: v1.0
** Descriptions:
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*******************************************************************************************************/
#ifndef  __UART_H
#define  __UART_H


#ifndef  IN_UART

    #ifdef __cplusplus
    extern "C" {
    #endif
    
    
/********************************************************************************************************
** Function name: UART_Select
** Descriptions: ѡ��Ҫ�����Ĵ��ڡ�(UART0--0��UART1--1)
**               ѡ�񴮿ں󣬱������һ��UART_Init()���г�ʼ��(ֻ��Ҫһ��)��
** Input: no    Ҫʹ�õĴ���
** Output: ������һ��ѡ�õĴ���
********************************************************************************************************/
extern int  UART_Select(uint8  no);



/********************************************************************************************************
** Function name: UART_Init
** Descriptions: ��ʼ�����ڡ�����Ϊ8λ����λ��1λֹͣλ������żУ�飬������ΪUART_BPS
** Input: ��
** Output: ��
********************************************************************************************************/
extern void  UART_Init(uint32 UART_BPS);



/********************************************************************************************************
** Function name: UART_SendByte
** Descriptions: �򴮿ڷ����ֽ����ݣ����ȴ�������ϡ�
** Input: data      Ҫ���͵�����
** Output: ��
********************************************************************************************************/
extern void  UART_SendByte(uint8 data);



/********************************************************************************************************
** Function name: UART_SendStr
** Descriptions: �򴮿ڷ���һ�ַ�����
**               ����'\n'�ַ�������ʱ�����'\r'�ַ���
** Input: str		Ҫ���͵��ַ�����ָ��
** Output: ��
********************************************************************************************************/
extern void  UART_SendStr(char const *str);



/********************************************************************************************************
** Function name: UART_GetKey
** Descriptions: ��UART�ڶ�ȡһ�ֽڰ������ݡ�
**               ��һֱ�ȴ���ֱ�����յ�1�ֽ����ݡ�
** Input: ��
** Output: ����ֵ���Ƕ���ֵ
********************************************************************************************************/
extern int  UART_GetKey(void);



/********************************************************************************************************/
    #ifdef __cplusplus
    }
    #endif  

#endif      // IN_UART    

#endif      // __UART_H
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
