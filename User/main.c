/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Randy Bancroft
 * Version            : V1.0.0
 * Date               : 2026/02/07
 * Description        : Main program body.
*********************************************************************************
* This code is based on WCH's UDP server example
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/*
 *@Note
For details on the selection of engineering chips,
please refer to the "CH32V30x Evaluation Board Manual" under the CH32V307EVT\EVT\PUB folder.
 * - This code was cobbled together by Randy S. Bancroft, a.k.a. CannonFodderSE 
 *   (https://github.com/CannonFodderSE) using various examples from openwch's git
 *   hub page at https://github.com/openwch/ch32v307 with assistance from ChatGPT.  
 *   I am not a professional programmer. I went to Google and Youtube universities :)
 *   So forgive any nonstandard programming.
 * - It's purpose is to interface hardware (switches, knobs, etc..) from DIY 
 *   cockpit panels for use with Dynamic Eagle's Digital Combat Simulator (DCS)
 *   flight simumlator (see https://www.digitalcombatsimulator.com) via DCS-BIOS
 *   (see https://github.com/DCS-Skunkworks/dcs-bios and 
 *   https://gitmemories.com/DCS-Skunkworks/dcs-bios).
 * - Communication is UDP protocol. This is transmitted via an integrated 
 *   ethernet port on the MCU.
 * - Hardware used is a CH32V307VCT6 RISC-V Development Board as seen on AliExpress
 *   at https://www.aliexpress.us/item/3256809231229507.html.
 * - This is different that the CH32V307VCT6 dev boards from WCH. See example at
 *   https://www.amazon.com/MARALANG-Evaluation-CH32V307VCT6-CH32V307V-EVT-R1-500709463/dp/B0D768JL1N.
 * - Program should work with any CH32V307VCT6 dev boards with maybe some pin reassignements.
 *
 * GPIO pin notes:
 *          -Ethernet uses GPIO pins PC0 (LINK LED), PC1 (ACT LED), PC6 (RXP), 
 *           PC7 (RXN), PC8 (TXP) and PC9 (TXN).  Solder bridges have to be closed
 *           on the bottom of the board for these to function.
 *          -These are configured by the ehternet driver
 *          -Printf statements send serial via GPIO pin PA9 (SUART1 TX)
 *          -Programmer/debugger uses GPIO pins PA13 (SWDIO) and PA14 (SWCLK)
 *          -LEDs use GPIO pins PA15 (LED1 red) and PB4 (LED2 green)
 *          -Power LED does not use ay GPIO pins
 *          -If associated solder bridges are shorted, microSD TF card reader
 *           used GPIO pins PC8 (DAT0), PC9 (DAT1), PC10 (DAT2), PC11 (CD/DAT3),
 *           PC12 (CLK), PD2 (CMD) and PD7 (TFSW).  PC8 and PC9 are shared with
 *           ethernet port.  
 *          -FT24C32A-ETR-T (33kbit EEPROM) memory uses GPIO pins 
 *           PB10 (SCL) and PB11 (SDA)
 *          -Flash memory (W25Qxxx) uses GPIO pins PA2 (CS), PA5 (CLK), 
 *           PA6 (DO/IO1) and PA7 (DI/IO0)
 *          -USB port 1 uses GPIO pins PA11 (DN1/DN2) and PA12 (DP1/DP2)
 *          -USB port 2 uses GPIO pins PB6 (DN1/DN2) and PB7 (DP1/DP2)
 *          -32.768kHz crystal uses GPIO pins PC14 (OSC32IN) and 
 *          -PC15 (OSC32OUT)
 *          -User button uses GPIO pin PB3
 *          -GPIO pin PB2 (BOOT1) is held low with an external 10k resistor.
 *           It has a open solder bridge that can be shorted to pull the port
 *           high.
 *
 *          See table below for easier visualization.  Blank square are free
 *          to use GPIO pins.
 *    ______________________________________________________________________________
 *    |                                   BANK                                     |
 * ---------------------------------------------------------------------------------
 * PIN|         A           |          B          |           C         |   D  | E |
 * ---------------------------------------------------------------------------------
 *  0 |                     |                     | ETHNET LINK LED     |      |   |
 * ---------------------------------------------------------------------------------
 *  1 |                     |                     | ETHNET ACT LED      |      |   |
 * ---------------------------------------------------------------------------------
 *  2 | FLASH-MEMORY CS     | BOOT BUTTON BOOT1   |                     | CMD  |   |
 * ---------------------------------------------------------------------------------
 *  3 |                     | USER BUTTON         |                     |      |   |
 * ---------------------------------------------------------------------------------
 *  4 |                     | LED2 GRN            |                     |      |   |
 * ---------------------------------------------------------------------------------
 *  5 | FLASH-MEMORY CLK    |                     |                     |      |   |
 * ---------------------------------------------------------------------------------
 *  6 | FLASH-MEMORY DO/IO1 | USB2 DN1/DN2        | ETHNET RXP          |      |   |
 * ---------------------------------------------------------------------------------
 *  7 | FLASH-MEMORY DI/IO0 | USB2 DP1/DP2        | ETHNET RXN          | TFSW |   |
 * ---------------------------------------------------------------------------------
 *  8 |                     |                     | ETHNET TXP          |      |   |
 *    |                     |                     | SD CARD TF DAT0     |      |   |
 * ---------------------------------------------------------------------------------
 *  9 | SUART1 TX           |                     | ETHNET TXN          |      |   |
 *    |                     |                     | SD CARD TF DAT1     |      |   |
 * ---------------------------------------------------------------------------------
 * 10 |                     | EEPROM CLK          | SD CARD TF DAT2     |      |   |
 * ---------------------------------------------------------------------------------
 * 11 | USB1 DN1/DN2        | EEPROM SDA          | SD CARD TF CD/DAT3  |      |   |
 * ---------------------------------------------------------------------------------
 * 12 | USB1 DP1/DP2        |                     | SD CARD TF CLK      |      |   |
 * ---------------------------------------------------------------------------------
 * 13 | SWDIO               |                     |                     |      |   |
 * ---------------------------------------------------------------------------------
 * 14 | SWCLK               |                     | 32KHZ XTAL OSC32IN  |      |   |
 * ---------------------------------------------------------------------------------
 * 15 | LED1 RED            |                     | 32KHZ XTAL OSC32OUT |      |   |
 * ---------------------------------------------------------------------------------
 * 
 * Reserved pins that cannot be configured are listed in gpio_mgr.c.
 */

#include "string.h"
#include "eth_driver.h"
#include "config.h"
#include "gpio_mgr.h"
#include "timebase.h"

/* Networking */
#define UDP_RECE_BUF_LEN                        1472
#define MY_UDP_BUF_LEN                          45

u8 MACAddr[6];                                        //MAC address
u16 srcport = 1000;                                   //source port
u16 BroadcastPort = 7778;                             //Broadcast port
u8 IPAddr[4] = {192, 168, 0, 10};                     //IP address
u8 GWIPAddr[4] = {192, 168, 0, 1};                    //Gateway IP address
u8 IPMask[4] = {255, 255, 255, 0};                    //subnet mask
u8 BroadcastAddr[4] = {192, 168, 0, 255};             //Broadcast IP address
u8 SocketId;
u8 SocketMyBuf[MY_UDP_BUF_LEN] = "This is a test\n";  // socket receive buffer
u8 SocketRecvBuf[UDP_RECE_BUF_LEN];                   //socket receive buffer

/* Buttons */
typedef struct {
    uint8_t last_state;
    uint32_t last_change;
} button_state_t;

static button_state_t btn_state[MAX_BUTTONS];

/* Encoders*/
typedef struct {
    uint8_t last_state;
    uint32_t last_change;
} encoder_state_t;

static encoder_state_t enc_state[MAX_ENCODERS];

/*
 * Quadrature lookup table
 * Index = (old_state << 2) | new_state
 * +1 = CW, -1 = CCW, 0 = invalid / bounce
 */
static const int8_t quad_table[16] =
{
     0, -1, +1,  0,
    +1,  0,  0, -1,
    -1,  0,  0, +1,
     0, +1, -1,  0
};

static uint8_t read_encoder_state(int idx)
{
    uint8_t a = GPIO_ReadPin(g_cfg.encoders[idx].a_pin) ? 1 : 0;
    uint8_t b = GPIO_ReadPin(g_cfg.encoders[idx].b_pin) ? 1 : 0;
    return (a << 1) | b;
}

/* Common */
extern uint32_t millis(void);


/*********************************************************************
 * @fn      mStopIfError
 *
 * @brief   check if error.
 *
 * @param   iError - error constants.
 *
 * @return  none
 */
void mStopIfError(u8 iError)
{
    if (iError == WCHNET_ERR_SUCCESS)
        return;
    printf("Error: %02X\r\n", (u16) iError);
}

/*********************************************************************
 * @fn      TIM2_Init
 *
 * @brief   Initializes TIM2 for use with networking functions.
 *
 * @return  none
 */
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / 1000000;
    TIM_TimeBaseStructure.TIM_Prescaler = WCHNETTIMERPERIOD * 1000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/*********************************************************************
 * @fn      Calculate broadcast IP
 *
 * @brief   Calculate broadcast IP address from IP address and netmask.
 *
 * @return  none
 */
void CalcBroadcastIP(uint8_t *ip, uint8_t *mask)
{
    for (int i = 0; i < 4; i++)
    {
        BroadcastAddr[i] = ip[i] | (~mask[i]);
    }
}

/*********************************************************************
 * @fn      Send UDP packet
 *
 * @brief   Send UDP packet containing message stored in "SocketMyBuf".
 *
 * @return  none
 */
void UdpSend(){
    u32 len;
    len = sizeof(SocketMyBuf);
    
    WCHNET_SocketUdpSendTo(
        SocketId, 
        SocketMyBuf, 
        &len, 
        BroadcastAddr, 
        BroadcastPort
    );
}

/*********************************************************************
 * @fn      WCHNET_UdpServerRecv
 *
 * @brief   UDP Receive data function
 *
 *@param    socinf - socket information.
 *          ipaddr - The IP address from which the data was sent
 *          port - source port
 *          buf - pointer to the data buffer
 *          len - received data length
 * @return  none
 */
void WCHNET_UdpServerRecv(struct _SOCK_INF *socinf, u32 ipaddr, u16 port, u8 *buf, u32 len)
{
    u8 ip_addr[4], i;

    printf("Remote IP:");
    for (i = 0; i < 4; i++) {
        ip_addr[i] = ipaddr & 0xff;
        printf("%d ", ip_addr[i]);
        ipaddr = ipaddr >> 8;
    }

    printf("srcport = %d len = %d socketid = %d\r\n", port, len,
            socinf->SockIndex);

    WCHNET_SocketUdpSendTo(socinf->SockIndex, buf, &len, ip_addr, port);
    UdpSend();
}

/*********************************************************************
 * @fn      WCHNET_CreateUdpSocket
 *
 * @brief   Create UDP Socket
 *
 * @return  none
 */
void WCHNET_CreateUdpSocket(void)
{
    u8 i;
    SOCK_INF TmpSocketInf;

    memset((void *) &TmpSocketInf, 0, sizeof(SOCK_INF));
    TmpSocketInf.SourPort = srcport;
    TmpSocketInf.ProtoType = PROTO_TYPE_UDP;
    TmpSocketInf.RecvBufLen = UDP_RECE_BUF_LEN;
    TmpSocketInf.RecvStartPoint = (u32) SocketRecvBuf;
    TmpSocketInf.AppCallBack = WCHNET_UdpServerRecv;
    i = WCHNET_SocketCreat(&SocketId, &TmpSocketInf);
    printf("SocketId %d\r\n", SocketId);
    mStopIfError(i);
}

/*********************************************************************
 * @fn      WCHNET_HandleSockInt
 *
 * @brief   Socket Interrupt Handle
 *
 * @param   socketid - socket id.
 *          intstat - interrupt status
 *
 * @return  none
 */
void WCHNET_HandleSockInt(u8 socketid, u8 intstat)
{
    if (intstat & SINT_STAT_RECV)                               //receive data
    {
    }
    if (intstat & SINT_STAT_CONNECT)                            //connect successfully
    {
        printf("TCP Connect Success\r\n");
    }
    if (intstat & SINT_STAT_DISCONNECT)                         //disconnect
    {
        printf("TCP Disconnect\r\n");
    }
    if (intstat & SINT_STAT_TIM_OUT)                            //timeout disconnect
    {
        printf("TCP Timeout\r\n");
    }
}

/*********************************************************************
 * @fn      WCHNET_HandleGlobalInt
 *
 * @brief   Global Interrupt Handle
 *
 * @return  none
 */
void WCHNET_HandleGlobalInt(void)
{
    u8 intstat;
    u16 i;
    u8 socketint;

    intstat = WCHNET_GetGlobalInt();                              //get global interrupt flag
    if (intstat & GINT_STAT_UNREACH)                              //Unreachable interrupt
    {
        printf("GINT_STAT_UNREACH\r\n");
    }
    if (intstat & GINT_STAT_IP_CONFLI)                            //IP conflict
    {
        printf("GINT_STAT_IP_CONFLI\r\n");
    }
    if (intstat & GINT_STAT_PHY_CHANGE)                           //PHY status change
    {
        i = WCHNET_GetPHYStatus();
        if (i & PHY_Linked_Status)
            printf("PHY Link Success\r\n");
    }
    if (intstat & GINT_STAT_SOCKET) {                             //socket related interrupt
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++) {
            socketint = WCHNET_GetSocketInt(i);
            if (socketint)
                WCHNET_HandleSockInt(i, socketint);
        }
    }
}

/*********************************************************************
 * @fn      Button_Init
 *
 * @brief   Initialize buttons in accordance with config
 *
 * @return  none
 */
void Button_Init(void)
{
    printf("[BTN] Initializing %d buttons\r\n", g_cfg.button_count);

    for (int i = 0; i < g_cfg.button_count; i++)
    {
        GPIO_ConfigInputPullup(g_cfg.buttons[i].pin);
        btn_state[i].last_state = GPIO_ReadPin(g_cfg.buttons[i].pin);
        btn_state[i].last_change = millis();
    }
}

/*********************************************************************
 * @fn      Button_Task
 *
 * @brief   Scans GPIO pins assigned to buttons for pressed or released state
 *          Sends associated message via UDP
 *
 * @return  none
 */
void Button_Task(void)
{
    for (int i = 0; i < g_cfg.button_count; i++)
    {
        uint8_t val = GPIO_ReadPin(g_cfg.buttons[i].pin);
                            
        if (val != btn_state[i].last_state)
        {
            if ((millis() - btn_state[i].last_change) >= g_cfg.debounce_ms)
            {
                printf("[BTN] %d Press -> %d\r\n", i, val);
                btn_state[i].last_state = val;
                btn_state[i].last_change = millis();

                if (val == 0)
                {
                    printf("[BTN] %d Press -> %s\r\n",
                                i, g_cfg.buttons[i].press_text);
                    
                    memcpy(SocketMyBuf,g_cfg.buttons[i].press_text,sizeof(g_cfg.buttons[i].press_text));
                    printf("len of SocketMyBuf %d", strlen(SocketMyBuf));
                    UdpSend();
                    
                }
                else
                {
                    printf("[BTN] %d Release -> %s\r\n",
                                i, g_cfg.buttons[i].release_text);
                    
                    memcpy(SocketMyBuf,g_cfg.buttons[i].release_text,sizeof(g_cfg.buttons[i].press_text));
                    printf("len of SocketMyBuf %d", strlen(SocketMyBuf));
                    UdpSend();
                    
                }
            }
        }
    }
}

/*********************************************************************
 * @fn      Encoder_Init
 *
 * @brief   Initialize encoders in accordance with config
 *
 * @return  none
 */
void Encoder_Init(void)
{
    printf("[ENC] Initializing %d encoders\r\n", g_cfg.encoder_count);

    for (int i = 0; i < g_cfg.encoder_count; i++)
    {
        GPIO_ConfigInputPullup(g_cfg.encoders[i].a_pin);
        GPIO_ConfigInputPullup(g_cfg.encoders[i].b_pin);

        enc_state[i].last_state  = read_encoder_state(i);
        enc_state[i].last_change = millis();
    }
}

/*********************************************************************
 * @fn      Encoder_Task
 *
 * @brief   Scans GPIO pins assigned to encoders for direction of rotation
 *          Sends associated message via UDP
 *
 * @return  none
 */
void Encoder_Task(void)
{
    for (int i = 0; i < g_cfg.encoder_count; i++)
    {
        uint8_t new_state = read_encoder_state(i);
        uint8_t old_state = enc_state[i].last_state;

        if (new_state != old_state)
        {
            if ((millis() - enc_state[i].last_change) >= g_cfg.debounce_ms)
            {
                uint8_t idx = (old_state << 2) | new_state;
                int8_t dir = quad_table[idx];

                if (dir == +1)
                {
                    printf("[ENC] %d CW -> %s\r\n",
                                i, g_cfg.encoders[i].cw_text);
                    
                    memcpy(SocketMyBuf,g_cfg.encoders[i].cw_text,sizeof(g_cfg.encoders[i].cw_text));
                    printf("len of SocketMyBuf %d", strlen(SocketMyBuf));
                    UdpSend();
                    
                }
                else if (dir == -1)
                {
                    printf("[ENC] %d CCW -> %s\r\n",
                                i, g_cfg.encoders[i].ccw_text);
                    
                    memcpy(SocketMyBuf,g_cfg.encoders[i].ccw_text,sizeof(g_cfg.encoders[i].ccw_text));
                    printf("len of SocketMyBuf %d", strlen(SocketMyBuf));
                    UdpSend();
                    
                }

                enc_state[i].last_state = new_state;
                enc_state[i].last_change = millis();
            }
        }
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program
 *
 * @return  none
 */
int main(void)
{
    u8 i;

    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);  // USART initialize
    printf("\r\n\r\n\r\n\r\n\r\n[MAIN] DCS-BIOS Control Panel\r\n");
    printf("[MAIN] SystemClk:%d\r\n", SystemCoreClock);
    printf("[MAIN] ChipID:%08x\r\n", DBGMCU_GetCHIPID());
    printf("[MAIN] net version:%x\r\n", WCHNET_GetVer());
    if ( WCHNET_LIB_VER != WCHNET_GetVer()) {
        printf("[MAIN] version error.\r\n");
    }
    TIM2_Init();  // Start timers for networking functions
    
    Timebase_Init();  // Start timers for debouncing buttons and encoders

    Config_Load();    // Reads configuration as definded in "dcs_config.ini.inc"

    // Populate network info from configuration
    IPAddr[0] = g_cfg.net.ip[0] & 0xff;
    IPAddr[1] = g_cfg.net.ip[1] & 0xff;
    IPAddr[2] = g_cfg.net.ip[2] & 0xff;
    IPAddr[3] = g_cfg.net.ip[3] & 0xff;
    IPMask[0] = g_cfg.net.mask[0] & 0xff;
    IPMask[1] = g_cfg.net.mask[1] & 0xff;
    IPMask[2] = g_cfg.net.mask[2] & 0xff;
    IPMask[3] = g_cfg.net.mask[3] & 0xff;
    GWIPAddr[0] = g_cfg.net.gw[0] & 0xff;
    GWIPAddr[1] = g_cfg.net.gw[1] & 0xff;
    GWIPAddr[2] = g_cfg.net.gw[2] & 0xff;
    GWIPAddr[3] = g_cfg.net.gw[3] & 0xff;
    MACAddr[0] = g_cfg.net.mac[0] & 0xff;
    MACAddr[1] = g_cfg.net.mac[1] & 0xff;
    MACAddr[2] = g_cfg.net.mac[2] & 0xff;
    MACAddr[3] = g_cfg.net.mac[3] & 0xff;
    MACAddr[4] = g_cfg.net.mac[4] & 0xff;
    MACAddr[5] = g_cfg.net.mac[5] & 0xff;

    // Check for user assigned MAC address
    int userMac = 0;
    for(int i = 0; i < 6; i++){
        userMac += MACAddr[i];
    }
    // If user assigned MAC address is configured use it
    // Otherwise, use the chip MAC address
    if(!userMac){
        WCHNET_GetMacAddr(MACAddr);  // get the chip MAC address
    }
    printf("[MAIN] mac addr:");
    for(i = 0; i < 6; i++) 
        printf("%x ",MACAddr[i]);
    printf("\r\n");
    
    Encoder_Init();   // Initialize pins for encoder as defined in Config_Load function

    Button_Init();   // Initialize pins for buttons as defined in Config_Load function

    
    i = ETH_LibInit(IPAddr, GWIPAddr, IPMask, MACAddr);  // Ethernet library initialization
    mStopIfError(i);
    if (i == WCHNET_ERR_SUCCESS)
        printf("[MAIN] WCHNET_LibInit Success\r\n");
    
    CalcBroadcastIP(IPAddr, IPMask);  // Caluculate Broadcast IP address from IP addres and netmask
    printf("[MAIN] BroadcastAddr = %d.%d.%d.%d\r\n",BroadcastAddr[0],BroadcastAddr[1],BroadcastAddr[2],BroadcastAddr[3]);
    
    WCHNET_CreateUdpSocket();  // Create  UDP Socket
    printf("[MAIN] IPAddr = %d.%d.%d.%d\r\n",IPAddr[0],IPAddr[1],IPAddr[2],IPAddr[3]);
    
    while(1)
    {
        Button_Task();

        Encoder_Task();

        // This section is only needes for incoming packets
        // Currently only sending
        /*Ethernet library main task function,
         * which needs to be called cyclically*/
        //WCHNET_MainTask();
        /*Query the Ethernet global interrupt,
         * if there is an interrupt, call the global interrupt handler*/
        /* if(WCHNET_QueryGlobalInt())
        {
            WCHNET_HandleGlobalInt();
        } */
    }
}

