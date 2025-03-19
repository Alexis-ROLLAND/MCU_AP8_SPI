/**
 * @file    lib_max31855.c 
 * @author 	Alexis ROLLAND
 * @date	2022-03
 * @brief 	lib for the MAX31855 Cold-Junction Compensated
 *          SPI Thermocouple-to-Digital Converter  
 *          V2 : 2024-05
 */

#include "lib_max31855.h" // Inclusion du fichier .h  renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/
spi_cs_t   MaxCS;
uint8_t    RxBuffer[4];

/*	Implémentation du code */
max31855_err_t max31855_init (const max31855_config_t *pMax31855CFG, max31855_desc_t *pMax31855){
    spi_config_t    spiCfg;

    // SPI Config
    if (pMax31855CFG->initType != INIT_ALREADY_DONE){
        spiCfg.spiClockPolarity = CLK_IDLE_IS_LOW;
        spiCfg.spiClockPhase = IDLE_TO_ACTIVE_CPHASE;
        spiCfg.spiDataFormat = BITS8;
        spiCfg.spiSamplePoint = MID_SMP;
        spiCfg.spiPrimaryPrescaler = PRI_PRE_4;
        spiCfg.spiSecondaryPrescaler = SEC_PRE_8;
        switch(pMax31855CFG->initType){
            case INIT_WITH_SPI1:
                spi_init(_SPI1, &spiCfg, pMax31855CFG->pSpi);
                break;
            case INIT_WITH_SPI2:
                spi_init(_SPI2, &spiCfg, pMax31855CFG->pSpi);
                break;
            default: break;
            }
        }
    pMax31855->pSpi = pMax31855CFG->pSpi;

    // CS Pin
    MaxCS = pMax31855CFG->MaxCS;
    pMax31855->pMaxCS = &MaxCS;
    // Configure & init CS Line
    spi_init_cs(pMax31855->pSpi, pMax31855->pMaxCS);

    return MAX31855_OK;
}

//------------------------------------------------------------------------------
max31855_err_t max31855_read_data(const max31855_desc_t *pMax31855, max31855_data_t *pData){

    uint16_t    temp;
    float   ftemp;

    spi_assertCS(pMax31855->pSpi, pMax31855->pMaxCS);
    spi_transfer_raw_bytes(pMax31855->pSpi, NULL, RxBuffer, MAX31855_DATA_SIZE);
    spi_deassertCS(pMax31855->pSpi, pMax31855->pMaxCS);

    // decode data : faults bits
    pData->sensorStatus = RxBuffer[3] & 0x0007;
    if (pData->sensorStatus != 0){
        pData->rawInternalTemp = 0;
        pData->rawSensorTemp = 0;
        pData->InternalTemp = 0;
        pData->SensorTemp = 0;
        return MAX31855_ERROR;
    }

    // Decode Data - Sensor Temp
    temp = (uint8_t)RxBuffer[0];
    temp <<= 8;
    temp += (uint8_t)RxBuffer[1];
    temp >>= 2;
    pData->rawSensorTemp = temp;

    // Decode Data - Internal temp
    temp = (uint8_t)RxBuffer[2];
    temp <<= 8;
    temp += (uint8_t)RxBuffer[3];
    temp >>= 4;
    pData->rawInternalTemp = temp;

    // Decode data - convert to float
    // Sensor temp
    if (pData->rawSensorTemp < 0x2000){     // sensor temp is >=0
        ftemp = (float)pData->rawSensorTemp * TEMP_SENSOR_LSB;
        pData->SensorTemp = ftemp;
    }
    else
    {
        temp = ~pData->rawSensorTemp;    // get C1 value of the 16bits data
        temp &= 0x1FFF;  // clear b[15:13] -> C1 value of the 14 bits data
        temp += 1;       // get the C2 value
        ftemp = -(TEMP_SENSOR_LSB)*(float)temp;
        pData->SensorTemp = ftemp;        
    }

    //  Internal temp
    if (pData->InternalTemp < 0x0100){     // internal temp is >=0
        ftemp = (float)pData->rawInternalTemp * TEMP_INTERNAL_LSB;
        pData->InternalTemp = ftemp;
    }
    else
    {
        temp = ~pData->rawInternalTemp;    // get C1 value of the 16bits data
        temp &= 0x07FF;  // clear b[15:11] -> C1 value of the 12 bits data
        temp += 1;       // get the C2 value
        ftemp = -(TEMP_INTERNAL_LSB)*(float)temp;
        pData->InternalTemp = ftemp;        
    }

    return MAX31855_OK;
}
//------------------------------------------------------------------------------