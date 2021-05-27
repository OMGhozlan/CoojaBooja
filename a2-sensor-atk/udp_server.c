/**
 * @file udp_server.c
 * @author Ghozlan
 * @brief A file that represents a node/server that collects data from other nodes/sensors
 * @version 0.1
 * @date 2021-05-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"


/**
 * @brief Logging macros
 * 
 */
#define LOG_MODULE "Server"
#define LOG_LEVEL LOG_LEVEL_INFO


/**
 * @brief Server configuration macros
 * 
 */
#define UDP_CLIENT_PORT	6666
#define UDP_SERVER_PORT	5555

/**
 * @brief Global variable representing the server that will
 * communicate with other (sensor) nodes
 */
static struct simple_udp_connection udp_conn;

//static linkaddr_t addr = {{0x00, 0xDE, 0xAD, 0xBE, 0xEF, 0xFA, 0xCA, 0xDE}};

PROCESS(srvr_node_proc, "Server");
AUTOSTART_PROCESSES(&srvr_node_proc);

/**
 * @brief Callback function invoked upon activation of RTX (on recieve)
 * 
 * @param c Pointer to udp connection struct
 * @param sndrAddr Address of the sending node
 * @param sndrPort Port used by sending node
 * @param rcvrAddr Address of the reciving node (self)
 * @param rcvrPort Port used by the reciving node (self)
 * @param data Data sent
 * @param datalen Length of the data sent
 */
static void rx_cback(struct simple_udp_connection *c, const uip_ipaddr_t *sndrAddr,
         uint16_t sndrPort, const uip_ipaddr_t *rcvrAddr, uint16_t rcvrPort,
         const uint8_t *data, uint16_t datalen){
  LOG_INFO("[*]Received'%.*s' from ", datalen, (char *)data);
  LOG_INFO_6ADDR(sndrAddr);
  LOG_INFO_("\n");
}


/**
 * @brief Construct a new process thread object
 * 
 */
PROCESS_THREAD(srvr_node_proc, ev, data){
  PROCESS_BEGIN();
  NETSTACK_ROUTING.root_start();
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, rx_cback);
  PROCESS_END();
}