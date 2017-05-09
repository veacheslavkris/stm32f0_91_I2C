#include "sys_clock.h" 

const uint32_t HSE_STARTUP_TIMEOUT = 1000;

void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/

  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
  /* Enable HSE */ 

/**
 *#define  RCC_CR_HSEON                        ((uint32_t)0x00010000U)        External High Speed clock enable
 *#define  RCC_CR_HSERDY                       ((uint32_t)0x00020000U)        External High Speed clock ready flag
 *#define  RCC_CR_HSEBYP                       ((uint32_t)0x00040000U)        External High Speed clock Bypass
 *#define  RCC_CR_CSSON                        ((uint32_t)0x00080000U)        Clock Security System enable
 *#define  RCC_CR_PLLON                        ((uint32_t)0x01000000U)        PLL enable
 *#define  RCC_CR_PLLRDY                       ((uint32_t)0x02000000U)        PLL clock ready flag
 */

  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  
	
	/*
	* #define  RCC_CFGR_HPRE_DIV1                  ((uint32_t)0x00000000U)        SYSCLK not divided 
	* #define  RCC_CFGR_HPRE_DIV2                  ((uint32_t)0x00000080U)        SYSCLK divided by 2
	* #define  RCC_CFGR_HPRE_DIV4                  ((uint32_t)0x00000090U)        SYSCLK divided by 4
	* #define  RCC_CFGR_HPRE_DIV8                  ((uint32_t)0x000000A0U)        SYSCLK divided by 8
	* #define  RCC_CFGR_HPRE_DIV16                 ((uint32_t)0x000000B0U)        SYSCLK divided by 16
	* #define  RCC_CFGR_HPRE_DIV64                 ((uint32_t)0x000000C0U)        SYSCLK divided by 64
	* #define  RCC_CFGR_HPRE_DIV128                ((uint32_t)0x000000D0U)        SYSCLK divided by 128
	* #define  RCC_CFGR_HPRE_DIV256                ((uint32_t)0x000000E0U)        SYSCLK divided by 256
	* #define  RCC_CFGR_HPRE_DIV512                ((uint32_t)0x000000F0U)        SYSCLK divided by 512

	* #define  RCC_CFGR_PPRE_DIV1                  ((uint32_t)0x00000000U)        HCLK not divided
	* #define  RCC_CFGR_PPRE_DIV2                  ((uint32_t)0x00000400U)        HCLK divided by 2
	* #define  RCC_CFGR_PPRE_DIV4                  ((uint32_t)0x00000500U)        HCLK divided by 4
	* #define  RCC_CFGR_PPRE_DIV8                  ((uint32_t)0x00000600U)        HCLK divided by 8
	* #define  RCC_CFGR_PPRE_DIV16                 ((uint32_t)0x00000700U)        HCLK divided by 16

	* #define  RCC_CFGR_PLLXTPRE                   ((uint32_t)0x00020000U)        HSE divider for PLL entry
	* #define  RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV1   ((uint32_t)0x00000000U)        HSE/PREDIV clock not divided for PLL entry
	* #define  RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV2   ((uint32_t)0x00020000U)        HSE/PREDIV clock divided by 2 for PLL entry

	*/
  
if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

     /* HCLK = SYSCLK / 1 */
     RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1; // POSITION 6
       
     /* PCLK = HCLK / 8 */
     RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV8; // POSITION 7

    /* PLL configuration */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL));
    
	
	/**
	 *#define  RCC_CFGR_PLLSRC                     ((uint32_t)0x00018000U)        PLL entry clock source
	 *#define  RCC_CFGR_PLLSRC_HSI_DIV2            ((uint32_t)0x00000000U)        HSI clock divided by 2 selected as PLL entry clock source
	 *#define  RCC_CFGR_PLLSRC_HSI_PREDIV          ((uint32_t)0x00008000U)        HSI/PREDIV clock selected as PLL entry clock source
	 *#define  RCC_CFGR_PLLSRC_HSE_PREDIV          ((uint32_t)0x00010000U)        HSE/PREDIV clock selected as PLL entry clock source
	 *#define  RCC_CFGR_PLLSRC_HSI48_PREDIV        ((uint32_t)0x00018000U)        HSI48/PREDIV clock selected as PLL entry clock source
	 */

	
	/**
	 PLLMUL configuration
	 
	 *#define  RCC_CFGR_PLLMUL                     ((uint32_t)0x003C0000U)        PLLMUL[3:0] bits (PLL multiplication factor)
	 *#define  RCC_CFGR_PLLMUL_0                   ((uint32_t)0x00040000U)        Bit 0
	 *#define  RCC_CFGR_PLLMUL_1                   ((uint32_t)0x00080000U)        Bit 1
	 *#define  RCC_CFGR_PLLMUL_2                   ((uint32_t)0x00100000U)        Bit 2
	 *#define  RCC_CFGR_PLLMUL_3                   ((uint32_t)0x00200000U)        Bit 3
	 *
	 *#define  RCC_CFGR_PLLMUL2                    ((uint32_t)0x00000000U)        PLL input clock*2
	 *#define  RCC_CFGR_PLLMUL3                    ((uint32_t)0x00040000U)        PLL input clock*3
	 *#define  RCC_CFGR_PLLMUL4                    ((uint32_t)0x00080000U)        PLL input clock*4
	 *#define  RCC_CFGR_PLLMUL5                    ((uint32_t)0x000C0000U)        PLL input clock*5
	 *#define  RCC_CFGR_PLLMUL6                    ((uint32_t)0x00100000U)        PLL input clock*6
	 *#define  RCC_CFGR_PLLMUL7                    ((uint32_t)0x00140000U)        PLL input clock*7
	 *#define  RCC_CFGR_PLLMUL8                    ((uint32_t)0x00180000U)        PLL input clock*8
	 *#define  RCC_CFGR_PLLMUL9                    ((uint32_t)0x001C0000U)        PLL input clock*9
	 *#define  RCC_CFGR_PLLMUL10                   ((uint32_t)0x00200000U)        PLL input clock10
	 *#define  RCC_CFGR_PLLMUL11                   ((uint32_t)0x00240000U)        PLL input clock*11
	 *#define  RCC_CFGR_PLLMUL12                   ((uint32_t)0x00280000U)        PLL input clock*12
	 *#define  RCC_CFGR_PLLMUL13                   ((uint32_t)0x002C0000U)        PLL input clock*13
	 *#define  RCC_CFGR_PLLMUL14                   ((uint32_t)0x00300000U)        PLL input clock*14
	 *#define  RCC_CFGR_PLLMUL15                   ((uint32_t)0x00340000U)        PLL input clock*15
	 *#define  RCC_CFGR_PLLMUL16                   ((uint32_t)0x00380000U)        PLL input clock*16
 */


	/**
	 *                          Bit definition for RCC_CFGR2 register  
	 *PREDIV configuration
	 *#define  RCC_CFGR2_PREDIV                    ((uint32_t)0x0000000FU)        PREDIV[3:0] bits
	 *#define  RCC_CFGR2_PREDIV_0                  ((uint32_t)0x00000001U)        Bit 0
	 *#define  RCC_CFGR2_PREDIV_1                  ((uint32_t)0x00000002U)        Bit 1
	 *#define  RCC_CFGR2_PREDIV_2                  ((uint32_t)0x00000004U)        Bit 2
	 *#define  RCC_CFGR2_PREDIV_3                  ((uint32_t)0x00000008U)        Bit 3
	 *
	 *#define  RCC_CFGR2_PREDIV_DIV1               ((uint32_t)0x00000000U)        PREDIV input clock not divided
	 *#define  RCC_CFGR2_PREDIV_DIV2               ((uint32_t)0x00000001U)        PREDIV input clock divided by 2
	 *#define  RCC_CFGR2_PREDIV_DIV3               ((uint32_t)0x00000002U)        PREDIV input clock divided by 3
	 *#define  RCC_CFGR2_PREDIV_DIV4               ((uint32_t)0x00000003U)        PREDIV input clock divided by 4
	 *#define  RCC_CFGR2_PREDIV_DIV5               ((uint32_t)0x00000004U)        PREDIV input clock divided by 5
	 *#define  RCC_CFGR2_PREDIV_DIV6               ((uint32_t)0x00000005U)        PREDIV input clock divided by 6
	 *#define  RCC_CFGR2_PREDIV_DIV7               ((uint32_t)0x00000006U)        PREDIV input clock divided by 7
	 *#define  RCC_CFGR2_PREDIV_DIV8               ((uint32_t)0x00000007U)        PREDIV input clock divided by 8
	 *#define  RCC_CFGR2_PREDIV_DIV9               ((uint32_t)0x00000008U)        PREDIV input clock divided by 9
	 *#define  RCC_CFGR2_PREDIV_DIV10              ((uint32_t)0x00000009U)        PREDIV input clock divided by 10
	 *#define  RCC_CFGR2_PREDIV_DIV11              ((uint32_t)0x0000000AU)        PREDIV input clock divided by 11
	 *#define  RCC_CFGR2_PREDIV_DIV12              ((uint32_t)0x0000000BU)        PREDIV input clock divided by 12
	 *#define  RCC_CFGR2_PREDIV_DIV13              ((uint32_t)0x0000000CU)        PREDIV input clock divided by 13
	 *#define  RCC_CFGR2_PREDIV_DIV14              ((uint32_t)0x0000000DU)        PREDIV input clock divided by 14
	 *#define  RCC_CFGR2_PREDIV_DIV15              ((uint32_t)0x0000000EU)        PREDIV input clock divided by 15
	 *#define  RCC_CFGR2_PREDIV_DIV16              ((uint32_t)0x0000000FU)        PREDIV input clock divided by 16

	 */

	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL12);  // POSITION 2 _ 4
    
	RCC->CFGR2 &= (uint32_t)((uint32_t)~(RCC_CFGR2_PREDIV_2));
  RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV_DIV2); // POSITION 3

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON; // POSITION 1

	/* Wait till PLL is ready */
	while((RCC->CR & RCC_CR_PLLRDY) == 0)
	{
	}

	/**
	 *Bit definition for RCC_CFGR register  
	 *SW configuration
	 *#define  RCC_CFGR_SW                         ((uint32_t)0x00000003U)        SW[1:0] bits (System clock Switch)
	 *#define  RCC_CFGR_SW_0                       ((uint32_t)0x00000001U)        Bit 0
	 *#define  RCC_CFGR_SW_1                       ((uint32_t)0x00000002U)        Bit 1
	 */

	/**
	 *#define  RCC_CFGR_SW_HSI                     ((uint32_t)0x00000000U)        HSI selected as system clock
	 *#define  RCC_CFGR_SW_HSE                     ((uint32_t)0x00000001U)        HSE selected as system clock
	 *#define  RCC_CFGR_SW_PLL                     ((uint32_t)0x00000002U)        PLL selected as system clock
	 *#define  RCC_CFGR_SW_HSI48                   ((uint32_t)0x00000003U)        HSI48 selected as system clock
	*/

	/**
	 *SWS configuration
	 *#define  RCC_CFGR_SWS                        ((uint32_t)0x0000000CU)        SWS[1:0] bits (System Clock Switch Status)
	 *#define  RCC_CFGR_SWS_0                      ((uint32_t)0x00000004U)        Bit 0
	 *#define  RCC_CFGR_SWS_1                      ((uint32_t)0x00000008U)        Bit 1
	 *
	 *#define  RCC_CFGR_SWS_HSI                    ((uint32_t)0x00000000U)        HSI oscillator used as system clock
	 *#define  RCC_CFGR_SWS_HSE                    ((uint32_t)0x00000004U)        HSE oscillator used as system clock
	 *#define  RCC_CFGR_SWS_PLL                    ((uint32_t)0x00000008U)        PLL used as system clock
	 *#define  RCC_CFGR_SWS_HSI48                  ((uint32_t)0x0000000CU)        HSI48 oscillator used as system clock
	 */


    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;   // POSITION 5  

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
//		LED_GREEN_ON;
  }
}

































