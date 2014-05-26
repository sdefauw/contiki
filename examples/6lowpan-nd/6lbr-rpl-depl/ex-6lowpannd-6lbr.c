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
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-ds6.h"
#include "sys/etimer.h"

#include "net/rpl/rpl.h"
#include "net/ip/uip-debug.h"

#include <stdio.h>



#define PREFIX_INIT 0xbbbb

static uip_ipaddr_t ipaddr;

/*---------------------------------------------------------------------------*/
PROCESS(test_router, "");
AUTOSTART_PROCESSES(&test_router);

/*---------------------------------------------------------------------------*/
void
set_global_address()
{

  uip_ip6addr(&ipaddr, PREFIX_INIT, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
}

/*---------------------------------------------------------------------------*/
void
set_prefix_address()
{

  uip_ip6addr(&ipaddr, PREFIX_INIT, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_prefix_add(&ipaddr, 64, 1, 0xc0, 86400, 14400);

}

/*---------------------------------------------------------------------------*/
void
set_context_prefix_address()
{

  uip_ip6addr(&ipaddr, PREFIX_INIT, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_context_pref_add(&ipaddr, 16, 10);
}

/*---------------------------------------------------------------------------*/
void
printstat()
{
  printf("s>br:");
  printf("%d,%d,%d,%d|", uip_stat.ip.recv, uip_stat.ip.sent, uip_stat.ip.forwarded, uip_stat.ip.drop);
  printf("%d,%d\n", uip_stat.nd6.recv, uip_stat.nd6.sent);
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_router, ev, data)
{
	static struct etimer periodic_timer;

  PROCESS_BEGIN();
 

  set_global_address();
  set_prefix_address();
  set_context_prefix_address();
  uip_ds6_br_config();

  /* Config RPL root */
  uip_ipaddr_copy(&ipaddr, &uip_ds6_get_global(ADDR_PREFERRED)->ipaddr);
  printf("dag %d\n", rpl_set_root(RPL_DEFAULT_INSTANCE,&ipaddr));

  etimer_set(&periodic_timer, 60*60*CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
  printstat();


  PROCESS_END();
}
