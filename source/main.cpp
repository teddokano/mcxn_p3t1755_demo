/*
 * Copyright 2022 NXP
 * Copyright 2024 Tedd OKANO
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include	"r01lib.h"

#include	"config.h"
r01lib_start;	/* *** place this word before making instance of r01lib classes *** */

DigitalOut	r(    RED   );	//	== D5 pin
DigitalOut	g(    GREEN );	//	== D6 pin
DigitalOut	b(    BLUE  );	//	"BLUE" (D3) is a dummy LED difinition. This pin is overriden by PWM output
DigitalOut	trig( D2    );	//	IBI detection trigger. Pin D0~D2, D4~D13, D18, D19 and A0~A5 can be used

DigitalOut	dp[]	= {
		DigitalOut( D0 ),
		DigitalOut( D1 ),
		DigitalOut( D2 ),
		DigitalOut( D3 ),
		DigitalOut( D4 ),
		DigitalOut( D5 ),
		DigitalOut( D6 ),
		DigitalOut( D7 ),
		DigitalOut( D8 ),
		DigitalOut( D9 ),
		DigitalOut( D10 ),
		DigitalOut( D11 ),
		DigitalOut( D12 ),
		DigitalOut( D13 ),
		DigitalOut( D18 ),
		DigitalOut( D19 )
};
DigitalOut	ap[]	= {
		DigitalOut( A0 ),
		DigitalOut( A1 ),
		DigitalOut( A2 ),
		DigitalOut( A3 ),
		DigitalOut( A4 ),
		DigitalOut( A5 )
};

int main(void)
{
	r	= 1;
	g	= 1;
	b	= 1;

	while ( true )
	{
		for ( int i = 0; i < sizeof( dp ) / sizeof( DigitalOut ); i++ )
		{
			for ( int t = 0; t < i + 1; t++ )
			{
				dp[ i ]	= 1;
				dp[ i ]	= 0;
			}
		}
		for ( int i = 0; i < sizeof( ap ) / sizeof( DigitalOut ); i++ )
		{
			for ( int t = 0; t < i + 1; t++ )
			{
				ap[ i ]	= 1;
				ap[ i ]	= 0;
			}
		}
	}
}
