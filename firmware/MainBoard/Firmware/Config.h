
#ifndef config_h
#define config_h

#define pinTempBoilerIn              25 //27 // wewnętrzny czujnik temperatury pieca (czujnik osadzony wewnątrz kotła) - podstawowy czujnik temperatury kotła
#define pinTempBoilerOut             30 // zewnętrzny czujnik temperatury pieca (czujnik przy wyjściu z pieca obiegu wody centralnego ogrzewania) - dodatkowe zabezpieczenie
#define pinTempWater                 33 // czujnik temperatury wody użytkowej
#define pinBoilerFeeder              3  // podajnik kotła
#define pinBoilerCentralHeatingPump  5  // pompa centralnego ogrzewania
#define pinBoilerWaterPump           6  // pompa ciepłej wody użytkowej
#define pinBoilerFloorPump           4  // pompa podłogówki
#define pinBoilerFeederHall          36 // czujniik halla podajnika kotła
#define pinBoilerFan                 8  // dmuchawa kotła
#define pinBuzzer                    39 // buzzer
#define pinRxNextion                 18 // Serial 1 - port szeregowy dla wyswietlacza dla Mega2560 exmple: EasyNex myNex(Serial1);
#define pinTxNextion                 19 // Serial 1 - port szeregowy dla wyswietlacza dla Mega2560



#define tempInterval               2000      // co jaki czas maja byc odczytywane czujniki temperatury 10000 = 1 sec
#define timeToStopFeeder           2000      // opoznienie zatrzymania podajnika
#define timeToEmergencyFeeder      180000UL  // czas do awaryjnego zatrzymania podajnika gdy hallotron nie działa - blokada podajnika do resetu sterownika
#define timeToUpdatescreen         1000      // czas co jaki jest aktualizowany wyświetlacz


#define ostatniPomiarTemp  0
#define ostatniaZmianaTemp 1
#define ostatniaTemp       2

#define blowingForce       5  // siła nadmuchu od 2 do 10: 1 - 10%, 2 - 20%, 3 - 30%, 10 - 100%
#define maxMillis          4294967296 // Maksymalna wartość millis()

#endif
