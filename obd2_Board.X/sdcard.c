
#include <xc.h>
#include "sdcard.h"
#include "mcc_generated_files/spi2.h" 

// 512 byte read buffer
//unsigned char SDRdata[512];
// 512 byte write buffer
//unsigned char SDWdata[512];
// 512 byte block buffer
unsigned char SDdata[512];
// SD command buffer
unsigned char SDcommand[6];
// no response flag
unsigned char no_response = 0;
// holds SD card response byte
unsigned char card_response = 0;
// timeout variable to determine card timeout
unsigned int timeout = SD_TIMEOUT;

void SDcard_init(void)
{
    char *dataReceived;
    unsigned char i = 0;
    unsigned char b = 0;
    
    // configure CS pin
    CSTRIS &= ~CS; // configure CS as output
    CS_high; // set CS high

    // send 80 clocks (10 bytes) to wake up SD card
    // load dummy values into buffer
    for(i = 0; i < 10; i++){
	SDdata[i] = 0xFF;
    }
    SPI2_Exchange8bitBuffer(SDdata, 10, dataReceived);
    // set CS low
    CS_low;

    // transmit command 0
    SDcommand[0] = 0x40; // start bit | host bit | command
    SDcommand[1] = 0x00; // no arguments
    SDcommand[2] = 0x00;
    SDcommand[3] = 0x00;
    SDcommand[4] = 0x00;
    SDcommand[5] = 0x95; // precalculated CRC
    SPI2_Exchange8bitBuffer(SDcommand, 6, dataReceived);

    // read back response
    SDcard_get_response(0x01);

    // load command 1
    SDcommand[0] = 0x41; // start bit | host bit | command
    SDcommand[1] = 0x00; // no arguments
    SDcommand[2] = 0x00;
    SDcommand[3] = 0x00;
    SDcommand[4] = 0x00;
    SDcommand[5] = 0x95; // CRC not needed, dummy byte
    // wait for SD card to go to idle mode
    no_response = 1;
    while(no_response){
            // send command 1
            SPI2_Exchange8bitBuffer(SDcommand, 6, dataReceived);
            // read back response
            // response time is 0 to 8 bytes
            SPI2_Exchange8bitBuffer(SDdata, 8, dataReceived);
            for(b = 0; b < 7; b++){
                    if(SDdata[b] == 0x00) no_response = 0;
            }
    }

    // set SD card CS high
    CS_high;
}

void SDcard_read_block(unsigned long address)
{
    char *dataReceived;
    
    // set CS low
    CS_low;
    // load CMD17 with proper
    // block address
    SDcommand[0] = 0x51; // 0x40 | 0x11 (17)
    SDcommand[1] = (address>>24) & 0xFF;
    SDcommand[2] = (address>>16) & 0xFF;
    SDcommand[3] = (address>>8) & 0xFF;
    SDcommand[4] = address & 0xFF;
    SDcommand[5] = 0xFF;

    // send command
    SPI2_Exchange8bitBuffer(SDcommand, 6, dataReceived);

    // read back response
    SDcard_get_response(0x00);

    // read back response and
    // wait for data token FEh
    SDcard_get_response(0xFE);
    // receive data block
    SPI2_Exchange8bitBuffer(SDcommand, 512, SDdata);
    // flush two bytes of CRC data
    SPI2_Exchange8bit(0xFF);
    SPI2_Exchange8bit(0xFF);
    SPI2_Exchange8bit(0xFF);

    // set SD card CS high
    CS_high;
}

void SDcard_write_block(unsigned long address)
{
    char *dataReceived;

    // set CS low
    CS_low;
    // load CMD24 with proper
    // block address
    SDcommand[0] = 0x58; // 0x40 | 0x18 (24)
    SDcommand[1] = (address>>24) & 0xFF;
    SDcommand[2] = (address>>16) & 0xFF;
    SDcommand[3] = (address>>8) & 0xFF;
    SDcommand[4] = address & 0xFF;
    SDcommand[5] = 0xFF;

    // send command
    SPI2_Exchange8bitBuffer(SDcommand, 6, dataReceived);

    // read back response
    SDcard_get_response(0x00);

    // send a one byte gap
    SPI2_Exchange8bit(0xFF);
    
    // begin block write
    // send data token 0xFE
    SPI2_Exchange8bit(0xFE);
    // write data block
    SPI2_Exchange8bitBuffer(SDcommand, 512, SDdata);
    
    // send two byte CRC data
    SPI2_Exchange8bit(0xFF);
    SPI2_Exchange8bit(0xFF);

    // read data response
    SPI2_Exchange8bitBuffer(SDcommand, 1, card_response);
    
    // set SD card CS high
    CS_high;
}

unsigned char SDcard_get_response(unsigned char response)
{
    char *dataReceived;

    // read back response
    // response time is 0 to 8 bytes
    no_response = 1;
    
    SPI2_Exchange8bitBuffer(SDcommand, 1, dataReceived);
    while(no_response && timeout){
        if(dataReceived == response) no_response = 0; // check if response matches
        timeout--;
    }
    if(timeout == 0){ // if loop has timed out
        return 1;
    }
    // if response received
    return 0;
}