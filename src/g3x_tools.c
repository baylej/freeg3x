#include <g3x_tools.h>

#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>

char* g3x_ProcTimer(void) {
  return NULL; // TODO: quoi retourner?
}

char* g3x_Chrono(void) {
  return NULL; // TODO: quoi retourner?
}

void g3x_GetTime(G3Xclock* clock) {
  ; // NYI Non implémenté
}

double g3x_Rand_Percent(double root, double percent) {
  return 0.d; // NYI Non implémenté
}

double g3x_Rand_Delta(double root, double delta) {
  return 0.d; // NYI Non implémenté
}

double g3x_Radcos(double rad) {
  return cos(rad);
}

double g3x_Radsin(double rad) {
  return sin(rad);
}

double g3x_Degcos(double deg) {
  return cos(deg * PI / 180.d);
}

double g3x_Degsin(double deg) {
  return sin(deg * PI / 180.d);
}

void g3x_FillColorMap(G3Xcolor *map, int n) {
  // NYI Non implémenté
}

void g3x_FillColorMap_va(G3Xcolor *map, int n, float v, float a) {
  // NYI Non implémenté
}

float* g3x_GetColorIndex(int idx) {
  return NULL; // NYI Non implémenté
}

void g3x_SetIndexColor(G3Xcolor* map, int idx, G3Xcolor col) {
  // NYI Non implémenté
}

void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha) {
  float col_tmp[4];

  col_tmp[0] = col[0] * ambi;
  col_tmp[1] = col[1] * ambi;
  col_tmp[2] = col[2] * ambi;
  col_tmp[3] = alpha;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, col_tmp);

  col_tmp[0] = col[0] * diff;
  col_tmp[1] = col[1] * diff;
  col_tmp[2] = col[2] * diff;
  col_tmp[3] = alpha;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col_tmp);

  col_tmp[0] = col[0] * spec;
  col_tmp[1] = col[1] * spec;
  col_tmp[2] = col[2] * spec;
  col_tmp[3] = alpha;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, col_tmp);

  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0*shine);
}

