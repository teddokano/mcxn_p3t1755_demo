/*
 * Copyright 2022 NXP
 * Copyright 2024 Tedd OKANO
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#define		ENABLE_I3C

#include	"r01lib.h"

#include	"config.h"
#include	"pin_control.h"

r01lib_start;	/* *** place this word before making instance of r01lib classes *** */

I3C			i3c;
I2C			i2c;

#ifdef	ENABLE_I3C
P3T1755		p3t1755( i3c );
#else
P3T1755		p3t1755( i2c, 0x4C );
#endif

DigitalOut	r(    RED   );	//	== D5 pin
DigitalOut	g(    GREEN );	//	== D6 pin
DigitalOut	b(    BLUE  );	//	"BLUE" (D3) is a dummy LED difinition. This pin is overriden by PWM output
DigitalOut	trig( D2    );	//	IBI detection trigger. Pin D0~D2, D4~D13, D18, D19 and A0~A5 can be used

void	DAA_set_dynamic_ddress_from_static_ddress( uint8_t static_address, uint8_t dynamic_address );

volatile int	g_Flag	= false;

void cb( void )
{
	g_Flag	= true;
}

int main(void)
{
#if 1
	InterruptIn	in( SW2 );
	in.rise( cb );

	while ( true )
	{
		if ( g_Flag )
		{
			g_Flag	= false;
			PRINTF( "!!!\r\n", in & 0x01 );
		}
		
		PRINTF( "%d\r\n", in & 0x01 );
		wait( 0.1 );
	}

#endif
	
	
#if 0
	Ticker	t;
	t.attach( cb, 1 );

	while ( true )
	{
		if ( g_Flag )
		{
			g_Flag	= false;
			PRINTF( "###\r\n" );
		}
	}
#endif
#if 0

	while ( true )
	{
		uint8_t	reg	= 0x00;
		int16_t	data;
		i2c.write( 0x4C, &reg, sizeof( reg ), NO_STOP );
		i2c.read( 0x4C, (uint8_t *)&data, sizeof( data ) );

		PRINTF( "temp = %8.4f˚C\r\n", (float)(((data & 0xFF) << 8) | ((data >> 8) & 0xFF)) / 256.0 );

		wait( 0.1 );
	}
#endif

	init_pin_control();
	i3c.set_IBI_callback( ibi_trigger_output );

	PRINTF("\r\nP3T1755 (Temperature sensor) I3C operation sample: getting temperature data and IBI\r\n");

#ifdef	ENABLE_I3C
	DAA_set_dynamic_ddress_from_static_ddress( P3T1755_ADDR_I2C, P3T1755_ADDR_I3C );
	p3t1755.address_overwrite( P3T1755_ADDR_I3C );
#endif
	
	float ref_temp	= p3t1755.temp();
	float low		= ref_temp + 1.0;
	float high		= ref_temp + 2.0;

	p3t1755.high( high );
	p3t1755.low(  low  );

	PRINTF( "  T_LOW / T_HIGH registers updated: %8.4f˚C / %8.4f˚C\r\n", low, high );
	PRINTF( "      based on current temperature: %8.4f˚C\r\n", ref_temp );

	p3t1755.conf( p3t1755.conf() | 0x02 );		//	ALART pin configured to INT mode

#ifdef	ENABLE_I3C
	p3t1755.ccc_set( CCC::DIRECT_ENEC, 0x01 );	// Enable IBI
#endif
	p3t1755.info();

	float	temp;
	uint8_t	ibi_addr;

	while ( true )
	{
		if ( (ibi_addr	= i3c.check_IBI()) )
			PRINTF("*** IBI : Got IBI from target_address: 7’h%02X (0x%02X)\r\n", ibi_addr, ibi_addr << 1 );

		temp	= p3t1755;
		PRINTF( "Temperature: %8.4f˚C\r\n", temp );

		led_set_color( temp, ref_temp );
		wait( 1 );
	}
}

void DAA_set_dynamic_ddress_from_static_ddress( uint8_t static_address, uint8_t dynamic_address )
{
	i3c.ccc_broadcast( CCC::BROADCAST_RSTDAA, NULL, 0 ); // Reset DAA
	i3c.ccc_set( CCC::DIRECT_SETDASA, static_address, dynamic_address << 1 ); // Set Dynamic Address from Static Address
}
