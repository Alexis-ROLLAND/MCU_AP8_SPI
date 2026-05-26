/**
 * @file    lib_test_25lc256.c 
 * @author 	Alexis ROLLAND
 * @date	2024-04
 * @brief 	
 *  
 *
 */

#include "lib_test_25lc256.h"


/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/

spi_desc_t              spiModule;      /**< The eeprom device NEEDS an existing SPI device */
eeprom25lc256_desc_t    dev_eeprom;     /**< EEprom device  */


/*	Implémentation du code */
void Initialiser(void){
    eeprom25lc256_config_t  eepromCFG;
    
    TRIS_LED = 0;
    LED = 0;
    
    eepromCFG.initType = INIT_WITH_SPI2;    /**< SPI device initialisation scheme   */
    eepromCFG.pSpi = &spiModule;            /**< Address of the SPI deveice to use  */
    eepromCFG.eepromCS.port = GPIO_PORTD;   /**< P79_EECS is on RD12    */
    eepromCFG.eepromCS.bitNumber = 12;      /**< P79_EECS is on RD12    */  
    
    eeprom25LC256_Init(&eepromCFG, &dev_eeprom);
    
    __delay_ms(500);
    eeprom25LC256_WriteEnable(&dev_eeprom);
}    

#if ((CURRENT_TEST == TEST_WREN_WRDIS) || (CURRENT_TEST == TEST_READ_SR))
void mainTask(void){
    
    __delay_ms(2000);
    eeprom25LC256_WriteEnable(&dev_eeprom);
    LED = 1;
    #if (CURRENT_TEST == TEST_READ_SR)
    uint8_t sr;
    __delay_ms(2000);
    eeprom25LC256_ReadSR(&dev_eeprom, &sr);    
    if ((sr & MASK_WEL) != MASK_WEL) ErrorHandler();
    #endif
   
   __delay_ms(2000);
   eeprom25LC256_WriteDisable(&dev_eeprom);
   LED = 0;
   #if  (CURRENT_TEST == TEST_READ_SR)
   __delay_ms(2000);
   eeprom25LC256_ReadSR(&dev_eeprom, &sr);    
   if ((sr & MASK_WEL) != 0x00) ErrorHandler();
   #endif
}

#endif

#if (CURRENT_TEST == TEST_WRITE_AND_READ)
uint8_t TxData[16] = {10,20,30,40,50,66,70,80,90,100,110,120,130,140,150,160};
uint8_t RxData[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 
void mainTask(void){
    int i;
    static  uint8_t WriteOnce = 0;
       
    if (!WriteOnce){
        __delay_ms(500);
        eeprom25LC256_Write(&dev_eeprom,0x0000,TxData,16);
        WriteOnce = 1;
    }
    
    __delay_ms(500);
    
    
    eeprom25LC256_Read(&dev_eeprom,0x0000,RxData,16);
    
    for (i=0; i<16; ++i){
        if (RxData[i] != TxData[i]) ErrorHandler();
    }
}

#endif


