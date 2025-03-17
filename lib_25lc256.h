/**
 * @file    lib_25lc256.h 
 * @author 	Alexis ROLLAND
 * @date	2024/04
 * @brief 	Header File for the SPI EEPROM 25LC256 driver 
 * @attention the low level SPI driver (lib_spi_pic24_ll) is needed
 */

#ifndef	__LIB_25LC256_H__
#define	__LIB_25LC256_H__
#include "lib_spi_pic24_ll.h"

/** 
 * EEPROM INSTRUCTION WORDS
 */
#define     CMD_RDSR        0x05    /**< ReaD Status Register Command Word  */        
#define     CMD_WREN        0x06    /**< WRite ENable Command Word  */
#define     CMD_WRDIS       0x04    /**< WRite DISable Command Word  */
#define     CMD_WRITE       0x02    /**< WRITE data Command Word  */
#define     CMD_READ        0x03    /**< READ data Command Word  */

#define     DUMMY           0xFF        /**< Dummy value for Writes only exchanges  */
#define     EEPROM_MAX_ADDR 0x7FFF      /**< Last EEPROM address    */

typedef enum{
    EEPROM_25LC256_OK = 0,                  /**< Success    */
    EEPROM_25LC256_ERROR = -1,              /**< Non specific EEPROM error  */
    EEPROM_25LC256_PAYLOAD_ERROR = -2,      /**< Payload exceeds 64 bytes for write operation   */
    EEPROM_25LC256_BAD_ADDRESS_ERROR = -3   /**< Address is out of range ( > 0x7FFF)    */     
} eeprom25lc256_err_t;

typedef enum    {   INIT_WITH_SPI1,     /**< Initialize with SPI1 scheme    */
                    INIT_WITH_SPI2,     /**< Initialize with SPI2 scheme    */
                    INIT_ALREADY_DONE   /**< do not initialize SPI module   */
} eeprom25lc256_spi_init_type_t;

/** 
 * Type eeprom25lc256_config_t
 */
typedef struct{
    eeprom25lc256_spi_init_type_t   initType;   /**< Initializing scheme (with SPI1, with SPI2 or init already done) */  
    spi_cs_t    eepromCS;                       /**< Use spi_cs_t to specify CS line for the component */
    spi_desc_t  *pSpi;                          /**< SPI descriptor initialized or not    */
} eeprom25lc256_config_t;

 /** 
 * Type eeprom25lc256_desc_t
 */
typedef struct{
    spi_desc_t  *pSpi;              /**< Fully initialized SPI descriptor    */
    spi_cs_t    *pEepromCS;         /**< CS line   */
} eeprom25lc256_desc_t;
    
/**
 * @brief   Initialize the device : SPI mode = 0, 8bits data format, 1MHz (Fcy = 4MHz)
 * 
 * @param[in]	pCfg    Address of the config structure
 * @param[out]  pDev    Address of the device descriptor
 * 
 * @return      EEPROM_25LC256_OK on success
 */
eeprom25lc256_err_t eeprom25LC256_Init(const eeprom25lc256_config_t *pCfg, eeprom25lc256_desc_t *pdev);

/**
 * @brief   Reads the SR register (Status register) 
 * 
 * @param[in]   pDev    Address of the device descriptor
 * @param[out]  pSr     Address of the byte receiving the SR value   	
 * 
 * @return  EEPROM_25LC256_OK on success   
  */
eeprom25lc256_err_t eeprom25LC256_ReadSR(const eeprom25lc256_desc_t *pdev, uint8_t *pSr);

/**
 * @brief       Sets the Write Enable Latch (enables writings)
 * 
 * @param[in]   pDev    Address of the device descriptor
 * 
 * @return      EEPROM_25LC256_OK on success   
 *
 * @attention   Prior to any attempt to write data to the 25LC256, the
 *              write enable latch must be set by issuing the WREN instruction
 *              (call to eeprom25LC256_WriteEnable function)
 */
eeprom25lc256_err_t eeprom25LC256_WriteEnable(const eeprom25lc256_desc_t *pdev);

/**
 * @brief       Clears the Write Enable Latch (disables writings)
 * 
 * @param[in]   pDev    Address of the device descriptor
 * 
 * @return      EEPROM_25LC256_OK on success   
 *
 * @attention   Prior to any attempt to write data to the 25LC256, the
 *              write enable latch must be set by issuing the WREN instruction
 *              (call to eeprom25LC256_WriteEnable function)
 */
eeprom25lc256_err_t eeprom25LC256_WriteDisable(const eeprom25lc256_desc_t *pdev);


/**
 * @brief       Write Data to the EEPROM
 * 
 * @param[in]   pDev    Address of the device descriptor
 * @param       StartAddress    Address of the first Byte to write	
 * @param[in]   pData   Address of the data to be written
 * @param       Length  Number of data bytes to write       
 * 
 * @return      EEPROM_25LC256_OK on success
 * @return      EEPROM_25LC256_BAD_ADDRESS_ERROR if the address is > 0x7FFF
 * @return      EEPROM_25LC256_PAYLOAD_ERROR Length is > 64
 * 
 * @attention   64 octets Max
 * 
 * @attention   Prior to any attempt to write data to the 25LC256, the
 *              write enable latch must be set by issuing the WREN instruction
 *              (call to eeprom25LC256_WriteEnable function)
 */
eeprom25lc256_err_t eeprom25LC256_Write(const eeprom25lc256_desc_t *pdev,
                                        uint16_t StartAddress,
                                        const uint8_t *pData,
                                        size_t  Length);

/**
 * @brief       Read Data from the EEPROM
 * 
 * @param[in]   pDev    Address of the device descriptor
 * @param       StartAddress    Address of the first Byte to read
 * @param[out]  pData   Address of the read data
 * @param       Length  Number of data bytes to read
 * 
 * @return      EEPROM_25LC256_OK on success
 * @return      EEPROM_25LC256_BAD_ADDRESS_ERROR if the address is > 0x7FFF    
 */
eeprom25lc256_err_t eeprom25LC256_Read( const eeprom25lc256_desc_t *pdev,
                                        uint16_t StartAddress,
                                        uint8_t *pData,
                                        size_t  Length);

#endif  /*  __LIB_25LC256_H__   */

