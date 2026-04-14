/**
 * @file  
 * @author 	Alexis ROLLAND
 * @date	2024-05
 * @brief 	
 *  
 *
 */


#include "lib_test_max31855.h" // Inclusion du fichier .h "Applicatif" renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/
max31855_desc_t MyMax31855;
spi_desc_t      SpiModule;

/*	Implémentation du code */
void Initialiser(void){
    max31855_config_t   maxCfg;

    // Leds
    TRISA &= 0xFF00;
    LATA = 0;

    maxCfg.pSpi = &SpiModule;
    maxCfg.initType = INIT_WITH_SPI2;   /**< SPI2 Module will be used   */
    maxCfg.MaxCS.port = GPIO_PORTG;     /**< CSA (MikroBus A - CS line) is on RG9   */
    maxCfg.MaxCS.bitNumber = 9;         /**< CSA (MikroBus A - CS line) is on RG9   */

    max31855_init (&maxCfg,&MyMax31855);




}

