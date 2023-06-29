/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-1b-ReglageAutomatiqueDateHeure.ino
  
  Description :   Programme permettant de régler la date et l'heure, "automatiquement" après de l'upload de ce code
                  dans votre DS3231, et redémarrage de votre Arduino (la date et heure inscrite sera celle de votre ordinateur)
                  
  Remarques :     - la librairie utilisée ici sera la "RTClib" de Adafruit (https://github.com/adafruit/RTClib)
                  - l'arduino utilisé dans cet exemple sera un Arduino Nano
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       26.06.2023

*/

// Inclusion de la librairie RTClib de Adafruit
#include "RTClib.h"

// Instanciation de celle-ci
RTC_DS3231 ds3231;

// ========================
// Initialisation programme
// ========================
void setup() {

  // Initialisation de la liaison série (PC <-> Arduino Nano)
  Serial.begin(9600);
  Serial.println(F("================================================================================"));
  Serial.println(F("Exemple DS3231 #1b : mise à jour automatique de la date et heure de votre DS3231"));
  Serial.println(F("                     après upload de ce programme / redémarrage de votre arduino"));
  Serial.println(F("                     (la date et heure utilisée sera celle de votre ordinateur)" ));
  Serial.println(F("================================================================================"));
  Serial.println("");


  // Initialisation du module DS 3231
  if (!ds3231.begin()) {
    Serial.println("[ERREUR] Impossible d'établir la connexion avec votre module DS3231 (problème de câblage ?)");
    Serial.flush();
    while (1);
  }
  


  // ************************************************************************************************************
  // Option 1 : réglage systématique de la date/heure du DS3231, après chaque upload/redémarrage de votre arduino
  // ************************************************************************************************************
  ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));    
 
  
  // *****************************************************************************************
  // Option 2 : mise à jour de la date et heure de votre DS3231, après upload de ce programme,
  //            SEULEMENT si le DS3231 signale qu'il a "perdu l'heure" (ce qui a pu arriver
  //            si sa pile de secours a été défaillante, par exemple)
  // *****************************************************************************************
  // if (ds3231.lostPower()) {
  //  ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));      
  // }


  // ====================================================================================
  // ----->
  // ----->  désactivez l'une ou autre des 2 options ci-dessus, selon vos besoins  <-----
  //                                                                               <-----
  // ====================================================================================

  Serial.println("Enregitrement de la nouvelle date/heure réussi !");

}

// =================
// Boucle principale
// =================
void loop() {
  
    // Vide (tout se passe dans la fonction "loop", ci-dessus)
    
}
