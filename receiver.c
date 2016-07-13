#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/leds.h"
#include <stdio.h>
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "net/netstack.h"


int count = 0;
/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Receiver");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	//count is incremented when rtimer fires. When a packet is received, print count value and reset it to 1 
  printf("*** Received '%s' slots counted between consecutive receptions = %u\n",(char *)packetbuf_dataptr(), count);
  count = 1;		//reset the count value to zero
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/


static void trigger(struct rtimer *t , void *ptr)
{
	count++;		
  rtimer_set(t,RTIMER_TIME(t) + RTIMER_ARCH_SECOND/100,1,trigger,ptr);		//reset the rtimer
}


PROCESS_THREAD(broadcast_process, ev, data)
{
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
	count = 0;
  static struct rtimer rt;
  static struct etimer et;
  
  leds_on(LEDS_GREEN);	//green LED indicates that the mote is a receiver
  
	NETSTACK_MAC.off(1);
  broadcast_open(&broadcast, 129, &broadcast_call);
  rtimer_set(&rt,RTIMER_NOW() + RTIMER_ARCH_SECOND/100,1,trigger,NULL);	//set the rtimer to fire every RTIMER_ARCH_SECOND/100 seconds
  
  while(1) 
  {
  	etimer_set(&et,CLOCK_SECOND/100);
  	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}



/*---------------------------------------------------------------------------*/
