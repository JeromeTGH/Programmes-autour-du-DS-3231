/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-1-ReglageManuelDateHeure.ino
  
  Description :   Programme permettant de régler la date et l'heure "manuellement",
                  c'est à dire en inscrivant ces valeurs là, précisément, dans le code
                  
  Remarques :     - l'arduino utilisé ici sera un modèle Nano
                  - la librairie utilisée sera RTClib de Adafruit (https://github.com/adafruit/RTClib)
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       25.06.2023

*/

// Inclusion de la librairie RTClib de Adafruit
#include "RTClib.h"

// Instanciation de celle-ci
RTC_DS3231 ds3231;

// ========================
// Initialisation programme
// ========================
void setup() {

  // Initialisation de la liaison série (PC <-> arduino nano)
  Serial.begin(9600);
  Serial.println(F("========================================================================="));
  Serial.println(F("Exemple DS3231 #1 : réglage 'manuel' de la date/heure d'un module DS 3231"));
  Serial.println(F("========================================================================="));
  Serial.println("");

  // Initialisation du module DS 3231
  if (!ds3231.begin()) {
    Serial.println("[ERREUR] Impossible de se connecter au module DS3231 (vérifier le câblage, par exemple");
    Serial.flush();
    while (1);
  }

  // **********************************************
  // Réglage d'une date/heure, de manière explicite
  // **********************************************
  
        // Exemple permettant de régler la date/heure du "25 juin 2023, à 22h45"
        // (valeurs modifiables comme bon vous semble, dans la limite de ce que les champs peuvent accepter)
        
        uint16_t  Annee     = 2023;     // de 2000 à 2099
        uint8_t   Mois      = 6;        // de 1 à 12
        uint8_t   Jour      = 25;       // de 1 à 31
        uint8_t   Heure     = 22;       // de 0 à 23
        uint8_t   Minutes   = 45;       // de 0 à 59
        uint8_t   Secondes  = 0;        // de 0 à 59
        
  ds3231.adjust(DateTime(Annee, Mois, Jour, Heure, Minutes, Secondes));

}


// =================
// Boucle principale
// =================
void loop() {
  
    // Vide (tout se passe dans la fonction "loop")
    
}
