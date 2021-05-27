/**
 * @file udp_client.c
 * @author Ghozlan
 * @brief A file that represents a node/client that sends data to a server
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
#include "random.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include <stdio.h>
#include <stdlib.h>


#define LOG_MODULE "Client"
#define LOG_LEVEL LOG_LEVEL_INFO

/**
 * @brief Node configuration macros
 * 
 */
#define UDP_CLIENT_PORT	6666
#define UDP_SERVER_PORT	5555
#define SEND_INTERVAL (5 * CLOCK_SECOND)

/**
 * @brief Global variable representing the server that will
 * communicate with other (sensor) nodes
 */
static struct simple_udp_connection udp_conn;
///////////////////////////////////////
static char* d_[] = {"31,25.052,40.025,101000.109",
"2,25.016,42.010,101002.188",
"30,25.404,40.028,101002.398",
"6,25.007,41.044,101000.297",
"18,25.512,42.482,101000.859",
"4,26.118,40.265,101000.125",
"27,25.658,42.492,101000.422",
"7,27.410,42.380,101000.039",
"14,27.483,40.406,101000.008",
"15,25.196,42.260,101000.219",
"25,27.504,40.707,101000.047",
"20,25.013,40.057,101002.477",
"16,26.691,40.433,101001.508",
"10,25.009,40.217,101000.039",
"12,26.572,40.006,101000.148",
"29,25.173,41.647,101000.016",
"22,25.218,41.526,101000.062",
"26,27.132,41.447,101000.055",
"9,25.737,40.080,101002.500",
"5,27.328,40.006,101000.188",
"19,25.009,41.551,101000.258",
"17,25.245,41.388,101000.141",
"13,25.334,42.425,101002.219",
"3,25.055,42.408,101000.117",
"21,25.176,40.709,101000.930",
"23,25.748,42.039,101002.188",
"24,25.050,40.262,101000.555",
"8,27.466,41.342,101000.406",
"28,27.226,40.389,101002.398",
"11,25.053,41.730,101000.078",
"31,25.198,40.272,101000.297",
"2,27.223,41.019,101000.078",
"30,25.013,40.702,101000.008",
"6,27.474,40.006,101000.102",
"18,26.266,40.017,101001.906",
"4,26.882,40.013,101002.484",
"27,27.504,41.744,101002.312",
"7,25.500,42.090,101000.500",
"14,25.747,41.657,101001.211",
"15,26.529,41.514,101000.039",
"25,26.550,40.019,101002.070",
"20,27.321,40.317,101002.508",
"16,26.926,40.303,101000.055",
"10,27.384,40.664,101000.117",
"12,25.010,41.278,101001.117",
"29,27.287,40.007,101001.445",
"22,25.133,40.853,101002.055",
"26,25.017,41.728,101001.414",
"9,27.116,42.349,101000.078",
"5,26.812,40.263,101001.320",
"19,26.430,41.908,101000.031",
"17,25.007,40.787,101000.648",
"13,25.886,40.037,101000.352",
"3,25.252,41.946,101001.219",
"21,27.340,40.017,101001.914",
"23,25.047,42.405,101002.203",
"24,25.069,41.723,101002.047",
"8,25.013,40.155,101000.023",
"28,27.448,40.040,101000.945",
"11,26.651,40.699,101001.828",
"31,25.248,42.415,101000.234",
"2,26.057,41.551,101000.977",
"30,25.087,40.799,101000.023",
"6,26.336,42.442,101000.391",
"18,25.034,40.007,101000.273",
"4,26.534,42.459,101000.023",
"27,27.458,40.377,101000.039",
"7,25.713,40.151,101000.039",
"14,25.041,40.016,101000.695",
"15,25.022,40.026,101001.805",
"25,25.033,40.016,101001.219",
"20,27.389,42.057,101000.117",
"16,25.098,42.505,101000.086",
"10,25.101,40.074,101000.031",
"12,26.906,40.038,101000.508",
"29,25.865,41.416,101000.133",
"22,25.425,41.601,101002.508",
"26,26.404,41.534,101000.055",
"9,25.918,40.012,101001.000",
"5,25.051,40.381,101000.359",
"19,26.559,40.047,101000.789",
"17,25.328,41.148,101001.602",
"13,26.248,40.328,101000.117",
"3,27.314,42.501,101002.141",
"21,25.290,40.443,101000.688",
"23,25.013,42.241,101000.719",
"24,25.226,40.015,101000.008",
"8,26.264,42.373,101000.492",
"28,25.484,41.835,101000.039",
"11,25.053,41.272,101000.172",
"31,25.049,40.050,101002.031",
"2,25.738,41.324,101000.117",
"30,25.084,41.807,101000.445",
"6,25.268,40.741,101000.250",
"18,27.463,41.068,101000.047",
"4,25.062,40.006,101000.758",
"27,25.204,42.506,101000.062",
"7,26.878,40.124,101000.016",
"14,27.155,40.006,101000.289" };
///////////////////////////////////////


PROCESS(client_node_proc, "Client");
AUTOSTART_PROCESSES(&client_node_proc);

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


PROCESS_THREAD(client_node_proc, ev, data){
    static struct etimer timer;
    static char str[100];
    uip_ipaddr_t dstIP;    

    PROCESS_BEGIN();
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, 
    UDP_SERVER_PORT, rx_cback);
    
    /**
     * @brief Construct a new etimer set object
     * 
     */
    etimer_set(&timer, SEND_INTERVAL);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        /**
         * @brief Check if the controller/collector node connectivity
         * 
         */
        if(NETSTACK_ROUTING.node_is_reachable() &&
        NETSTACK_ROUTING.get_root_ipaddr(&dstIP)) {                    
            /**
             * @brief Get the sensor readings and fill 
             * them in the sensor object
             * 
             */
            snprintf(str, 255, "%s", d_[random_rand() % 99]);
            simple_udp_sendto(&udp_conn, str, strlen(str), &dstIP);
        } else {
            LOG_INFO("[-]Controller not reachable\n");
        }
        /**
         * @brief Re-set timer object
         * 
         */
        etimer_set(&timer, SEND_INTERVAL
            - CLOCK_SECOND + (random_rand() % (2 * CLOCK_SECOND)));
    }
    PROCESS_END();
}