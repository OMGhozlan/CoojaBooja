/**
 * @file CollectorNode.c
 * @author your name (you@domain.com)
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

static void udp_rx_callback(struct simple_udp_connection *conn, const uip_ipaddr_t *sender_addr,
uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port,
const uint8_t *data, uint16_t datalen){
sensor sensor = *(struct sensorData*)(data);
LOG_INFO("Temp = %.3f || Humid = %.3f || Pres = %.3f\n",
(float) sensor.temperature, (float) sensor.humidity, (float) sensor.pressure);
#if WITH_ACK
char str[32];
    snprintf(str, sizeof(str), "Data Received");
    simple_udp_sendto(&udp_conn, str, strlen(str), sender_addr);
#endif
}
//----------------------------------------------------------------
// The process thread of controller
PROCESS_THREAD(collector_node_proc, ev, data) {
    PROCESS_BEGIN();
    NETSTACK_ROUTING.root_start();
    simple_udp_register(&udp_conn, UDP_CONTROLLER_PORT, NULL,
    UDP_SENSOR_PORT, udp_rx_callback);
    PROCESS_END();
}