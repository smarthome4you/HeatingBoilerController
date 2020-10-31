
#ifndef config_h
#define config_h

#define pinTempBoilerIn              25 // wewnętrzny czujnik temperatury pieca (czujnik osadzony wewnątrz kotła) - podstawowy czujnik temperatury kotła
#define pinTempBoilerOut             26 // zewnętrzny czujnik temperatury pieca (czujnik przy wyjściu z pieca obiegu wody centralnego ogrzewania) - dodatkowe zabezpieczenie
#define pinTempWater                 27 // czujnik temperatury wody użytkowej
#define pinBoilerCentralHeatingPump  28 // pompa centralnego ogrzewania
#define pinBoilerWaterPump           29 // pompa ciepłej wody użytkowej
#define pinBoilerFloorPump           30 // pompa podłogówki
#define pinBoilerFeeder              31 // podajnik kotła
#define pinBoilerFeederHall          32 // czujniik halla podajnika kotła
#define pinBoilerBlower              32 // dmuchawa kotła


#define tempInterval          1000      // co jaki czas maja byc odczytywane czujniki temperatury 10000 = 1 sec
#define timeToStopFeeder      2000      // opoznienie zatrzymania podajnika
#define timeToEmergencyFeeder 60000     // czas do awaryjnego zatrzymania podajnika gdy hallotron nie działa - blokada podajnika do resetu sterownika

#define ostatniPomiarTemp  0
#define ostatniaZmianaTemp 1
#define ostatniaTemp       2

#define blowingForce      50  // siła nadmuchu w %

#endif
