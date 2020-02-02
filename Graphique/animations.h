#ifndef _ANIMATIONS_H

#define _ANIMATIONS_H

  extern void transition_vide(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void transition_fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void transition_image(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);

  extern void copyFromSurface(SDL_Surface * s, int x0, int y0);
  extern void animation(int state);
  extern void animation_1(int state);
  extern void animation_2(int state);
  extern void animationsInit(void);

  extern void music_simple(int state);
  extern void music_fft(int state);
  extern void mix0105(int state);
  extern void text(int state);

#endif