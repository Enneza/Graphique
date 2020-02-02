/*!\file window.c
 * \brief affichage du spectre sonore
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date April 15 2016 */
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dp.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_mixer.h>
#include "audioHelper.h"


static void draw(void);
static void quit(void);


#define ECHANTILLONS 1024


static Sint16 _hauteurs[ECHANTILLONS];
static GLuint _screen = 0;


static void draw(void) {
  int i;
  gl4dpSetColor(RGB(255, 255, 255));
  gl4dpSetScreen(_screen);
  gl4dpClearScreen();
  for(i = 0; i < ECHANTILLONS/*  - 1 */; i+=2) {
    int x0, y0;
    x0 = (i * (gl4dpGetWidth() - 1)) / (ECHANTILLONS - 1);
    y0 = _hauteurs[i];
    gl4dpPutPixel(x0, y0);

  }
  gl4dpSetColor(RGB(0, 0, 0));
  for(i = 1; i < ECHANTILLONS/*  - 1 */; i+=2) {
    int x0, y0;
    x0 = (i * (gl4dpGetWidth() - 1)) / (ECHANTILLONS - 1);
    y0 = _hauteurs[i];

    gl4dpPutPixel(x0, y0);
  }
  gl4dpUpdateScreen(NULL);
}


static void quit(void) {
  if(_screen) {
    gl4dpSetScreen(_screen);
    gl4dpDeleteScreen();
    _screen = 0;
  }
}


void music_simple(int state) {
  static int h = 1;
  switch(state) {
  case GL4DH_INIT:
    _screen = gl4dpInitScreen();
    h = gl4dpGetHeight();
    return;
  case GL4DH_FREE:
    quit();
    return;
  case GL4DH_UPDATE_WITH_AUDIO: {
    int i;
    Sint16 *s = (Sint16 *)ahGetAudioStream();
    if(ahGetAudioStreamLength() >= 2 * ECHANTILLONS)
      for(i = 0; i < ECHANTILLONS; i++)
	_hauteurs[i] =  h / 2 + (h / 2) * s[i] / ((1 << 15) - 1.0);
    return;
  }
  default: /* GL4DH_DRAW */
    draw();
    return;
  }
}