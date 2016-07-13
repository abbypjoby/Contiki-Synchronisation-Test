#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "sys/rtimer.h"
#include "net/netstack.h"
#include "dev/leds.h"
#include <stdio.h>


int count = 0;
int pc = 0;
char seq[2] = "a"; // used to keep track of the packet sent


/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Broadcaster");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/



static void trigger(struct rtimer *t , void *ptr)
{
		count++;
		if (count == 2000)		//count=2000 means 20seconds or 2000 slots
		{
    	packetbuf_copyfrom(seq, 2);
	    broadcast_send(&broadcast);
	    printf("*** Transmit '%s' with no of slots between two transmissions = %u \n",seq,count); //seq is used like a packet identifier, to keep track of the packets
    
	 	  if(seq[0] == 'z')
    		seq[0] = 'a';
    	else
    		seq[0]++;
    	
    	count=0;
    }
	rtimer_set(t,RTIMER_TIME(t) + RTIMER_ARCH_SECOND/100,1,trigger,ptr);		//reset the rtimer
}



PROCESS_THREAD(broadcast_process, ev, data)
{
  static struct etimer et;
  static struct rtimer rt;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
  
  leds_on(LEDS_RED);	//Red led indicates the mote broadcasts packets
  
	NETSTACK_MAC.off(1);
	rtimer_set(&rt,RTIMER_NOW() + RTIMER_ARCH_SECOND/100,1,trigger,NULL);	//schedule the rtimer to fire every RTIMER_ARCH_SECOND/100 seconds 
  broadcast_open(&broadcast, 129, &broadcast_call);
  
  while(1)
  {
    etimer_set(&et, CLOCK_SECOND/100);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
