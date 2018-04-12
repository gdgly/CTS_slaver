/*******************************************************************************
* ��Ŀ��� :  
* �汾��   :  1.0
* �ļ���   :  serial_port.c
* �������� :  
* ����     :  
* ����˵�� :  ���������
*******************************************************************************/
/*******************************************************************************
*                                 ͷ�ļ�����
*******************************************************************************/
#include "serial_port.h"
#include "hardware.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_dma.h"
#include "datatype.h"

/*******************************************************************************
*                                 �ֲ�׃��
*******************************************************************************/

/*******************************************************************************
*                                 ȫ��׃��
*******************************************************************************/

/*******************************************************************************
*                                 �������x
*******************************************************************************/
/*******************************************************************************
* �������� : UARTInit
* �������� : ��ʼ������
* ������� : ��
* ������� : ��
* �������� : ��
*******************************************************************************/
void UARTInit(uint8_t* p_rec_buf, uint32_t rec_num)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(UART_IO_CLKSRC, ENABLE);  //ʹ��GPIOA��ʱ��
	RCC_AHBPeriphClockCmd(DMA_CLKSRC, ENABLE);
	
	#ifdef STM32F030F4P6
	RCC_APB2PeriphClockCmd(UART_CLKSRC, ENABLE);//ʹ��USART��ʱ��
	#else
	RCC_APB1PeriphClockCmd(UART_CLKSRC, ENABLE);//ʹ��USART��ʱ��
	#endif
	
//	RCC_APB2PeriphClockCmd(UART_CLKSRC, ENABLE);//ʹ��USART��ʱ��
//	//RCC_APB1PeriphClockCmd(UART_CLKSRC, ENABLE);//ʹ��USART��ʱ��
	
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(UART_IO_PORT, UART_RX_AF_PIN_SOURCE, GPIO_AF_1);//����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(UART_IO_PORT, UART_TX_AF_PIN_SOURCE, GPIO_AF_1);//����PA10�ɵڶ���������  RX	

	GPIO_InitStructure.GPIO_Pin = UART_RX_PIN | UART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(UART_IO_PORT, &GPIO_InitStructure);

	/* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = UART_BAUDRATE;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART, &USART_InitStructure);	
	#if 1	
	//DMA �l������
	DMA_DeInit(UART_DMA_TX_CHANNEL);	/* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART->TDR);//�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//������Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = (uint32_t)0;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���ݿ���Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//���ݿ���Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority�趨DMAͨ��x���������ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(UART_DMA_TX_CHANNEL, &DMA_InitStructure);

		//DMA ��������
	DMA_DeInit(UART_DMA_RX_CHANNEL);	/* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART->RDR);//�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)p_rec_buf;//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = rec_num;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���ݿ���Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//���ݿ���Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority�趨DMAͨ��x���������ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(UART_DMA_RX_CHANNEL, &DMA_InitStructure);
	
	USART_DMACmd(UART, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(UART_DMA_RX_CHANNEL, ENABLE);
	DMA_Cmd(UART_DMA_TX_CHANNEL, ENABLE);
	#endif	
	USART_Cmd(UART, ENABLE);

}

/*******************************************************************************
* �������� : DMA_ReWorkCfg
* �������� : ����һ���Ѿ���ʼ����DMAͨ��, ���������乤��
* ������� : dma_Channel_x  ָ��DMAͨ����ָ��
*            mem_address    �������õ��ڴ��ַ
*            num            ���´������Ŀ
*******************************************************************************/
static void DMA_ReWorkCfg (DMA_Channel_TypeDef* dma_Channel_x, uint32_t mem_address, uint32_t num)
{
	dma_Channel_x->CCR &= 0xFFFFFFFE;                /* �Ƚ�ֹDMAxͨ�� */

	dma_Channel_x->CMAR = mem_address;               /* ���������ڴ��ַ */

	dma_Channel_x->CNDTR = num;                      /* ���´�����Ŀ */

	dma_Channel_x->CCR |= 0x00000001;                /* ʹ��DMAxͨ�� */
}

/*******************************************************************************
* �������� : UartSendNBytes
* �������� : ͨ��DMA���ƴӴ��ڷ�������
* ������� : ��
* ������� : ��
* �������� : ��
*******************************************************************************/
void UartSendNBytes (uint8_t *p_buf, uint32_t num)
{
	DMA_ReWorkCfg(UART_DMA_TX_CHANNEL, (uint32_t)p_buf, num);
}

/*******************************************************************************
* �������� : GetUartReceiverResidualCnt
* �������� : ȡ����pdc������ʣ�����
* ������� : ��
* ������� : ��
* �������� :����pdc������ʣ����� 
*******************************************************************************/
int32_t GetUartReceiverResidualCnt(void)
{
	return(DMA_GetCurrDataCounter(UART_DMA_RX_CHANNEL));
}


