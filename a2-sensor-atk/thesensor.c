
/**
 * @file thesensor.c
 * @author Ghozlan
 * @brief 
 * @version 0.1
 * @date 2021-05-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "contiki.h"
#include "sensornode.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "random.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-icmp6.h"
#include <stdio.h>

/**
 * @brief Logging macros
 * 
 */
#define LOG_MODULE "TheSensor"

/**
 * @brief Server configuration macros
 * 
 */
#define UDP_SENSOR_PORT 6666
#define UDP_CONTROLLER_PORT 5555
#define COLLECTION_INTERVAL (3.3 * CLOCK_SECOND)


/**
 * @brief Global variable representing the server/connection that will
 * collect data from other (sensor) nodes
 */
static struct simple_udp_connection udp_conn;

//----------------------------------------------------------------
PROCESS(thesensor_proc, "TheSensor Process");
AUTOSTART_PROCESSES(&thesensor_proc);
//----------------------------------------------------------------
static void udp_rx_callback(struct simple_udp_connection *conn, 
const uip_ipaddr_t *sender_addr, uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
uint16_t receiver_port, const uint8_t *data, uint16_t datalen){
    LOG_INFO("Controller %.*s\n", datalen, (char *) data);
}

PROCESS_THREAD(thesensor_proc, ev, data) {
    static struct etimer timer;
    static sensor sensor;
    uip_ipaddr_t dstIP;
    PROCESS_BEGIN();
    simple_udp_register(&udp_conn, UDP_SENSOR_PORT, NULL, 
    UDP_CONTROLLER_PORT, udp_rx_callback);
    
    /**
     * @brief Construct a new etimer set object
     * 
     */
    etimer_set(&timer, COLLECTION_INTERVAL);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        /**
         * @brief Get the sensor readings and fill 
         * them in the sensor object
         * 
         */
        float v_ = (*awgnGen_ptr)();
        getSensorData(&sensor, v_);
        
        /**
         * @brief Check if the controller/collector node connectivity
         * 
         */
        if(NETSTACK_ROUTING.node_is_reachable() &&
        NETSTACK_ROUTING.get_root_ipaddr(&dstIP)) {
            simple_udp_sendto(&udp_conn, &sensor, sizeof(sensor), &dstIP);
            sensor.msgCount++;
        } else {
            LOG_INFO("[-]Controller not reachable\n");
        }
        /**
         * @brief Re-set timer object
         * 
         */
        etimer_set(&timer, COLLECTION_INTERVAL);
    }
    PROCESS_END();
}
