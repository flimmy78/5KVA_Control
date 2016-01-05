#include "DSP2833x_Device.h"     // Device Header Include File
#include "DSP2833x.h" 
#include "DA_Driver.h"

void InitSpiA() {
// First Init GPIO
	EALLOW;
	/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

	GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

	//enable SPIA CLK
	SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   // SPI-A
	EDIS;

	//init FIFO
	// Initialize SPI FIFO registers and disable FIFOs.
	SpiaRegs.SPIFFTX.all = 0x8040;				// disable Tx FIFO;
	SpiaRegs.SPIFFRX.all = 0x0040;				// disable Rx FIFO;
	SpiaRegs.SPIFFCT.all = 0x0;

	//init SPI A
	SpiaRegs.SPICCR.all = 0x000F;	  // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICTL.all = 0x0006;    		// Enable master mode, normal phase,
											// enable talk, and SPI int disabled.
	SpiaRegs.SPIBRR = 0x007F;
	SpiaRegs.SPICCR.all = 0x008F;		         // Relinquish SPI from Reset
	SpiaRegs.SPIPRI.bit.FREE = 1;   // Set so breakpoints don't disturb xmission
}

void set_DA(Uint16 Value, Uint16 CHN) //from 0 to 7
{
	DELAY_US(200);
	SpiaRegs.SPITXBUF = (CHN << 12) | (Value << 2);
	DELAY_US(200);
	SpiaRegs.SPITXBUF = 0xA000 | (0x0001 << CHN);
}
