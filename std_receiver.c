#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/leds.h"
#include <stdio.h>
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "net/netstack.h"

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Standard Receiver");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(broadcast_process, ev, data)
{
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();

  static struct etimer et;
  
  leds_on(LEDS_GREEN);		//green LED indicates that this mote is a receiver
	NETSTACK_MAC.off(1);
  broadcast_open(&broadcast, 129, &broadcast_call);
  
  while(1) 
  {
  	etimer_set(&et,CLOCK_SECOND/100);
  	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
