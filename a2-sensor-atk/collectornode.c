/**
 * @file CollectorNode.c
 * @author Ghozlan
 * @brief A file that represents a node/server that collects data from other nodes/sensors
 * @version 0.1
 * @date 2021-05-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "contiki.h"
#include "sensornode.h"
#include "sys/log.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "net/linkaddr.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Logging macros
 * 
 */
#define LOG_MODULE "TheCollector"
#define LOG_LEVEL LOG_LEVEL_INFO


/**
 * @brief Server configuration macros
 * 
 */
#define WITH_ACK 0
#define UDP_SENSOR_PORT 6666
#define UDP_CONTROLLER_PORT 5555

/**
 * @brief Global variable representing the server/connection that will
 * collect data from other (sensor) nodes
 */
static struct simple_udp_connection udp_conn;
static linkaddr_t addr = {{0x00, 0xDE, 0xAD, 0xBE, 0xEF, 0xFA, 0xCA, 0xDE}};

/**
 * @brief IPv6 address of this (main) node
 * 
 */

PROCESS(collector_node_proc, "CollectorNode Process");
AUTOSTART_PROCESSES(&collector_node_proc);

static void rx_callback(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data,
uint16_t datalen) {
    LOG_INFO("[*] Received data ");
    //printf(sender_addr)
    //LOG_INFO_6ADDR(sender_addr);
    //LOG_INFO("\n");
}


/**
 * @brief Construct a new process thread object
 * 
 */
PROCESS_THREAD(collector_node_proc, ev, data) {
    PROCESS_BEGIN();
    linkaddr_set_node_addr(&addr);
    NETSTACK_ROUTING.root_start();
    simple_udp_register(&udp_conn, UDP_CONTROLLER_PORT, NULL,
    UDP_SENSOR_PORT, rx_callback);
    PROCESS_END();
}