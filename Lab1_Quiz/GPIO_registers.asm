 ; GPIO_registers: places the .set references to the correct addresses of the GPAMUX1, GPATOGGLE, GPADIR, GPDAT, and WDCR;

GPAMUX1 	.set 0x6F86		;Pin Function for the group	[GPIO 0:15]
GPBMUX1		.set 0x6F96		;Pin function for the group [GPIO 32:47]
GPATOGGLE 	.set 0x6FC6		;Toggles control of the configuration
GPADIR		.set 0x6F8A		;Sets the direction of control
GPADAT		.set 0x6FC0		;Contains the value of the DATA register 0:31
WDCR		.set 0x7029		;Watchdog Register
GPAPUD		.set 0x6F8C		;Pull-up resistors
GPBDAT		.set 0x6FC8		;GPIO data for 32:63
GPBDIR		.set 0x6F9A		;Direction of the 32:63 gpio
;****************************End of the GPIO******************************************************************************

