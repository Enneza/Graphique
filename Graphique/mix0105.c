#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <GL4D/gl4dh.h>
#include "audioHelper.h"


static void init(void);
static void loadTexture(GLuint id, const char * filename);
static void draw(void);
static void quit(void);


static GLuint _pId = 0, _pId2 = 0;
static GLuint _sphere = 0, _quad = 0, _tex[3] = {0};


static void init(void) {
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);

  _pId  = gl4duCreateProgram("<vs>shaders/depTex.vs", "<fs>shaders/depTex.fs", NULL);
  _pId2  = gl4duCreateProgram("<vs>shaders/effet.vs", "<fs>shaders/effet.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  _sphere = gl4dgGenSpheref(30, 30);
  _quad = gl4dgGenQuadf();
  glGenTextures(sizeof _tex / sizeof *_tex, _tex);
  loadTexture(_tex[0], "images/sphere.jpg");
  loadTexture(_tex[2], "images/quad.jpg");

  glBindTexture(GL_TEXTURE_2D, _tex[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2] / 4, vp[3] / 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glEnable(GL_TEXTURE_2D);
}


static void loadTexture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}


static void draw(void) {
  GLint vp[4];
  GLboolean dt = glIsEnabled(GL_DEPTH_TEST);
  glGetIntegerv(GL_VIEWPORT, vp);
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * vp[3] / vp[2], 0.5 * vp[3] / vp[2], 1.0, 1000.0);
  gl4duBindMatrix("modelViewMatrix");

  gl4duTranslatef(0, -1, 0);
  gl4duSendMatrices();
  glBindTexture(GL_TEXTURE_2D, _tex[0]);
  gl4dgDraw(_sphere);

  glUseProgram(0);

  gl4dfConvFrame2Tex(&_tex[2]);
  glUseProgram(_pId2);
  glBindTexture(GL_TEXTURE_2D, _tex[2]);
  glUniform1f(glGetUniformLocation(_pId2, "temps"), gl4dGetElapsedTime());
  gl4dgDraw(_quad);
  gl4dfConvFrame2Tex(&_tex[2]);
  gl4dfConvTex2Frame(_tex[2]);
  
  if(!dt)
    glDisable(GL_DEPTH_TEST);
}


static void quit(void) {
  glDeleteTextures(sizeof _tex / sizeof *_tex, _tex);
}


void mix0105(int state) {
  switch(state) {
  case GL4DH_INIT:
    init();
    return;
  case GL4DH_FREE:
    quit();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    return;
  default: /* GL4DH_DRAW */
    draw();
    return;
  }
}