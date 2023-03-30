// igethz.c - Interface funksjon for teperaturm†ler - iGetHz()
// Author: Einar Valen, Date: 14.3.1994, Tested ok: 14.3.1994


// INCLUDES
#include <time.h>
#include <stdio.h>
#include <dos.h>


// DEFINES
#define TEST_KODE
#define PORT_ADRESSE  iHz // Adresse til io-port
													// (eller Hz optelling ved test)
#define SECONDS 1     		// Antall sekunder sampling skal vare
													// Jo lengre interval jo mer n›yaktig resultat

#ifdef TEST_KODE
	// Avledningsmanøver i.f.m test
	#define INPORTB( Hz_Count) ErstatningForInportb( Hz_Count)
#else
	#define INPORTB( adr) inportb( adr)  // Ekte vare.
#endif


// FUNCTIONS

#ifdef TEST_KODE
static int iAkkumulator = 0;

unsigned char ErstatningForInportb( int iHz) // Testfunksjon
	{
	unsigned char byte;		// Simulerer verdi returnert fra inportb()
	iAkkumulator++;
	byte = iAkkumulator % 2;    // 0 og 1 annenhver gang
//  printf( "c==%x, i==%d, iHz==%d.\n", byte, iAkkumulator, iHz);
	return (byte);		// Returnerer 0x00 og 0x01 annenhver gang
	}
#endif


int iGetHz( void) // Returnerer antall ganger bit 7 == 1
	{
	int iHz = 0;
	clock_t Slutt = clock() + (CLK_TCK * SECONDS); // N†r sampling skal slutte
	while (clock() < Slutt)  								    // Er det p† tide † slutte?
		iHz += (INPORTB( PORT_ADRESSE) & 0x01);  // Tell n†r bit 7 == 1 ?
	return (iHz);                         // Returner antall enere i perioden
	}


#ifdef TEST_KODE
int main(void)
	{
	int j;
	for (j = 0; j < 20; j++)  // Test Funksjonen 20 ganger
		printf( "j==%d: iGetHz()==%d\n", j, iGetHz() );
	return (0);
	}
#endif


// End of File