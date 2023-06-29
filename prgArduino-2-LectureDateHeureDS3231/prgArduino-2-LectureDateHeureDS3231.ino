/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-2-LectureDateHeureDS3231.ino
  
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
char joursDeLaSemaine[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};


// ========================
// Initialisation programme
// ========================
void setup() {

  // Initialisation de la liaison série (PC <-> Arduino Nano)
  Serial.begin(9600);
  Serial.println(F("========================================================================"));
  Serial.println(F("Exemple DS3231 #2 : lecture de la date et heure contenue dans le DS3231,"));
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


// =================================================================================
// Fonction : formateValeurSurDeuxChiffresMinimum
// (permet d'avoir une homogénéïté, au niveau de l'affichage, sur le moniteur série)
// =================================================================================
String formateValeurSurDeuxChiffresMinimum(int nombre)
{
  if (nombre < 10)
    return "0" + String(nombre);
  else
    return String(nombre);
}


// =================
// Boucle principale
// =================
void loop() {

  // Lecture de la date/heure actuelle, de notre horloge temps réel DS 3231
  DateTime dateHeureDuDS3231 = ds3231.now();

  // Affichage de ça sur le moniteur série
  Serial.print("Date/heure actuelle (DS3231) : ");
  Serial.print(joursDeLaSemaine[dateHeureDuDS3231.dayOfTheWeek()]);
  Serial.print(" ");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dateHeureDuDS3231.day()));
  Serial.print("/");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dateHeureDuDS3231.month()));
  Serial.print("/");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dateHeureDuDS3231.year()));
  Serial.print(" ");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dateHeureDuDS3231.hour()));
  Serial.print(":");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dateHeureDuDS3231.minute()));
  Serial.print(":");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dateHeureDuDS3231.second()));
  Serial.println();

  // Attente de 2 secondes, avant relecture de l'heure (histoire de vérifier que l'horloge RTC "avance bien" !)
  delay(2000);
    
}
