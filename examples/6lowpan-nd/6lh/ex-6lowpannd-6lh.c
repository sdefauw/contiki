/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

#include "contiki.h"
#include "net/uip.h"
#include "net/uip-nd6.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"
#include "sys/etimer.h"

#include <stdio.h>
#include <string.h>

#define SEND_INTERVAL   (1 * CLOCK_SECOND)

/*---------------------------------------------------------------------------*/
PROCESS(test_aro, "Test process of 6LoWPAN ND (ARO)");
AUTOSTART_PROCESSES(&test_aro);

/*---------------------------------------------------------------------------*/
static uip_ipaddr_t *
set_global_address(void)
{
  static uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }

  return &ipaddr;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_aro, ev, data)
{
  uip_ipaddr_t *ipaddr;
  static struct etimer periodic_timer;

	PROCESS_BEGIN();

	printf("TEST ;) \n");

  ipaddr = set_global_address();
  uip_ds6_init();


  etimer_set(&periodic_timer, SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
    //SEND NS every x second
    printf("ARO send...\n");
    //uip_nd6_ns_output(NULL, NULL, ipaddr);
    uip_nd6_ns_output_aro(NULL, NULL, ipaddr, 60);
    tcpip_ipv6_output();
    etimer_set(&periodic_timer, SEND_INTERVAL);
  }

  PROCESS_END();
  printf("END PROCESS :() \n");
}