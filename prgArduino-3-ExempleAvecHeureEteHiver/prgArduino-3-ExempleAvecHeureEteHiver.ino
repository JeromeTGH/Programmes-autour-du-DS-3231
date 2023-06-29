/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-3-ExempleAvecHeureEteHiver.ino
  
  Description :   Programme permettant de régler la date et l'heure d'un DS3231 à l'upload de ce programme,
                  au format "UTC" (c'est à dire : sans tenir compte des fuseaux horaires)
                  
  Remarques :     - la librairie qui sera utilisée ici sera la "RTClib" de Adafruit (https://github.com/adafruit/RTClib)
                  - l'arduino utilisé pour tester ce programme sera un Arduino Nano
                  - ce programme analysera la date/heure locale du PC, fera une conversion vers de l'UTC, sur la base du
                    fuseau horaire "France" (étant entendu qu'en France nous sommes en "UTC+1" en hiver, et en "UTC+2" en été)
                  - ce programme peut mal fonctionner si vous l'uploadez juste au moment ou aux alentours d'un changement d'heure (été ↔ hiver, j'entends)
                  - ATTENTION : chaque "reset" de l'arduino reprogramme la dernière heure connue du PC, et faussera donc ensuite l'heure (en fait, ici,
                                il ne s'agit que d'un programme exemple de base, pour illustrer une potentielle fonction "heure d'été / heure d'hiver"
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       29.06.2023

*/

// Inclusion de la librairie RTClib d'Adafruit
#include "RTClib.h"

// Instanciation de celle-ci
RTC_DS3231 ds3231;

// Constantes
const int nombreDheuresArajouterOuEnleverEnHiver  = 1;      // En France, en heure d'été, nous somme en "UTC + 1 heure" (d'où le "1" ici)
const int nombreDheuresArajouterOuEnleverEnEte    = 2;      // En France, en heure d'été, nous somme en "UTC + 2 heures" (d'où le "2" ici)

const char joursDeLaSemaine[7][12]                = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};

const int nombreDeSecondesAcompenser              = 10;     // Pour compenser le temps d'upload de ce programme, lors de l'enregistrement
                                                            // de la date/heure dans cette horloge RTC

// ========================
// Initialisation programme
// ========================
void setup() {

  // Initialisation de la liaison série (PC <-> Arduino Nano)
  Serial.begin(9600);
  Serial.println(F("==============================================================================="));
  Serial.println(F("Exemple DS3231 #3 : mise à jour de la date/heure de votre DS3231, au format UTC"));
  Serial.println(F("                   (valable seulement 'en France', et doit être adapté au delà)"));
  Serial.println(F("==============================================================================="));
  Serial.println("");


  // Initialisation du module DS 3231
  if (!ds3231.begin()) {
    Serial.println("[ERREUR] Impossible d'établir la connexion avec votre module DS3231 (problème de câblage ?)");
    Serial.flush();
    while (1);
  }

  
  // Affichage des paramètres saisis (heures UTC, été/hiver)
  Serial.println(F("Paramètres entrés (pour compensation en heures, conversion UTC <-> heure locale du pays) : "));
  Serial.print(F("   → pour l'hiver : "));
  if(nombreDheuresArajouterOuEnleverEnHiver>0)
    Serial.print(F("+"));
  Serial.print(nombreDheuresArajouterOuEnleverEnHiver);
  Serial.print(F(" H"));
  Serial.println();
  Serial.print(F("   → pour l'été   : "));
  if(nombreDheuresArajouterOuEnleverEnEte>0)
    Serial.print(F("+"));
  Serial.print(nombreDheuresArajouterOuEnleverEnEte);
  Serial.print(F(" H"));
  Serial.println();
  Serial.println();


  // Récupération de la date/heure de votre PC (depuis où vous uploadez ce programme, vers votre arduino)
  DateTime dateHeurePC (F(__DATE__), F(__TIME__));       // Récupération de la date et heure de votre PC (on peut être ici en heure d'été ou d'hiver)
  DateTime dateHeureLocale (dateHeurePC + TimeSpan(0,0,0,nombreDeSecondesAcompenser));    // Ajoute 0 jour, 0 heures, 0 minutes, et x secondes


  // Affichage de cette date/heure là sur le moniteur série, de l'IDE Arduino
  Serial.println(F("Date/heure (locale) lue sur le PC : "));
  Serial.print(F("   → "));
  afficheUneDateHeureSurLeMoniteurSerie(dateHeureLocale);
  Serial.println();

  // Affichage du fait qu'il s'agisse ou non d'une heure d'été/hiver
  Serial.print(F("   → remarque : on est en heure d'"));
  if(estOnEnHeureDeEte(dateHeureLocale))
    Serial.print(F("été"));
  else
    Serial.print(F("hiver"));
  Serial.println();
  Serial.println();

  // Calcul du nombre d'heures à retrancher ou rajouter, pour passer en heure UTC
  int nbreDheuresAretrancherOuAjouter = estOnEnHeureDeEte(dateHeureLocale) ? nombreDheuresArajouterOuEnleverEnEte : nombreDheuresArajouterOuEnleverEnHiver;    

  // Conversion de la date/heure locale au format UTC
  Serial.println(F("Date/heure (UTC) d'après la date/heure locale :"));
  DateTime dateHeureUTC     (dateHeureLocale - TimeSpan(0,nbreDheuresAretrancherOuAjouter,0,0));
  Serial.print(F("   → "));
  afficheUneDateHeureSurLeMoniteurSerie(dateHeureUTC);
  Serial.println();
  Serial.println();

  // Enregistrement de cette date/heure UTC dans le DS3231
  Serial.println(F("Enregistrement de cette date/heure UTC dans le DS3231 :"));
  ds3231.adjust(dateHeureUTC);
  Serial.println(F("   → effectué"));
  Serial.println();


  // Lecture de la date/heure contenue dans le DS 3231
  Serial.println(F("Lecture de la date/heure contenue dans le DS 3231 :"));
  DateTime dateHeureDuDS3231 = ds3231.now();
  Serial.print(F("   → tel que lu dans l'horloge  : "));
  afficheUneDateHeureSurLeMoniteurSerie(dateHeureDuDS3231);
  Serial.println(" (UTC)");

  // Calcul du nombre d'heures à rajouter ou enlever, pour arriver à l'heure de "notre fuseau horaire (+ compensation été/hiver)
  int nbreDheuresAajouterOuRetirer = estOnEnHeureDeEte(dateHeureLocale) ? nombreDheuresArajouterOuEnleverEnEte : nombreDheuresArajouterOuEnleverEnHiver;
  
  // Conversion de la date/heure UTC contenue dans le DS3231 en date/heure locale, correspondant à notre fuseau horaire (avec compensation heure d'été ou hiver)
  DateTime dateHeureDuDS3231Corrige (dateHeureDuDS3231 + TimeSpan(0,nbreDheuresAajouterOuRetirer,0,0));    // On ajoute ou retire 0 jour, x heure, 0 minute, et 0 seconde
  Serial.print(F("   → au format \"local\" (France) : "));
  afficheUneDateHeureSurLeMoniteurSerie(dateHeureDuDS3231Corrige);
  Serial.println();
  
}

// =================
// Boucle principale
// =================
void loop() {
  
  // Vide (tout se passe dans la section "setup")
    
}


// ======================================================================
// Fonction "afficheUneDateHeureSurLeMoniteurSerie"
// (permet d'afficher "proprement" une date/heure, sur le moniteur série)
// ======================================================================
void afficheUneDateHeureSurLeMoniteurSerie(DateTime dt) {
  Serial.print(joursDeLaSemaine[dt.dayOfTheWeek()]);
  Serial.print(" ");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dt.day()));
  Serial.print("/");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dt.month()));
  Serial.print("/");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dt.year()));
  Serial.print(" ");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dt.hour()));
  Serial.print(":");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dt.minute()));
  Serial.print(":");
  Serial.print(formateValeurSurDeuxChiffresMinimum(dt.second()));
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


// =================================================================================
// Fonction : xxxxxxxxx
// (permet xxxxxxxxx)
// =================================================================================
boolean estOnEnHeureDeEte(DateTime dateHeureAanalyser) {

/*
 * En France, le passage à l'heure d'hiver s'effectue le dernier dimanche d'octobre,
 * et le passage à l'heure d'été, le dernier dimanche de mars.
 * 
 * À la fin mars, on ajoute 1 heure, dans la nuit de samedi à dimanche.
 * Le changement se fait à 2 heures du matin, où il sera alors 3h du matin.
 * 
 * À la fin d'octobre, on retire 1 heure, dans la nuit de samedi à dimanche. 
 * Le changement se fait à 3 heures du matin, où il sera alors 2h du matin.
 * 
 * À noter que ce changement d'heure va possiblement disparaître, dans les années à venir.
 * Mais là pour l'instant, en 2023, il est toujours "en cours".
 * 
*/

  // Variables temporaires de cette fonction
  boolean estAvantLeDernierDimancheDeMars;
  boolean estApresLeDernierDimancheDeOctobre;

  // Récupération temporaire des données dont nous aurons besoin
  int anneeDeLaDateHeureAanalyser = dateHeureAanalyser.year();
  int moisDeLaDateHeureAanalyser = dateHeureAanalyser.month();
  int jourDeLaDateHeureAanalyser = dateHeureAanalyser.day();
  int heuresDeLaDateHeureAanalyser = dateHeureAanalyser.hour();
  int minutesDeLaDateHeureAanalyser = dateHeureAanalyser.minute();

  // Recherche du "numéro" de jour correspondant au dernier dimanche du mois de mars (de l'année de la date à analyser)
  int dernierDimancheDuMoisDeMars;
  for(int i=31; i >= 1; i--) {
    DateTime jourDeMars (anneeDeLaDateHeureAanalyser, 3, i, 0, 0, 0);       // Paramètres = année, mois, jour, heures, minutes, secondes
    if(jourDeMars.dayOfTheWeek() == 0) {
      dernierDimancheDuMoisDeMars = i;  // "dayOfTheWeek == 0" signifiant que nous sommes un "dimanche", avec la librairie RTClib,
      break;                            // et le "break" permet alors de quitter cette boucle for, comme nous avons trouvé le dernier dimanche du mois
    }
  }

  // Recherche du "numéro" de jour correspondant au dernier dimanche du mois d'octobre (de l'année de la date à analyser)
  int dernierDimancherDuMoisDeOctobre;
  for(int i=31; i >= 1; i--) {
    DateTime jourDeOctobre (anneeDeLaDateHeureAanalyser, 10, i, 0, 0, 0);   // Paramètres = année, mois, jour, heures, minutes, secondes
    if(jourDeOctobre.dayOfTheWeek() == 0) {
      dernierDimancherDuMoisDeOctobre = i;  // "dayOfTheWeek == 0" signifiant que nous sommes un "dimanche", avec la librairie RTClib,
      break;                                // et le "break" permet alors de quitter cette boucle for, comme nous avons trouvé le dernier dimanche du mois
    }
  }

  // On teste pour savoir si on est avant le dernier dimanche de mars, et avant 3h du matin
  if((moisDeLaDateHeureAanalyser <= 3) && (jourDeLaDateHeureAanalyser <= dernierDimancheDuMoisDeMars) && (heuresDeLaDateHeureAanalyser < 3))
    estAvantLeDernierDimancheDeMars = true;
  else
    estAvantLeDernierDimancheDeMars = false;

  // On teste pour savoir si on est après le dernier dimanche d'octobre, et après 3h du matin
  // (remarque : surtout ne pas faire l'upload entre 2 et 3h du mat, ce jour là en particulier, sinon ça va dysfonctionner !)
  if((moisDeLaDateHeureAanalyser >= 10) && (jourDeLaDateHeureAanalyser >= dernierDimancherDuMoisDeOctobre) && (heuresDeLaDateHeureAanalyser >= 3))
    estApresLeDernierDimancheDeOctobre = true;
  else
    estApresLeDernierDimancheDeOctobre = false;

  // Et on retourne le résultat
  if(estAvantLeDernierDimancheDeMars || estApresLeDernierDimancheDeOctobre)
    return false;       // Car là, on serait en "heure d'hiver"
  else
    return true;        // Car là, on serait en "heure d'été"

}
