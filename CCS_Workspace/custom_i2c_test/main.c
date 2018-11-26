#include <msp.h>
#include <stdio.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/**
 * main.c
 */

typedef struct
{
    uint_fast8_t selectClockSource;
    uint32_t i2cClk;
    uint32_t dataRate;
    uint_fast8_t byteCounterThreshold;
    uint_fast8_t autoSTOPGeneration;
} eUSCI_I2C_MasterConfig;

#define SLAVE_ADDRESS                               0x48

#define PORT                                        1
#define LED_PIN                                     0x0001
#define SDA_PIN                                     0x0040
#define SCL_PIN                                     0x0080


#define EUSCI_B_I2C_CLOCKSOURCE_SMCLK               ((uint16_t)0x0080)
#define EUSCI_B_I2C_SET_DATA_RATE_100KBPS           100000
#define EUSCI_B_I2C_NO_AUTO_STOP                    ((uint16_t)0x0000)

#define EUSCI_A_CMSIS(x)                            ((EUSCI_A_Type *) x)
#define EUSCI_B_CMSIS(x)                            ((EUSCI_B_Type *) x)

#define EUSCI_B_I2C_TIMEOUT_28_MS                   ((uint16_t)0x0040)

#define INT_EUSCIB0                                 (36) /* EUSCIB0 IRQ */

#define EUSCI_B_I2C_RECEIVE_INTERRUPT0              ((uint16_t)0x0001)
#define EUSCI_B_I2C_CLOCK_LOW_TIMEOUT_INTERRUPT     ((uint16_t)0x0080)


/* System exceptions */
#define FAULT_NMI                                       ( 2) /* NMI fault */
#define FAULT_HARD                                      ( 3) /* Hard fault */
#define FAULT_MPU                                       ( 4) /* MPU fault */
#define FAULT_BUS                                       ( 5) /* Bus fault */
#define FAULT_USAGE                                     ( 6) /* Usage fault */
#define FAULT_SVCALL                                    (11) /* SVCall */
#define FAULT_DEBUG                                     (12) /* Debug monitor */
#define FAULT_PENDSV                                    (14) /* PendSV */
#define FAULT_SYSTICK                                   (15) /* System Tick */

#define NVIC_EN0_R              0xE000E100  // Interrupt 0-31 Set Enable
#define NVIC_EN1_R              0xE000E104  // Interrupt 32-54 Set Enable


#define EUSCI_B_I2C_STOP_INTERRUPT                             EUSCI_B_IE_STPIE
#define EUSCI_B_I2C_START_INTERRUPT                            EUSCI_B_IE_STTIE

#define NUM_OF_REC_BYTES                    10


static const uint32_t g_pulEnRegs[] =
{ NVIC_EN0_R, NVIC_EN1_R };

/* Variables */
const uint8_t TXData[] = {0x04};
static uint8_t RXData[NUM_OF_REC_BYTES];
static volatile uint32_t xferIndex;
static volatile bool stopSent;
unsigned int i=0, timeout=0;

static const uint32_t GPIO_PORT_TO_BASE[] =
{   0x00,
    (uint32_t)P1,
    (uint32_t)P1+1,
    (uint32_t)P3,
    (uint32_t)P3+1,
    (uint32_t)P5,
    (uint32_t)P5+1,
    (uint32_t)P7,
    (uint32_t)P7+1,
    (uint32_t)P9,
    (uint32_t)P9+1,
    (uint32_t)PJ
};

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000,                                   // SMCLK = 3kHz
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};


int get_temp()
{
    return 0;
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	uint_fast8_t selectedPort = PORT;   //Initialize Port
	uint_fast16_t ledPin = LED_PIN;     //Initialize LED pin

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + ((uint32_t)&P1->SEL0 - (uint32_t)P1)) &= ~ledPin;
    HWREG16(baseAddress + ((uint32_t)&P1->SEL1 - (uint32_t)P1)) &= ~ledPin;
    HWREG16(baseAddress + ((uint32_t)&P1->DIR - (uint32_t)P1)) |= ledPin;

    //----------------------------------------------------------------------
    //I2C Section
    //----------------------------------------------------------------------

    uint_fast16_t i2cPins = SDA_PIN + SCL_PIN;  //Initialize I2C pins

    HWREG16(baseAddress + OFS_PADIR) &= ~i2cPins;
    HWREG16(baseAddress + OFS_PASEL0) |= i2cPins;
    HWREG16(baseAddress + OFS_PASEL1) &= ~i2cPins;

    stopSent = false;
    memset(RXData, 0x00, NUM_OF_REC_BYTES);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
//    I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);



    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    uint32_t moduleInstance = EUSCI_B0_BASE;
    const eUSCI_I2C_MasterConfig *config = &i2cConfig;

    uint_fast16_t preScalarValue;

    /* Disable the USCI module and clears the other bits of control register */
    BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) =
            1;

    /* Configure Automatic STOP condition generation */
    EUSCI_B_CMSIS(moduleInstance)->CTLW1 = (EUSCI_B_CMSIS(moduleInstance)->CTLW1
            & ~EUSCI_B_CTLW1_ASTP_MASK) | (config->autoSTOPGeneration);

    /* Byte Count Threshold */
    EUSCI_B_CMSIS(moduleInstance)->TBCNT = config->byteCounterThreshold;

    /*
     * Configure as I2C master mode.
     * UCMST = Master mode
     * UCMODE_3 = I2C mode
     * UCSYNC = Synchronous mode
     */
    EUSCI_B_CMSIS(moduleInstance)->CTLW0 = (EUSCI_B_CMSIS(moduleInstance)->CTLW0
            & ~EUSCI_B_CTLW0_SSEL_MASK)
            | (config->selectClockSource | EUSCI_B_CTLW0_MST
                    | EUSCI_B_CTLW0_MODE_3 | EUSCI_B_CTLW0_SYNC
                    | EUSCI_B_CTLW0_SWRST);

    /*
     * Compute the clock divider that achieves the fastest speed less than or
     * equal to the desired speed.  The numerator is biased to favor a larger
     * clock divider so that the resulting clock is always less than or equal
     * to the desired clock, never greater.
     */
    preScalarValue = (uint16_t) (config->i2cClk / config->dataRate);

    EUSCI_B_CMSIS(moduleInstance)->BRW = preScalarValue;

    /* Set clock low timeout, interrupt triggered if slave hold SCL>28ms */
    moduleInstance = EUSCI_B0_BASE;
    uint_fast16_t timeout = EUSCI_B_I2C_TIMEOUT_28_MS;

    uint_fast16_t swrstValue;

    /* Save value of UCSWRST bit and disable USCI module */
    swrstValue = BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_SWRST_OFS);
    BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) =
            1;

    /* Set timeout */
    EUSCI_B_CMSIS(moduleInstance)->CTLW1 = (EUSCI_B_CMSIS(moduleInstance)->CTLW1
            & (~EUSCI_B_CTLW1_CLTO_3)) | timeout;

    /* Restore value of UCSWRST bit */
    BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) = swrstValue;
//
    /* Specify slave address */
    moduleInstance = EUSCI_B0_BASE;
    uint_fast16_t slaveAddress = SLAVE_ADDRESS;
    /* Set the address of the slave with which the master will communicate */
    EUSCI_B_CMSIS(moduleInstance)->I2CSA = (slaveAddress);

    //--------------------------------------------------------------------------------------------------
    /* Enable I2C Module to start operations */
    moduleInstance = EUSCI_B0_BASE;
    /* Reset the UCSWRST bit to enable the USCI Module */
    BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) = 0;
    //--------------------------------------------------------------------------------------------------
    uint32_t interruptNumber = INT_EUSCIB0;
    //
    // Determine the interrupt to enable.
    //
    if (interruptNumber == FAULT_MPU)
    {
        //
        // Enable the MemManage interrupt.
        //
        SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    } else if (interruptNumber == FAULT_BUS)
    {
        //
        // Enable the bus fault interrupt.
        //
        SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    } else if (interruptNumber == FAULT_USAGE)
    {
        //
        // Enable the usage fault interrupt.
        //
        SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
    } else if (interruptNumber == FAULT_SYSTICK)
    {
        //
        // Enable the System Tick interrupt.
        //
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    } else if (interruptNumber >= 16)
    {
        //
        // Enable the general interrupt.
        //
        HWREG32 (g_pulEnRegs[(interruptNumber - 16) / 32]) = 1
                << ((interruptNumber - 16) & 31);
    }
    //--------------------------------------------------------------------------------------------------
    moduleInstance = EUSCI_B0_BASE;
    uint_fast16_t mask = EUSCI_B_I2C_RECEIVE_INTERRUPT0;

    //Enable the interrupt masked bit
    EUSCI_B_CMSIS(moduleInstance)->IE |= mask;
    //--------------------------------------------------------------------------------------------------
    moduleInstance = EUSCI_B0_BASE;
    mask = EUSCI_B_I2C_CLOCK_LOW_TIMEOUT_INTERRUPT;

    //Enable the interrupt masked bit
    EUSCI_B_CMSIS(moduleInstance)->IE |= mask;
    //--------------------------------------------------------------------------------------------------

    /* Making sure the last transaction has been completely sent out */
    moduleInstance = EUSCI_B0_BASE;
    while (BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS));
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

//    /* Send start and the first byte of the transmit buffer. */
//    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, TXData[0]);
//
//    /* Sent the first byte, now we need to initiate the read */
//    xferIndex = 0;
//    MAP_I2C_masterReceiveStart(EUSCI_B0_BASE);
//
    /* Send start and the first byte of the transmit buffer. */
    moduleInstance = EUSCI_B0_BASE;
    uint8_t txData = TXData[0];

    //Store current transmit interrupt enable
    uint16_t txieStatus = EUSCI_B_CMSIS(moduleInstance)->IE & EUSCI_B_IE_TXIE0;

    //Disable transmit interrupt enable
    BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->IE, EUSCI_B_IE_TXIE0_OFS) = 0;

    //Send start condition.
    EUSCI_B_CMSIS(moduleInstance)->CTLW0 |= EUSCI_B_CTLW0_TR
          + EUSCI_B_CTLW0_TXSTT;

    //Poll for transmit interrupt flag and start condition flag.
    while (BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0,
              EUSCI_B_CTLW0_TXSTT_OFS)
              || !BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->IFG,
                      EUSCI_B_IFG_TXIFG0_OFS));

    //Send single byte data.
    EUSCI_B_CMSIS(moduleInstance)->TXBUF = txData;

    //Reinstate transmit interrupt enable
    EUSCI_B_CMSIS(moduleInstance)->IE |= txieStatus;

    /* Sent the first byte, now we need to initiate the read */
    moduleInstance = EUSCI_B0_BASE;

    xferIndex = 0;
    //Set USCI in Receive mode
//    EUSCI_B_CMSIS(moduleInstance)->rCTLW0.r = (EUSCI_B_CMSIS(moduleInstance)->rCTLW0.r & (~UCTR)) | UCTXSTT;
    moduleInstance = EUSCI_B0_BASE;
    EUSCI_B_CMSIS(moduleInstance)->CTLW0 = (EUSCI_B_CMSIS(moduleInstance)->CTLW0 & (~EUSCI_B_CTLW0_TR)) | EUSCI_B_CTLW0_TXSTT;


    while(1){
	    // Blink LED on finish
	    HWREG16(baseAddress + OFS_PAOUT) ^= ledPin;
        __delay_cycles(300000);

	}
}

void EUSCIB0_IRQHandler(void)
{
    uint_fast16_t status;
    uint32_t moduleInstance = EUSCI_B0_BASE;
    uint16_t mask = EUSCI_B_CMSIS(moduleInstance)->IE;
    status = EUSCI_B_CMSIS(moduleInstance)->IFG & mask;

    mask = status;
    EUSCI_B_CMSIS(moduleInstance)->IFG &= ~(mask);

    /* Check for clock low timeout and increment timeout counter */
    if (status & EUSCI_B_I2C_CLOCK_LOW_TIMEOUT_INTERRUPT)
    {
        timeout++;
    }

    /* Receives bytes into the receive buffer. If we have received all bytes,
     * send a STOP condition */
    if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
    {
        if (xferIndex == NUM_OF_REC_BYTES - 2)
        {
            mask = EUSCI_B_I2C_RECEIVE_INTERRUPT0;
            //Disable the interrupt masked bit
            EUSCI_B_CMSIS(moduleInstance)->IE &= ~(mask);

            mask = EUSCI_B_I2C_STOP_INTERRUPT;
            EUSCI_B_CMSIS(moduleInstance)->IE |= mask;

            /*
             * Switch order so that stop is being set during reception of last
             * byte read byte so that next byte can be read.
             */
            //Send stop condition.
            BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS) =1;
            RXData[xferIndex++] = (EUSCI_B_CMSIS(moduleInstance)->RXBUF & EUSCI_B_RXBUF_RXBUF_MASK);
        } else
        {

            RXData[xferIndex++] = (EUSCI_B_CMSIS(moduleInstance)->RXBUF & EUSCI_B_RXBUF_RXBUF_MASK);
        }
    }
    else if (status & EUSCI_B_I2C_STOP_INTERRUPT)
    {
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
        mask = EUSCI_B_I2C_STOP_INTERRUPT;
        //Disable the interrupt masked bit
            EUSCI_B_CMSIS(moduleInstance)->IE &= ~(mask);
    }
}

