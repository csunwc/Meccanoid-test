
codes:

commands to module:

00-???  data
F4 = discovered ; all set
FA = LIM mode ; stops motor & report servo pos
FB = not used
FC = request moduel type (Servo or LED) : RESPONSE 
FD = reset; erase ID
FE = empty; no module at this position : RESPONSE
FF = header/reserved to avoid confusion


servo's position  0x00 - 0xEF (0-180 deg)




The basic object design pattern is
(1) a packet object with data for all modules
(2) a communicate method is called repeatly,
each invoke advance to the next module.... thus time division to serve all modules.




V0 original lib ================================

problem: takes a long time to init. 50x because it uses round-robin, thus it takes 3-4 loops per modules.
to complete.  not very efficient.

next version: a new discover function just to config modules during init.

V1 mod lib with sequential module discovery ================================

problem: modules are not responsive in the 1st 4 messages... self init to find ID????
after that, init  is deterministric 1 discover call per module. 


V2 add read servo position w/ power off function ==========================

setServotoLIM(int servoNum)

loop:
 send packet
 getServoPosition(int servoNum)




