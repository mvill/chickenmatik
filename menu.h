#ifndef _MENU_H_
#define _MENU_H_
 
/* Structure d'un menu */
typedef struct {
  const char* prompt;     // Titre du menu
  const char** items;     // Tableau de choix du menu
  const uint8_t nbItems;  // Nombre de choix possibles
  void (*callbackFnct)(uint8_t menuItemSelected); // Pointeur sur fonction pour gérer le choix de l'utilisateur
} Menu_t;                                                             
 
/* Listes des touches de la shield lcd DFrobots */
/*
typedef enum {
  BP_NONE,   // Aucun bouton appuyé
  BP_SELECT, // Bouton SELECT
  BP_LEFT,   // Bouton gauche
  BP_RIGHT,  // Bouton droite
  BP_UP,     // Bouton haut
  BP_DOWN,    // Bouton bas
  BP_RST	 // Bouton RST
} Button_t;*/
 
/*
typedef enum {
  MAIN_SCREEN,
  MENU_SCREEN
} State_t;
*/

typedef struct {
  const int pin;
  const String label;     // Titre du menu
  int state;
} Button_t;
 
#endif /* _MENU_H_ */
