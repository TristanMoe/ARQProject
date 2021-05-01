#include "crc.h"
#include "packets.h"

using namespace std;

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
	unsigned char test[] = "123456789";

	/*
 * Print the check value for the selected CRC algorithm.
 */
	printf("The check value for the %s standard is 0x%X\n", CRC_NAME, CHECK_VALUE);

	/*
	 * Compute the CRC of the test message, slowly.
	 */
	printf("The crcSlow() of \"123456789\" is 0x%X\n", crc_slow(test, sizeof(test)));

	/*
	 * Compute the CRC of the test message, more efficiently.
	 */
	crc_init();
	printf("The crcFast() of \"123456789\" is 0x%X\n", crc_fast(test, sizeof(test)));

	struct packet_array * p_array; 

	size_t i; 
	const size_t data_size = 200; 
	uint8_t data[data_size]; 
	for (i = 0; i < 200; i++)
	{
		data[i] = rand() % 50; 
	}
	
	p_array = create_packets(data, data_size, false); 


	printf("PACKET ONE:\n");
	print_packet(&p_array->packets[0]);

	//printf("\n\nPACKET TWO:\n");
	//print_packet(&p_array->packets[1]); 
		
	bool val_check = validate_checksum(&p_array->packets[0]); 
	printf("%d", val_check); 

	printf("\n\nPACKET ONE:\n");
	print_packet(&p_array->packets[0]);

	//p_array->packets[0].data[7] = (uint8_t) 22; 
	val_check = validate_checksum(&p_array->packets[0]);
	printf("%d", val_check);
	
	printf("\n\nPACKET ONE:\n");
	print_packet(&p_array->packets[0]);

	p_array->free_mem(); 
	return 0;
}
