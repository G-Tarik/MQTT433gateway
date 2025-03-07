alectoWS1700Init();
alectoWSD17Init();
alectoWX500Init();
arctechContactInit();
arctechDimmerInit();
arctechDuskInit();
arctechMotionInit();
arctechScreenInit();
arctechScreenOldInit();
arctechSwitchInit();
arctechSwitchOldInit();
auriolInit();
beamishSwitchInit();
clarusSwitchInit();
cleverwattsInit();
conradRSLContactInit();
conradRSLSwitchInit();
daycomInit();
ehomeInit();
elro300SwitchInit();
elro400SwitchInit();
elro800ContactInit();
elro800SwitchInit();
eurodomestSwitchInit();
ev1527Init();
fanjuInit();
heitechInit();
impulsInit();
iwds07Init();
keruiD026Init();
logilinkSwitchInit();
mumbiInit();
nexusInit();

/*
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/ninjablocks_weather.c: In function 'ninjablocksWeatherInit':
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/ninjablocks_weather.c:34:26: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '71910' to '6374' [-Woverflow]
   34 | #define MIN_PULSE_LENGTH 2115
      |                          ^~~~
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/ninjablocks_weather.c:214:35: note: in expansion of macro 'MIN_PULSE_LENGTH'
  214 |  ninjablocks_weather->mingaplen = MIN_PULSE_LENGTH*PULSE_DIV;
      |                                   ^~~~~~~~~~~~~~~~
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/ninjablocks_weather.c:35:26: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '72250' to '6714' [-Woverflow]
   35 | #define MAX_PULSE_LENGTH 2125
      |                          ^~~~
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/ninjablocks_weather.c:215:35: note: in expansion of macro 'MAX_PULSE_LENGTH'
  215 |  ninjablocks_weather->maxgaplen = MAX_PULSE_LENGTH*PULSE_DIV;
*/
// ninjablocksWeatherInit();

pollinInit();
quiggGT1000Init();

/*
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_gt7000.c: In function 'createFooter':
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_gt7000.c:35:28: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '81000' to '15464' [-Woverflow]
   35 | #define PULSE_QUIGG_FOOTER 81000
      |                            ^~~~~
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_gt7000.c:146:46: note: in expansion of macro 'PULSE_QUIGG_FOOTER'
  146 |  quigg_gt7000->raw[quigg_gt7000->rawlen-1] = PULSE_QUIGG_FOOTER;
      |                                              ^~~~~~~~~~~~~~~~~~
Compiling .pio/build/nodemcuv2/lib21e/ESPiLight/pilight/libs/pilight/protocols/433.92/rsl366.c.o
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_gt7000.c: In function 'quiggGT7000Init':
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_gt7000.c:272:28: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '72900' to '7364' [-Woverflow]
  272 |  quigg_gt7000->maxgaplen = (int)(PULSE_QUIGG_FOOTER*0.9);
      |                            ^
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_gt7000.c:273:28: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '89100' to '23564' [-Woverflow]
  273 |  quigg_gt7000->mingaplen = (int)(PULSE_QUIGG_FOOTER*1.1);
*/
// quiggGT7000Init();

quiggGT9000Init();

/*
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_screen.c: In function 'createFooter':
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_screen.c:35:35: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '81000' to '15464' [-Woverflow]
35 | #define PULSE_QUIGG_SCREEN_FOOTER 81000
   |                                   ^~~~~
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_screen.c:157:46: note: in expansion of macro 'PULSE_QUIGG_SCREEN_FOOTER'
157 |  quigg_screen->raw[quigg_screen->rawlen-1] = PULSE_QUIGG_SCREEN_FOOTER;
   |                                              ^~~~~~~~~~~~~~~~~~~~~~~~~
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_screen.c: In function 'quiggScreenInit':
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_screen.c:289:28: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '72900' to '7364' [-Woverflow]
289 |  quigg_screen->maxgaplen = (int)(PULSE_QUIGG_SCREEN_FOOTER*0.9);
   |                            ^
lib/ESPiLight/src/pilight/libs/pilight/protocols/433.92/quigg_screen.c:290:28: warning: unsigned conversion from 'int' to 'uint16_t' {aka 'short unsigned int'} changes value from '89100' to '23564' [-Woverflow]
290 |  quigg_screen->mingaplen = (int)(PULSE_QUIGG_SCREEN_FOOTER*1.1);
*/
// quiggScreenInit();

rc101Init();
rsl366Init();
sc2262Init();
secudoSmokeInit();
selectremoteInit();
silvercrestInit();
smartwaresSwitchInit();
tcmInit();
techlicoSwitchInit();
teknihallInit();
tfa2017Init();
tfa30Init();
tfaInit();
x10Init();