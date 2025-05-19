/**
 * @file    lib_25lc256.c
 * @author 	Alexis ROLLAND
 * @date	2024-04
 * @brief 	
 *          
 *
 */


#include "lib_25lc256.h" // Inclusion du fichier .h "Applicatif" renommé

/* Directives de compilation		*/

/* Déclarations des variables globales 	*/
spi_cs_t    eepromCS;

/*	Implémentation du code */
eeprom25lc256_err_t eeprom25LC256_Init(const eeprom25lc256_config_t *pCfg, eeprom25lc256_desc_t *pdev){
    spi_config_t    spiCfg;
    
    // SPI Config
    if (pCfg->initType != INIT_ALREADY_DONE){
        spiCfg.spiClockPolarity = CLK_IDLE_IS_LOW;
        spiCfg.spiClockPhase = ACTIVE_TO_IDLE_CPHASE;
        spiCfg.spiDataFormat = BITS8;
        spiCfg.spiSamplePoint = MID_SMP;
        spiCfg.spiPrimaryPrescaler = PRI_PRE_4;
        spiCfg.spiSecondaryPrescaler = SEC_PRE_1;
        switch(pCfg->initType){
            case INIT_WITH_SPI1:
                spi_init(_SPI1, &spiCfg, pCfg->pSpi);
                break;
            case INIT_WITH_SPI2:
                spi_init(_SPI2, &spiCfg, pCfg->pSpi);
                break;
            default: break;
            }
        }
    
    pdev->pSpi = pCfg->pSpi;
   
    eepromCS = pCfg->eepromCS;
    pdev->pEepromCS = &eepromCS;
    
    // Configure & init CS Line
    spi_init_cs(pdev->pSpi, pdev->pEepromCS);
       
    return EEPROM_25LC256_OK;
}        

    
eeprom25lc256_err_t eeprom25LC256_ReadSR(const eeprom25lc256_desc_t *pdev, uint8_t *pSr){
    spi_assertCS(pdev->pSpi,pdev->pEepromCS);
    spi_transfer_raw_byte(pdev->pSpi, CMD_RDSR, NULL);     /**<    Send RDSR instruction word - Receive dummy */
    spi_transfer_raw_byte(pdev->pSpi, DUMMY, pSr);         /**<    Send dummy, read SR */
    spi_deassertCS(pdev->pSpi,pdev->pEepromCS);
    
    return EEPROM_25LC256_OK;
}
    

eeprom25lc256_err_t eeprom25LC256_WriteEnable(const eeprom25lc256_desc_t *pdev){
    spi_assertCS(pdev->pSpi,pdev->pEepromCS);
    spi_transfer_raw_byte(pdev->pSpi, CMD_WREN, NULL);  /**<   Send WREN Code  */
    spi_deassertCS(pdev->pSpi,pdev->pEepromCS);
    return EEPROM_25LC256_OK;
}

eeprom25lc256_err_t eeprom25LC256_WriteDisable(const eeprom25lc256_desc_t *pdev){
    spi_assertCS(pdev->pSpi,pdev->pEepromCS);
    spi_transfer_raw_byte(pdev->pSpi, CMD_WRDIS, NULL);  /**< Send WRDIS Code   */
    spi_deassertCS(pdev->pSpi,pdev->pEepromCS);
    return EEPROM_25LC256_OK;
}

eeprom25lc256_err_t eeprom25LC256_Write(const eeprom25lc256_desc_t *pdev, uint16_t StartAddress, const uint8_t *pData, size_t Length){   
    if (StartAddress > EEPROM_MAX_ADDR) return EEPROM_25LC256_BAD_ADDRESS_ERROR;   
    if (Length > 64) return EEPROM_25LC256_PAYLOAD_ERROR;

    spi_assertCS(pdev->pSpi,pdev->pEepromCS);       // Select EEPROM
    spi_transfer_raw_byte(pdev->pSpi, CMD_WRITE, NULL);                /**<  Send Write command - Receive dummy */
    spi_transfer_raw_byte(pdev->pSpi,(StartAddress>>8),NULL);          /**<  Send Higher Byte of address    */
    spi_transfer_raw_byte(pdev->pSpi,(StartAddress & 0x00FF),NULL);    /**<  Send lower Byte of address */
    spi_transfer_raw_bytes(pdev->pSpi,pData, NULL, Length);
    spi_deassertCS(pdev->pSpi,pdev->pEepromCS);
    
    return EEPROM_25LC256_OK;
}

eeprom25lc256_err_t eeprom25LC256_Read(const eeprom25lc256_desc_t *pdev, uint16_t StartAddress, uint8_t *pData, size_t  Length){
    if (StartAddress > EEPROM_MAX_ADDR) return EEPROM_25LC256_BAD_ADDRESS_ERROR;
    spi_assertCS(pdev->pSpi,pdev->pEepromCS);       // Select EEPROM
    spi_transfer_raw_byte(pdev->pSpi, CMD_READ, NULL);                 /**<  Send Read command - Receive dummy */
    spi_transfer_raw_byte(pdev->pSpi,(StartAddress>>8),NULL);          /**<  Send Higher Byte of address    */
    spi_transfer_raw_byte(pdev->pSpi,(StartAddress & 0x00FF),NULL);    /**<  Send lower Byte of address */
    spi_transfer_raw_bytes(pdev->pSpi,NULL,pData,Length);
    spi_deassertCS(pdev->pSpi,pdev->pEepromCS);
    return EEPROM_25LC256_OK;
}


