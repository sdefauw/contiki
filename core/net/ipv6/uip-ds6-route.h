/*
 * Copyright (c) 2012, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef UIP_DS6_ROUTE_H
#define UIP_DS6_ROUTE_H

#include "sys/stimer.h"
#include "lib/list.h"

void uip_ds6_route_init(void);

#ifndef UIP_CONF_UIP_DS6_NOTIFICATIONS
#define UIP_DS6_NOTIFICATIONS 1
#else
#define UIP_DS6_NOTIFICATIONS UIP_CONF_UIP_DS6_NOTIFICATIONS
#endif

#if UIP_DS6_NOTIFICATIONS
/* Event constants for the uip-ds6 route notification interface. The
   notification interface allows for a user program to be notified via
   a callback when a route has been added or removed and when the
   system has added or removed a default route. */
#define UIP_DS6_NOTIFICATION_DEFRT_ADD 0
#define UIP_DS6_NOTIFICATION_DEFRT_RM  1
#define UIP_DS6_NOTIFICATION_ROUTE_ADD 2
#define UIP_DS6_NOTIFICATION_ROUTE_RM  3

typedef void (* uip_ds6_notification_callback)(int event,
					       uip_ipaddr_t *route,
					       uip_ipaddr_t *nexthop,
					       int num_routes);
struct uip_ds6_notification {
  struct uip_ds6_notification *next;
  uip_ds6_notification_callback callback;
};

void uip_ds6_notification_add(struct uip_ds6_notification *n,
			      uip_ds6_notification_callback c);

void uip_ds6_notification_rm(struct uip_ds6_notification *n);
/*--------------------------------------------------*/
#endif

/* Routing table */
#ifndef UIP_CONF_MAX_ROUTES
#ifdef UIP_CONF_DS6_ROUTE_NBU
#define UIP_DS6_ROUTE_NB UIP_CONF_DS6_ROUTE_NBU
#else /* UIP_CONF_DS6_ROUTE_NBU */
#define UIP_DS6_ROUTE_NB 4
#endif /* UIP_CONF_DS6_ROUTE_NBU */
#else /* UIP_CONF_MAX_ROUTES */
#define UIP_DS6_ROUTE_NB UIP_CONF_MAX_ROUTES
#endif /* UIP_CONF_MAX_ROUTES */

/* State of RPL  */
#if CONF_6LOWPAN_ND_OPTI_FUSION
#define UIP_RT_RPL_STATE_IN 0
#define UIP_RT_RPL_STATE_ACK 1
#endif /* CONF_6LOWPAN_ND_OPTI_FUSION */

/* State of DAD in RT */
#if CONF_6LOWPAN_ND_OPTI_TDAD
#define UIP_RT_DAD_STATE_IN_RT 1
#define UIP_RT_DAD_STATE_IN_DAD 2
#define UIP_RT_DAD_STATE_IN_BOTH 3
#define uip_rt_state_is_in_rt(x) (x->dup.isused & UIP_RT_DAD_STATE_IN_RT)
#define uip_rt_state_is_in_dad(x) (x->dup.isused & UIP_RT_DAD_STATE_IN_DAD)
#endif /* CONF_6LOWPAN_ND_OPTI_TDAD */

/** \brief define some additional RPL related route state and
 *  neighbor callback for RPL - if not a DS6_ROUTE_STATE is already set */
#ifndef UIP_DS6_ROUTE_STATE_TYPE
#define UIP_DS6_ROUTE_STATE_TYPE rpl_route_entry_t
/* Needed for the extended route entry state when using ContikiRPL */
typedef struct rpl_route_entry {
  uint32_t lifetime;
  void *dag;
  uint8_t learned_from;
  uint8_t nopath_received;
#if CONF_6LOWPAN_ND_OPTI_FUSION
  uint8_t state;
#endif /* CONF_6LOWPAN_ND_OPTI_FUSION */
} rpl_route_entry_t;
#endif /* UIP_DS6_ROUTE_STATE_TYPE */


/** \brief A Duplication Address Detection list entry */
#if UIP_CONF_6LBR
typedef struct uip_ds6_dup_addr {
  uint8_t isused;
#if !CONF_6LOWPAN_ND_OPTI_TDAD
  uip_ipaddr_t ipaddr;
#endif /* !CONF_6LOWPAN_ND_OPTI_TDAD */
  uip_lladdr_t eui64;
  struct stimer lifetime;
} uip_ds6_dup_addr_t; 
#endif /* UIP_CONF_6LBR */


/** \brief The neighbor routes hold a list of routing table entries
    that are attached to a specific neihbor. */
struct uip_ds6_route_neighbor_routes {
  LIST_STRUCT(route_list);
};

/** \brief A border router list entry */
#if CONF_6LOWPAN_ND
typedef struct uip_ds6_border_router {
  uint8_t state;
  uint32_t version;
#if UIP_CONF_6L_ROUTER
  uint8_t lifetime;        /* in 60sec */ 
#endif /* UIP_CONF_6L_ROUTER */
#if !UIP_CONF_6LBR
  struct stimer timeout;
  uint8_t rscount;
  struct stimer rs_timer;
#endif /* !UIP_CONF_6LBR */
  uip_ipaddr_t ipaddr;
} uip_ds6_border_router_t;
#endif /* CONF_6LOWPAN_ND */

/** \brief An entry in the routing table */
typedef struct uip_ds6_route {
  struct uip_ds6_route *next;
  /* Each route entry belongs to a specific neighbor. That neighbor
     holds a list of all routing entries that go through it. The
     routes field point to the uip_ds6_route_neighbor_routes that
     belong to the neighbor table entry that this routing table entry
     uses. */
  struct uip_ds6_route_neighbor_routes *neighbor_routes;
  uip_ipaddr_t ipaddr;
#ifdef UIP_DS6_ROUTE_STATE_TYPE
  UIP_DS6_ROUTE_STATE_TYPE state;
#endif
#if CONF_6LOWPAN_ND_OPTI_TDAD
  uip_ds6_dup_addr_t dup;
#endif /* CONF_6LOWPAN_ND_OPTI_TDAD */
  uint8_t length;
} uip_ds6_route_t;

/** \brief A neighbor route list entry, used on the
    uip_ds6_route->neighbor_routes->route_list list. */
struct uip_ds6_route_neighbor_route {
  struct uip_ds6_route_neighbor_route *next;
  struct uip_ds6_route *route;
};

/** \brief An entry in the default router list */
typedef struct uip_ds6_defrt {
  struct uip_ds6_defrt *next;
  uip_ipaddr_t ipaddr;
  struct stimer lifetime;
  uint8_t isinfinite;
#if CONF_6LOWPAN_ND
  uip_ds6_border_router_t *br;
  uint8_t state;
#endif /* CONF_6LOWPAN_ND */
} uip_ds6_defrt_t;


/** \name Border router list basic routines */
/** @{ */
#if CONF_6LOWPAN_ND
uip_ds6_border_router_t *uip_ds6_br_add(uint32_t version, uint16_t lifetime, 
                                        uip_ipaddr_t *ipaddr);
void uip_ds6_br_rm(uip_ds6_border_router_t *br);
uip_ds6_border_router_t *uip_ds6_br_lookup(uip_ipaddr_t *ipaddr);
#if UIP_CONF_6LBR
void uip_ds6_br_config();
void uip_ds6_br_periodic(void);
#endif /* UIP_CONF_6LBR */
#endif /* CONF_6LOWPAN_ND */
/** @} */

/** \name Default router list basic routines */
/** @{ */
uip_ds6_defrt_t *uip_ds6_defrt_add(uip_ipaddr_t *ipaddr,
                                   unsigned long interval);
void uip_ds6_defrt_rm(uip_ds6_defrt_t *defrt);
uip_ds6_defrt_t *uip_ds6_defrt_lookup(uip_ipaddr_t *ipaddr);
uip_ipaddr_t *uip_ds6_defrt_choose(void);

void uip_ds6_defrt_periodic(void);
/** @} */


/** \name Routing Table basic routines */
/** @{ */
uip_ds6_route_t *uip_ds6_route_lookup(uip_ipaddr_t *destipaddr);
#if CONF_6LOWPAN_ND
uip_ds6_route_t *uip_ds6_route_lookup_by_nexthop(uip_ipaddr_t *ipaddr);
#endif /* CONF_6LOWPAN_ND */
uip_ds6_route_t *uip_ds6_route_add(uip_ipaddr_t *ipaddr, uint8_t length,
                                   uip_ipaddr_t *next_hop);
void uip_ds6_route_rm(uip_ds6_route_t *route);
void uip_ds6_route_rm_by_nexthop(uip_ipaddr_t *nexthop);

uip_ipaddr_t *uip_ds6_route_nexthop(uip_ds6_route_t *);
int uip_ds6_route_num_routes(void);
uip_ds6_route_t *uip_ds6_route_head(void);
uip_ds6_route_t *uip_ds6_route_next(uip_ds6_route_t *);

/** @} */

#endif /* UIP_DS6_ROUTE_H */
