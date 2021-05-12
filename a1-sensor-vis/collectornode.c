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
#include "sys/log.h"
#include "sensornode.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief  Logging macros
 * 
 */
#define LOG_MODULE "CollectorNode"
#define LOG_LEVEL LOG_LEVEL_INFO // Acts as a method to print/echo data

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

PROCESS(collector_node_proc, "CollectorNode Process");
AUTOSTART_PROCESSES(&collector_node_proc);

/**
 * @brief Construct a new process thread object
 * 
 */
PROCESS_THREAD(collector_node_proc, ev, data) {
    PROCESS_BEGIN();
    NETSTACK_ROUTING.root_start();
    simple_udp_register(&udp_conn, UDP_CONTROLLER_PORT, NULL,
    UDP_SENSOR_PORT, NULL);
    PROCESS_END();
}