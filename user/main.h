#include "stm32f091xc.h"
#include "sys_clock.h"
#include "systick.h"
#include "TMP275.h"

#include "display_max_7219.h"
#include "rtc_lse_32768.h"
#include "uart8.h"
#include "pc13btn.h"
#include "pa5_led.h"
#include "DecBcdCharConverter.h"


/* Private define ------------------------------------------------------------*/





void cycle_digits(void);
