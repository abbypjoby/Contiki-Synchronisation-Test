
# Timer Synchronization Test for running 6TiSCH

## Overview

Time slotted channel hopping (TSCH) is a combination of two efficient methods
namely time synchronisation and channel hopping attempted at creating a low
power, highly reliable operation. It has been implemented successfully in the
field of Internet of Things using the Contiki operating system in several
hardware platforms. Implementing a minimal configuration of 6TiSCH (A basic RPL +
6TiSCH network) on platforms involve configuring their radio drivers.  6TiSCH
involves real time task scheduling of communication slots using the inherent
rtimer. Any synchronisation problems would have come as a result of portability
issues with the 6TiSCH code, Radio driver configuration issues or timer
desynchronisation. Attempts at decoding the root cause would involve an
elaborate study on the 6TiSCH code and continuous debugging of the hardware. 

This document describes a test procedure (useful for developers working in
Contiki) that will tell the user whether the rtimer on the specific mote is
capable of implementing 6TiSCH.

Being the most important timer in the context of 6TiSCH, the rtimer permits
pre-emptive task scheduling. When an rtimer fires, the function associated with
it is immediately executed regardless of the process being executed at that
instant. In this testing method, we create slots of 10ms duration using rtimer and
transmit packets at intervals of 2000 slots or 20 seconds. The receiver on
obtaining the first packet starts it’s inherent rtimer and counts the number of
slots till it receives the next packet. If the receiving mote counts the number
of slots between two consecutive packet receptions as 2000, we can say that the
rtimer of both motes are in sync.


## Prerequisites

Before starting the test procedure, it is important to identify a reference
mote. A reference mote is one which could be kept as a standard against which
various parameters of the target mote could be compared. The most important
criteria for selecting a reference mote is accurate timing configurations. The
reference mote's notion of time should match with the wall clock time. This
could be verified with the help of a small program using rtimer and checking if
the motes notion of say 10 mins matches with that observed on the system on the
host, or even using a handheld stopwatch to get initial symptoms of the timer
inaccuracies. A variation of around 1sec for every 10mins could be ignored as
human delay (time taken by a person to press stop,start).  Moreover, the
reference mote should be capable of implementing 6TiSCH. Using the motes we have
in our lab, Zolertia Z1 mote and Tmote sky were shortlisted as reference motes.
However, various other motes that satisfy the above criteria could also be
chosen. 


# TSCH_sync_test1


Method to verify stability of rtimer across motes (performed on similar motes)

### Steps : 
Ensure that both the programs (broadcaster.c and std_receiver.c) are in the
folder “contiki/examples/rime”

1. Upload the program 'broadcaster.c' onto the first target mote.
2. Upload the program 'receiver.c' onto the second target mote.
3. Connect both motes to the computer. 
4. Observe their output from the serial port using applications like Gtkterm 
   or putty (it takes 20 seconds for each reading to appear on the screen).
5. Reset the motes and observe the output again.
6. Ignore the first reading.
7. Compare your readings with the expected output shown below. 
   If your readings are coming out to be same as that shown below 
   (ignoring the first value), Then the rtimers of both the motes are 
    accurate enough to implement TSCH among each other. 


Expected output  (similar to below)

```
ID:1	Rime started with address 1.0
ID:1	MAC 01:00:00:00:00:00:00:00 Contiki-3.x-2412-gc1b5323 started. 
        Node id is set to 1.
ID:1	CSMA ContikiMAC, channel check rate 8 Hz, radio channel 26
ID:1	Starting 'Broadcaster'
ID:1	*** Transmit 'a' with no of slots between two transmissions = 2000 
ID:1	*** Transmit 'b' with no of slots between two transmissions = 2000 	
ID:1	*** Transmit 'c' with no of slots between two transmissions = 2000 	
ID:1	*** Transmit 'd' with no of slots between two transmissions = 2000 	
ID:1	*** Transmit 'e' with no of slots between two transmissions = 2000 	


ID:2	Rime started with address 2.0
ID:2	MAC 02:00:00:00:00:00:00:00 Contiki-3.x-2412-gc1b5323 started. Node id is set to 2.
ID:2	CSMA ContikiMAC, channel check rate 8 Hz, radio channel 26
ID:2	Starting 'Receiver'
ID:2	*** Received 'a' slots counted between consecutive receptions =2417	  //this value is ignored
ID:2	*** Received 'b' slots counted between consecutive receptions = 2000
ID:2	*** Received 'c' slots counted between consecutive receptions = 2000
ID:2	*** Received 'd' slots counted between consecutive receptions = 2000
ID:2	*** Received 'e' slots counted between consecutive receptions = 2000
```

### Note :

Make sure that both the motes have different Node id values. 
If the received slot count is between 1999 - 2001 (not exactly 2000), It may
still be possible to implement TSCH.  

If the received slot count is further apart by an amount of more than 2, you're going to have a really tough time trying to get TSCH working on that mote.  

Reference motes could be used as a standard to test the compatability of the
target mote in a heterogenous TSCH network.


# TSCH_sync_test2

Method to verify stability of the target motes rtimer against that of a standard reference mote (performed on different motes)



### Steps : 

Ensure that both the programs (broadcaster.c and std_receiver.c) are in the
folder “contiki/examples/rime” also replace the driver code ie 'cc2420.c' found
in "contiki/dev/cc2420" with the driver code given here - 'cc2420.c' (Note that
the name of the driver code has to be cc2420.c otherwise it will not work).

1. Upload the program 'broadcaster.c' onto the target mote whose rtimer needs 
   testing.
2. Upload the program 'std_receiver.c' onto the standard reference mote.
4. Observe their output from the serial port using applications like Gtkterm 
   or putty.
5. Reset the motes and observe the output again (it takes 20 seconds
   for each reading to appear on the screen).
6. Ignore the first reading.
7. Compare your readings with the expected output shown below. If your 
   readings are coming out to be same as that shown below (ignoring the first 
   value), Then the rtimer of the target mote is accurate enough to implement 
   TSCH. 





Expected output  (similar to below)
```
ID:1	Rime started with address 1.0
ID:1	MAC 01:00:00:00:00:00:00:00 Contiki-3.x-2412-gc1b5323 started. Node id is set to 1.
ID:1	CSMA ContikiMAC, channel check rate 8 Hz, radio channel 26
ID:1	Starting 'Broadcaster'
ID:1	*** Transmit 'a' with no of slots between two transmissions = 2000 
ID:1	*** Transmit 'b' with no of slots between two transmissions = 2000 
ID:1	*** Transmit 'c' with no of slots between two transmissions = 2000 	
ID:1	*** Transmit 'd' with no of slots between two transmissions = 2000 
ID:1	*** Transmit 'e' with no of slots between two transmissions = 2000 


ID:2	Rime started with address 2.0
ID:2	MAC 02:00:00:00:00:00:00:00 Contiki-3.x-2412-gc1b5323 started. Node id is set to 2.
ID:2	CSMA ContikiMAC, channel check rate 8 Hz, radio channel 26
ID:2	Starting 'Standard Receiver'
ID:2	*** Received, slots counted between consecutive receptions = 1939
ID:2	*** Received, slots counted between consecutive receptions = 2000
ID:2	*** Received, slots counted between consecutive receptions = 2000
ID:2	*** Received, slots counted between consecutive receptions = 2000
ID:2	*** Received, slots counted between consecutive receptions = 2000
```
### Note :

Make sure that both the motes have different Node id values. 

If the received slot count is between 1999 - 2001 (not exactly 2000), It may
still be possible to implement TSCH.

If the received slot count is further apart by an amount of more than 2, you're
going to have a really tough time trying to get TSCH working on that mote. 
