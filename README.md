# Multiclient-Server

Hello,

this is my Multiclient, I have following Problem:

If I send "BEG" to the Server I want to block every other Process to use the function "Bedingung" and if I send "END" to the Server it should wake up the other clients/processes.

So my Idea right now is to set up a Mutex Semaphore in the main, to block other processes, but it doesn't work. 


To use this Server compile the Code and use in another bash telnet localhost 5678. 
