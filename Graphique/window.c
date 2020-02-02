#include <stdlib.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4duw_SDL2.h>
#include "animations.h"
#include "audioHelper.h"


// Prototypes des fonctions statiques contenues dans ce fichier C.
static void init(void);
static void quit(void);
static void resize(int w, int h);
static void keydown(int keycode);


// Tableau contenant les animations sous la forme de timeline
static GL4DHanime _animations[] = {
  { 20000, mix0105, NULL, NULL },
  { 5000, mix0105, music_fft, transition_vide},
  { 5000, music_fft, NULL, NULL },
  { 10000, music_fft, music_simple, transition_fondu},
  { 5000, music_simple, NULL, NULL},
  { 5000, music_simple, animation, transition_fondu},
  { 1000, animation, NULL, NULL},
  { 1000, animation, animation_1, transition_fondu},
  { 1000, animation_1, NULL, NULL},
  {  500, animation_1, animation_2, transition_fondu},
  { 2500, animation_2, NULL, NULL},
  //{ 5000, animation, text, transition_image},
  //{ 5000, text, NULL, NULL},
  {    0, NULL, NULL, NULL }
};


// Dimensions initiales de la fenêtre
static GLfloat _dim[] = {1024, 1024};


// Fonction principale: initialise la fenêtre, OpenGL, audio et lance la boucle principale (infinie).
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "HUARD Alicia", 0, 0, _dim[0], _dim[1], GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwKeyDownFunc(keydown);
  gl4duwDisplayFunc(gl4dhDraw);

  // Trouve dans le dossier Audio la musique
  ahInitAudio("audio/ShindouSatoshi.mp3");
  gl4duwMainLoop();
  return 0;
}


// Fonction initialisation des paramètres et éléments OpenGL ainsi que les divers données et fonctionnalités liées à la gestion des animations.
static void init(void) {
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  gl4dhInit(_animations, _dim[0], _dim[1], animationsInit);
  resize(_dim[0], _dim[1]);
}


// Paramétre la vue (viewPort) OpenGL en fonction des dimensions de la fenêtre.
  // w largeur de la fenêtre.
  // h hauteur de la fenêtre.
static void resize(int w, int h) {
  _dim[0] = w; _dim[1] = h;
  glViewport(0, 0, _dim[0], _dim[1]);
}


// Permet de gérer les évènements clavier
static void keydown(int keycode) {
  switch(keycode) {
  case SDLK_ESCAPE:
  case 'q':
    exit(0);
  default: break;
  }
}


// Sortie du programme
static void quit(void) {
  ahClean();
  gl4duClean(GL4DU_ALL);
}