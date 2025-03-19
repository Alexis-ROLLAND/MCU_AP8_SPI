/**
 * @file    lib_max31855.h 
 * @author 	Alexis ROLLAND
 * @date	2024-04
 * @brief 	lib for the MAX31855 Cold-Junction Compensated
 *          SPI Thermocouple-to-Digital Converter 
 *  
 */

#ifndef	__LIB_MAX31855_H__
#define	__LIB_MAX31855_H__
#include <xc.h>
#include "lib_spi_pic24_ll.h"

#define     MAX31855_DATA_SIZE  4

typedef enum    {   MAX31855_OK,        /**< Succes value                   */
                    MAX31855_ERROR      /**< Non Specific Error             */
} max31855_err_t;

typedef enum    {   INIT_WITH_SPI1,     /**< Initialize with SPI1 scheme    */
                    INIT_WITH_SPI2,     /**< Initialize with SPI2 scheme    */
                    INIT_ALREADY_DONE   /**< do not initialize SPI module   */
} max31855_spi_init_type_t;


typedef struct {
    max31855_spi_init_type_t    initType;   /**< Initializing scheme (with SPI1, with SPI2 or init already done) */
    spi_desc_t  *pSpi;                      /**< Address of the SPI descriptor (initialized or not)    */
    spi_cs_t    MaxCS;                      /**< Chip Select    */
} max31855_config_t;

typedef struct  {
    spi_desc_t  *pSpi;          /**< Fully initialized SPI descriptor (adrress)   */
    spi_cs_t    *pMaxCS;        /**< Address of the real "CS pin" object    */
} max31855_desc_t; 

#define TEMP_SENSOR_LSB     (float)0.25
#define TEMP_INTERNAL_LSB   (float)0.0625    

typedef struct   {
    uint16_t rawInternalTemp;   /**< In LSB (1LSB = 0.0625°C) - 12 bit signed data      */
    uint16_t rawSensorTemp;     /**< In LSB (1LSB = 0.25°C)   - 14 bit signed data      */
    float    InternalTemp;      /**< Internal temperature, in °C, float format          */
    float    SensorTemp;        /**< Sensor Temp, in °C, float format                   */  
    uint8_t  sensorStatus;      /**< 0 means no error. bit 0 set means "Open Circuit", bit 1 set means "Short to Ground", bit 2 set means "Short to Vcc"  */
} max31855_data_t;

/**
 * @brief       Initializes the Max31855 device descriptor
 * 
 * @param[in]	pMax31855CFG    Address of a max31855 init structure
 * @param[out]  pMax31855       Address of a max31855 device descriptor	
 * 
 * @return      MAX31855_OK
 */
 max31855_err_t max31855_init (const max31855_config_t *pMax31855CFG, max31855_desc_t *pMax31855);


 /**
 * @brief       Reads data from the MAX31855 device, throught the device descriptor  
 * 
 * @param[in]   pMax31855   Address of a fully configured MAX31855 device descriptor        
 * @param[out]  pDataAddress of the max31855_data_t structure variable    
 * 
 * @return      MAX31855_OK or MAX31855_ERROR if at least one of the fault bits is set (SCV, SVG or OC) 
 *
 */
 max31855_err_t max31855_read_data(const max31855_desc_t *pMax31855, max31855_data_t *pData);


#endif  /*  __LIB_MAX31855_H__  */