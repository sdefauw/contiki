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
#include "sys/etimer.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip.h"

#include <stdio.h>


/*---------------------------------------------------------------------------*/
PROCESS(test_host, "Test process of 6LoWPAN ND host");
AUTOSTART_PROCESSES(&test_host);

/*---------------------------------------------------------------------------*/
void
printstat(uint8_t id)
{
  printf("s>%u:", id);
  printf("%d,%d,%d,%d|", uip_stat.ip.recv, uip_stat.ip.sent, uip_stat.ip.forwarded, uip_stat.ip.drop);
  printf("%d,%d\n", uip_stat.nd6.recv, uip_stat.nd6.sent);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_host, ev, data)
{
  static struct etimer periodic_timer;
  static uip_ds6_addr_t * addr;
  static uint32_t t;
  static uint8_t id;

  PROCESS_BEGIN();

  t = 0;
  etimer_set(&periodic_timer, CLOCK_SECOND);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
    etimer_set(&periodic_timer, CLOCK_SECOND);
    t++;
    if((addr = uip_ds6_get_global(ADDR_PREFERRED)) != NULL) {
      id = addr->ipaddr.u8[15];
      printf("c>%u:%u\n", id,t);
      break;
    }
  }

  printstat(id);
  etimer_set(&periodic_timer, 60*60*CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
  printstat(id);


  PROCESS_END();
}

