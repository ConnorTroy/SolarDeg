#include "driverConfig.h"
#include <stdlib.h>
//#include "printf.h"

/**
 * main.c
 *
 * Connor Troy
 * Power Haus - Solar Soaker - Solar Degradation Unit
 * 12/2/2018
 *
 * This project is a testing environment for the various drivers we will create.
 *
 */



void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

/*
 * ===================================================================================================
*                                                  I2C
 * ===================================================================================================
 */

    I2C_init(I2C_MODULE, &i2cConfig);
    I2C_enable(I2C_MODULE);

//    INA219_updateConfig(&powerSensorConfig, 0x0FFF);
//    uint16_t con = INA219_readConfigReg(&powerSensorConfig);

    INA219_reset(&powerSensorConfig);
//    con = INA219_readConfigReg(&powerSensorConfig);

    INA219_writeCalibrationReg(&powerSensorConfig, INA219_calculate_calibration(&powerSensorConfig));
    uint16_t cal = INA219_readCalibrationReg(&powerSensorConfig);

    uint16_t con = INA219_readConfigReg(&powerSensorConfig);
//    uint16_t cal = INA219_readCalibrationReg(&powerSensorConfig);

/*
 * ===================================================================================================
*                                                  PWM
 * ===================================================================================================
 */

    PCA_enable_output(I2C_MODULE);

/*
 * ===================================================================================================
 *                                               Uart
 * ===================================================================================================
 */


    uart_init(UART_MODULE, &uartConfig);
    uart_enable(UART_MODULE);

    NVIC_EnableIRQ(EUSCIA2_IRQn);   // Enable eUSCIA0 interrupt in NVIC


    memset(UART_RX_BUF_MULTIBYTE, '-', UART_RX_BUF_LEN);

    poll_for_ext_connect( UART_MODULE, UART_RX_BUF_MULTIBYTE, UART_RX_BUF_LEN );


/*
 * ===================================================================================================
 *                                              Test Run
 * ===================================================================================================
 */


//    while(1)
//    {
////        uart_send_multiple( UART_MODULE, "Hello\n", 6 );
//        float current = INA219_readCurrent(&powerSensorConfig);
//        current = current < powerSensorConfig.max_current ? current : 0;
//        char iLine[11];
//        memset(iLine, ' ', 11);
//        ftoa(current, iLine, 7);
//        iLine[10] = 'A';
//        uart_send_multiple( UART_MODULE, (uint8_t*)iLine, sizeof(iLine));
//        uart_send_byte( UART_MODULE, '\n');
//        uart_send_byte( UART_MODULE, '\n');
//        __delay_cycles(1000000);
//
//    }



    PCA_reset(I2C_MODULE);

    uint16_t Sweep_Pins[] = {BIT0, BIT1, BIT2, BIT3, BIT4, BIT5};

    int c;
    for (c = 0; c < sizeof(Sweep_Pins); c++)
    {
        P2->SEL0 &= ~Sweep_Pins[c];
        P2->SEL1 &= ~Sweep_Pins[c];
        P2->DIR |= Sweep_Pins[c];
        P2->OUT &= ~Sweep_Pins[c];
    }


//    poll_for_ext_connect( UART_MODULE, UART_RX_BUF_MULTIBYTE, UART_RX_BUF_LEN );

    int dutyCycleRange = 256/2;
    float* V_arr;
    V_arr = malloc(dutyCycleRange * sizeof(float));
    float* I_arr;
    I_arr = malloc(dutyCycleRange * sizeof(float));
    memset(V_arr, 0, dutyCycleRange);
    memset(I_arr, 0, dutyCycleRange);


    int AA, dutyCycle;
    // Send Start Flag
    ext_send_start( UART_MODULE );

    uart_send_byte( UART_MODULE, '0'); // Cell 0
    uart_send_byte( UART_MODULE, '\n');

    uart_send_byte( UART_MODULE, '-'); // Active Area
    uart_send_byte( UART_MODULE, '\n');



    for (dutyCycle = 0; dutyCycle <= dutyCycleRange; ++dutyCycle)
    {


        // Update Duty Cycle
        PCA_change_duty(I2C_MODULE, 10, dutyCycle);
        __delay_cycles(100000);
        // TIMER_A0 -> CCR[1] = dutyCycle;

        for (AA = 0; AA < 6; ++AA)
        {

            // Switch Active Area
            P2->OUT = Sweep_Pins[AA];
            __delay_cycles(10000);


            // Read INA
            float current = INA219_readCurrent(&powerSensorConfig);
            current = current < powerSensorConfig.max_current ? current : 0;
            V_arr[AA] += dutyCycle;
            I_arr[AA] += current;

            // Send Data
            if (AA == 2)
            {
                char vLine[6];
                memset(vLine, ' ', 6);
                ftoa((5 * (float)dutyCycle/256), vLine, 2);
                uart_send_multiple( UART_MODULE, (uint8_t*)vLine, sizeof(vLine));
                uart_send_byte( UART_MODULE, ' ');

                char iLine[11];
                memset(iLine, ' ', 11);
                ftoa(current, iLine, 10);
                uart_send_multiple( UART_MODULE, (uint8_t*)iLine, sizeof(iLine));
                uart_send_byte( UART_MODULE, '\n');
            }
        }

    }
    ext_send_end( UART_MODULE );

    P2->OUT = 0;

    // Calculate MPP
    float* V_I_arr;
    V_I_arr  = malloc(dutyCycleRange * sizeof(float));
    memset(V_I_arr, 0, dutyCycleRange);

    int MPP_Index = 0;
    float MPP = 0;

    int i;
    for (i = 0; i < dutyCycleRange; i++)
    {
        V_arr[i] /= 6;
        I_arr[i] /= 6;
        V_I_arr[i] = V_arr[i] * I_arr[i];
        if (V_I_arr[i] > MPP)
        {
            MPP = V_I_arr[i];
            MPP_Index = i;
        }

    }
    free(V_arr);
    free(I_arr);
    free(V_I_arr);

    // Set MPP
    PCA_reset(I2C_MODULE);
    PCA_change_duty(I2C_MODULE, 9, MPP_Index);





    while(1){}

}


// Inputs UART Receive data to UART Buffer
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = EUSCI_A2 -> IFG;

    if(status & EUSCI_A_IFG_RXIFG)
    {
        EUSCI_A2 -> IFG &= ~(EUSCI_A_IFG_RXIFG);
        uart_receive(EUSCI_A2, UART_RX_BUF_MULTIBYTE, UART_RX_BUF_LEN);
    }
}

