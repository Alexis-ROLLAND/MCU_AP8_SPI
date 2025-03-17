/**
 * @file    lib_test_25lc256.h 
 * @author 	Alexis ROLLAND
 * @date	2024 04
 * @brief 	Header file for lib_test_25lc256 (test App)
 *  
 *
 */
#ifndef	__LIB_TEST_25LC256_H__
#define	__LIB_TEST_25LC256_H__

#ifndef FCY
#define FCY 4000000UL
#endif

#include <libpic30.h>
#include <xc.h>

#define     TEST_WREN_WRDIS     1
#define     TEST_READ_SR        2
#define     TEST_WRITE_AND_READ 3

#define     CURRENT_TEST    TEST_WREN_WRDIS



#define LED         LATAbits.LATA0
#define TRIS_LED    TRISAbits.TRISA0
#define ErrorHandler()  {LED = ~LED;__delay_ms(25);}

/**
 * @brief Global init function/task 
 * 
 * @param	None
 * 
 * @return  Nothing 
 *
 */
void Initialiser(void);

/**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
void mainTask(void);
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */


 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 
 


#endif  /*  __LIB_TEST_25LC256_H__  */



