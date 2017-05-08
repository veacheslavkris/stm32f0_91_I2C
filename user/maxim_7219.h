#ifndef MAXIM_7219_H
    #define MAXIM_7219_H

    #include "stdint.h"
    
    
#define ADDR_NO_OP          (0x00)
#define ADDR_DIG_0          (0x01)
#define ADDR_DIG_1          (0x02)
#define ADDR_DIG_2          (0x03)
#define ADDR_DIG_3          (0x04)
#define ADDR_DIG_4          (0x05)
#define ADDR_DIG_5          (0x06)
#define ADDR_DIG_6          (0x07)
#define ADDR_DIG_7          (0x08)
#define ADDR_DECODE_MODE    (0x09)
#define ADDR_INTENSITY      (0x0A)
#define ADDR_SCAN_LIMIT     (0x0B)
#define ADDR_SHUTDOWN       (0x0C)
#define ADDR_DISPLAY_TEST   (0x0F)
    
#define REG_DATA_CLEAR      (0x00)
#define REG_DATA_D_0        (0x01)     
#define REG_DATA_D_1        (0x02)     
#define REG_DATA_D_2        (0x04)     
#define REG_DATA_D_3        (0x08)     
#define REG_DATA_D_4        (0x10)     
#define REG_DATA_D_5        (0x20)     
#define REG_DATA_D_6        (0x40)     
#define REG_DATA_D_7        (0x80)     


/*----- ADDR_SHUTDOWN -----*/
#define SD_MODE                 REG_DATA_CLEAR
#define SD_NORMAL_OPERATION     REG_DATA_D_0
    
    
/*----- ADDR_DECODE_MODE -----*/    
#define NO_DECODE_FOR_DIGITS    REG_DATA_CLEAR
    
    
/*----- ADDR_INTENSITY -----*/
#define INTENSITY_1_32      (0x00)
#define INTENSITY_3_32      (0x01)
#define INTENSITY_5_32      (0x02)
#define INTENSITY_7_32      (0x03)
#define INTENSITY_9_32      (0x04)
#define INTENSITY_11_32     (0x05)
#define INTENSITY_13_32     (0x06)
#define INTENSITY_15_32     (0x07)
#define INTENSITY_17_32     (0x08)
#define INTENSITY_19_32     (0x09)
#define INTENSITY_21_32     (0x0A)
#define INTENSITY_23_32     (0x0B)
#define INTENSITY_25_32     (0x0C)
#define INTENSITY_27_32     (0x0D)
#define INTENSITY_29_32     (0x0E)
#define INTENSITY_31_32     (0x0F)   

#define INTENSITY_MIN       INTENSITY_1_32    
#define INTENSITY_MAX       INTENSITY_31_32
        
/*----- ADDR_SCAN_LIMIT -----*/    
#define DISPLAY_DIGIT_0         (0x00)    
#define DISPLAY_DIGIT_01        (0x01) 
#define DISPLAY_DIGIT_012       (0x02) 
#define DISPLAY_DIGIT_0123      (0x03) 
#define DISPLAY_DIGIT_01234     (0x04) 
#define DISPLAY_DIGIT_012345    (0x05) 
#define DISPLAY_DIGIT_0123456   (0x06) 
#define DISPLAY_DIGIT_01234567  (0x07) 
    
/*----- ADDR_DISPLAY_TEST -----*/   
#define DT_NORMAL_OPERATION     REG_DATA_CLEAR
#define DT_MODE                 REG_DATA_D_0
        
    
    
    
#define REG_DATA_SEG_G      REG_DATA_D_0    
#define REG_DATA_SEG_F      REG_DATA_D_1
#define REG_DATA_SEG_E      REG_DATA_D_2    
#define REG_DATA_SEG_D      REG_DATA_D_3
#define REG_DATA_SEG_C      REG_DATA_D_4
#define REG_DATA_SEG_B      REG_DATA_D_5
#define REG_DATA_SEG_A      REG_DATA_D_6
#define REG_DATA_SEG_DP     REG_DATA_D_7

#define DIGIT_0         (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F)
#define DIGIT_1         (REG_DATA_SEG_B|REG_DATA_SEG_C)
#define DIGIT_2         (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_G|REG_DATA_SEG_E|REG_DATA_SEG_D)
#define DIGIT_3         (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_G)
#define DIGIT_4         (REG_DATA_SEG_F|REG_DATA_SEG_G|REG_DATA_SEG_B|REG_DATA_SEG_C)
#define DIGIT_5         (REG_DATA_SEG_A|REG_DATA_SEG_F|REG_DATA_SEG_G|REG_DATA_SEG_C|REG_DATA_SEG_D)
#define DIGIT_6         (REG_DATA_SEG_A|REG_DATA_SEG_F|REG_DATA_SEG_E|REG_DATA_SEG_D|REG_DATA_SEG_C|REG_DATA_SEG_G)
#define DIGIT_7         (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_C)
#define DIGIT_8         (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define DIGIT_9         (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define DIGIT_POINT     REG_DATA_SEG_DP
    

    


#define SIGN_A          (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_E          (REG_DATA_SEG_A|REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_C          (REG_DATA_SEG_A|REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F)
#define SIGN_F          (REG_DATA_SEG_A|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_H          (REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_L          (REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F)
#define SIGN_P          (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_U          (REG_DATA_SEG_B|REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F)
#define SIGN_UNDSCR     (REG_DATA_SEG_D)
#define SIGN_UPRSCR     (REG_DATA_SEG_A)
#define SIGN_UND_O      (REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_G)    
#define SIGN_UPR_O      (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_UND_U      (REG_DATA_SEG_C|REG_DATA_SEG_D|REG_DATA_SEG_E)        
#define SIGN_UND_R      (REG_DATA_SEG_E|REG_DATA_SEG_G)            
#define SIGN_UND_T      (REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_UND_C      (REG_DATA_SEG_D|REG_DATA_SEG_E|REG_DATA_SEG_G)    
#define SIGN_UPR_C      (REG_DATA_SEG_A|REG_DATA_SEG_F|REG_DATA_SEG_G)
#define SIGN_UND_N      (REG_DATA_SEG_C|REG_DATA_SEG_E|REG_DATA_SEG_G)        
#define SIGN_UPR_N      (REG_DATA_SEG_A|REG_DATA_SEG_B|REG_DATA_SEG_F)
#define SIGN_TIRE       (REG_DATA_SEG_G)    
    
void latch_max7219(void);
void set_data_bit(uint32_t val);
void config_max7219(uint8_t reg_addr, uint8_t reg_data);

void send_bits(uint32_t val);


void Max7219_SetDigitSegment(uint8_t reg_digit, uint8_t reg_data_seg);
void Max7219_Init(void);
void Max7219_ClearAllDigits(void);







#endif

