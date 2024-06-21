#include "ppu.h"

void ppu_connect_cartridge(ppu* ppu_ctx, cartridge* cart){
	
	ppu_ctx->cart = cart;
	
}

uint8_t ppu_read_byte(uint16_t address)
{

    return 0x0000;
}

void ppu_write_byte(uint16_t address,uint8_t value){


}

uint8_t ppu_read_cpu(uint16_t address){
    
    uint8_t data = 0x00;

	switch (address)
	{
	case 0x0000: // Control
		break;
	case 0x0001: // Mask
		break;
	case 0x0002: // Status
		break;
	case 0x0003: // OAM Addr
		break;
	case 0x0004: // OAM Data
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // PPU Address
		break;
	case 0x0007: // PPU Data
		break;
	}

	return data;

}

uint8_t ppu_write_cpu(uint16_t address, uint8_t value){

	uint8_t data = 0x00;

	switch (address)
	{
	case 0x0000: // Control
		break;
	case 0x0001: // Mask
		break;
	case 0x0002: // Status
		break;
	case 0x0003: // OAM Addr
		break;
	case 0x0004: // OAM Data
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // PPU Address
		break;
	case 0x0007: // PPU Data
		break;
	}

}