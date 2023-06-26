/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-3-LectureDateHeureDS3231.ino
  
  Description :   Programme permettant de lire la date et heure "en cours" dans le DS3231
                  
  Remarques :     - la librairie utilisée ici sera la "RTClib" de Adafruit (https://github.com/adafruit/RTClib)
                  - l'arduino utilisé pour les essais sera un Nano
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       26.06.2023

*/

// Inclusion de la librairie RTClib de Adafruit
#include "RTClib.h"

// Instanciation de celle-ci
RTC_DS3231 ds3231;

// Constante(s)
char joursDeLaSemaine[7][8] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};    // 7 jours de huit caractères de long


// ========================
// Initialisation programme
// ========================
void setup() {

  // Initialisation de la liaison série (PC <-> Arduino Nano)
  Serial.begin(9600);
  Serial.println(F("========================================================================"));
  Serial.println(F("Exemple DS3231 #3 : lecture de la date et heure contenue dans le DS3231,"));
  Serial.println(F("                    avec rafraîchissement toutes les 2 secondes         "));
  Serial.println(F("========================================================================"));
  Serial.println("");


  // Initialisation du module DS3231
  if (!ds3231.begin()) {
    Serial.println("[ERREUR] Impossible d'établir la connexion avec votre module DS3231 (problème de câblage ?)");
    Serial.flush();
    while (1);
  }

}

// =================
// Boucle principale
// =================
void loop() {

  // Lecture de la date/heure actuelle, de notre horloge temps réel DS 3231
  DateTime dateHeureDuDS3231 = ds3231.now();

  // Affichage de ça sur le moniteur série
  Serial.print("Date/heure actuelle (DS3231) : ");
  Serial.print(dateHeureDuDS3231.year(), DEC);
  Serial.print("/");
  Serial.print(dateHeureDuDS3231.month(), DEC);
  Serial.print("/");
  Serial.print(dateHeureDuDS3231.day(), DEC);
  Serial.print("/");
  Serial.print(joursDeLaSemaine[dateHeureDuDS3231.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(dateHeureDuDS3231.hour(), DEC);
  Serial.print(":");
  Serial.print(dateHeureDuDS3231.minute(), DEC);
  Serial.print(":");
  Serial.print(dateHeureDuDS3231.second(), DEC);
  Serial.println();

  // Attente de 2 secondes, avant relecture de l'heure (histoire de vérifier que l'horloge RTC "avance bien" !)
  delay(2000);
    
}
