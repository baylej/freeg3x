#include <g3x_capture.h>

// NYI Non implémenté

void g3x_SetFrameRate(int frame_per_sec) {}

int g3x_GetFrameRate(void) {
  return 0;
}

void g3x_SetBitRate(int bit_rate) {}

int g3x_GetBitRate(void) {
  return 0;
}

void g3x_SetPid(int force_pid) {}

int g3x_GetPid(void) {
  return 0;
}

void g3x_SetMaxImage(int max) {}

int g3x_GetMaxImage(void) {
  return 0;
}

bool g3x_PlugCapture(char *basename, int downleftx, int downlefty, int uprightx, int uprighty) {
  return false;
}

void g3x_UnplugCapture(void) {}

bool g3x_Snapshot(char *format, char *basename, int w, int h) {
  return false;
}

bool g3x_FilmFrame(void) {
  return false;
}

bool g3x_MakeMpeg(void) {
  return false;
}

bool g3x_MakeAvi(void) {
  return false;
}

bool g3x_MakeMpeg4(void) {
  return false;
}

bool g3x_MakeFlv(void) {
  return false;
}

