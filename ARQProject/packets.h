#include <cstdint>
#include <stdbool.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "crc.h"
#define PACKET_SIZE 128 
#define START_BYTE 0x01 
#define COMMAND_DIRECT 0x0E 
#define COMMAND_RELAY 0x0F 
#define END_BYTE 0x04 


typedef struct packet {
	uint8_t data[PACKET_SIZE] = { 0 };
} packet;

typedef struct packet_array {
	packet_array(size_t n_pack)
	{
		n_packets = n_pack;
		packets = (packet *)malloc(sizeof(packet) * n_packets);

		if (packets == NULL) {
			printf("Malloc of size %zu failed!\n", n_packets);
			exit(-1);
		}
	}

	void free_mem()
	{
		free(packets); 
	}

	packet* packets;
	size_t n_packets;
} packet_array;

void print_packet(packet * packet)
{
	size_t i; 
	for (i = 0; i < PACKET_SIZE; i++)
	{
		printf("[%zu] = [%u]\n", i, packet->data[i]);
	}
}

bool validate_checksum(packet * packet) 
{
	// Subtract checksum and end byte. 
	const size_t subset_size = PACKET_SIZE - 2;
	uint8_t checksum_subset[PACKET_SIZE] = {0};
	memcpy(checksum_subset, packet->data, subset_size);

	crc_init(); 
	uint8_t checksum = crc_fast(checksum_subset, PACKET_SIZE); 
	uint8_t packet_checksum = packet->data[PACKET_SIZE - 2];
	// 2nd last byte is checksum. 
	return checksum == packet_checksum; 
}

packet_array* create_packets(const uint8_t * const data, size_t n_bytes, bool relay)
{
	size_t i;
	size_t j;
	size_t n_packets;
	size_t n_packet_write;
	// Start, Command, Checksum, End Bytes; 
	size_t n_prefix = 4;
	size_t written_bytes = 0;
	// Data start from index 2. 
	size_t i_packet = 2;
	
	double approx_packets = (double) n_bytes / (PACKET_SIZE - n_prefix);
	n_packets = ceil(approx_packets); 

	n_packet_write = (PACKET_SIZE - n_prefix);

	struct packet_array packets(n_packets); 

	crc_init();

	for (i = 0; i < n_packets; i++)
	{
		struct packet p;
		p.data[0] = START_BYTE;
		p.data[1] = relay == true ? COMMAND_RELAY : COMMAND_DIRECT;

		if (written_bytes + n_packet_write < n_bytes)
		{
			for (j = i * n_packet_write; j < (i + 1) * n_packet_write; j++)
			{
				p.data[i_packet] = data[j];
				written_bytes++;
				i_packet++;
			}
		}
		else
		{
			for (j = i * n_packet_write; j < n_bytes; j++)
			{
				p.data[i_packet] = data[j];
				i_packet++;
			}
		}

		i_packet = 2;

		// Calculate checksum for command + content bytes.
		p.data[PACKET_SIZE - 2] = crc_fast(p.data, PACKET_SIZE);
		p.data[PACKET_SIZE - 1] = END_BYTE;

		// Set packet into packet array.
		packets.packets[i] = p;
	}

	return &packets;
}