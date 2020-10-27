
#ifndef config_h
#define config_h

#define czujnikTempCO_1    0 // wewnętrzny w kotle
#define czujnikTempCO_2    1 // zewnetrzny na ujsciu wody z kotła
#define czujnikTempCWU     2

#define okresPomiaruTemp  5000 // co jaki czas maja byc odczytywane czujniki temperatury 1000 = 1 sec

#define ostatniPomiarTemp  0
#define ostatniaZmianaTemp 1
#define ostatniaTemp       2

#define deltaTemp          1 // 1 stopień celcjusza

float zadanaTempCO     = 55;
float zadanaTempCWU    = 60;
int   silaDmuchawy     = 50; // od 10% do 100%
float tempHistereza    = 1;
int   interwalPrzepalanie          = 40*60; // w sekundach 40 minut
int   interwalPodajnikaStandardowy = 60;    // odstęp pomiędzy załaczeniami podajnika w sekundach
int   interwalPodajnikaRozpalanie  = 180;   // odstęp pomiędzy załaczeniami podajnika w sekundach

#endif
