#include <g3x_basix.h>
#include <g3x_transfo.h>
#include <g3x_macros.h>

#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

/* freeglut inclut Windef.h qui définit near et far */
#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

/*!        PRIVATE ELEMENTS        !*/

static bool g3x_init_done = false;

static int screen_w, screen_h;

static void reshaped(int w, int h) {
  glViewport(0, 0, w, h);
  screen_w = w;
  screen_h = h;
}

static G3Xpoint cam_pos, cam_tar;
static G3Xcamera camera;

static void camera_spheric_to_cartesian() {
  camera.pos = &cam_pos;
  camera.tar = &cam_tar;

  G3Xpoint p = {camera.dist, 0.d, 0.d};

  G3Xhmat A, B, C;
  g3x_MakeRotationZ(A, camera.theta);
  g3x_MakeRotationY(B, camera.phi);
  g3x_ProdHMat(A, B, C);

  g3x_MakeTranslationXYZ(B, cam_tar[0], cam_tar[1], cam_tar[2]);
  g3x_ProdHMat(C, B, A);

  g3x_ProdHMatPoint(A, p, cam_pos);
}

static void (*anim_f)(void) = NULL;
static void (*init_f)(void) = NULL;
static void (*draw_f)(void) = NULL;
static void (*exit_f)(void) = NULL;

static bool anim_running = false;

static void draw() {
  if (!draw_f) return;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, camera.open, camera.near, camera.far);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt((*(camera.pos))[0],  (*(camera.pos))[1],  (*(camera.pos))[2],
            (*(camera.tar))[0],  (*(camera.tar))[1],  (*(camera.tar))[2],
                 camera.upv[0],       camera.upv[1],       camera.upv[2]);
  draw_f();
  glutSwapBuffers();

  GLenum e = glGetError();
  if (e != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error %d occured, aborting\n", e);
    glutLeaveMainLoop();
  }
}

static int ref_freq = 33;
static void idle() {
  if (anim_f && anim_running) anim_f();
  glutPostRedisplay();
}

void toggle_anim(void) {
  anim_running = !anim_running;
}

void (*mouse_move_action)(G3Xpoint) = NULL;

static G3Xpoint mouse_last_clic;
static G3Xpoint mouse_pos;
static bool mousebutton[256];
static void (*key_actions[256])(void);

static void keyboard_listener(unsigned char key, int x, int y) {
  if (key_actions[key])
    key_actions[key]();
  else
    printf("no registered action for key '%c'(0x%2X)\n", key, key);
}

static void mouse_grab_listener(int x, int y) {
  camera.theta += (mouse_pos[0] - x) * 0.01d;
  camera.phi   += (y - mouse_pos[1]) * 0.01d;
  camera_spheric_to_cartesian();

  mouse_pos[0] = x;
  mouse_pos[1] = y;
}

static void mouse_move_listener(int x, int y) {
  mouse_pos[0] = x;
  mouse_pos[1] = y;
  if (mouse_move_action)
    mouse_move_action(mouse_pos);
}

static void mouse_clic_listener(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    mousebutton[button] = true;
    mouse_last_clic[0] = x;
    mouse_last_clic[1] = y;
  } else {
    mousebutton[button] = false;
  }
}

/*!        PUBLIC  ELEMENTS        !*/

void g3x_AutoSnap_jpg(void) {
  // NYI Non implémenté
}

void g3x_AutoSnap_png(void) {
  // NYI Non implémenté
}

void g3x_SetBkgCol(double bkg) {
  // TODO: un seul double pour une couleur?
}

double g3x_GetBkgCol(void) {
  return 0.d; // TODO: voir g3x_SetBkgCol
}

void g3x_Arrow(float r, float h, G3Xcolor col) {
  G3Xpoint orig = {0.d, 0.d, 0.d};
  G3Xvector orient = {1.d, 0.d, 0.d};
  g3x_ArrowVect(orig, orient, r, h, col);
}

void g3x_ArrowVect(G3Xpoint O, G3Xvector V, float r, float h, G3Xcolor col) {
  glColor3f(col[0], col[1], col[2]);
  glBegin(GL_LINES);
    glVertex3f(       O[0],         O[1],         O[2]);
    glVertex3f(O[0]+V[0]*h,  O[1]+V[1]*h,  O[2]+V[2]*h);
  glEnd();
}

void g3x_Axis(void) {
  G3Xpoint orig = {0.d, 0.d, 0.d};

  G3Xvector orientX = {1.d, 0.d, 0.d};
  G3Xcolor     colX = {1.d, 0.d, 0.d, 1.d};
  g3x_ArrowVect(orig, orientX, 1., 1., colX);

  G3Xvector orientY = {0.d, 1.d, 0.d};
  G3Xcolor     colY = {0.d, 1.d, 0.d, 1.d};
  g3x_ArrowVect(orig, orientY, 1., 1., colY);

  G3Xvector orientZ = {0.d, 0.d, 1.d};
  G3Xcolor     colZ = {0.d, 0.d, 1.d, 1.d};
  g3x_ArrowVect(orig, orientZ, 1., 1., colZ);
}

void g3x_SetAutoRotationZ(double angle) {
  // NYI Non implémenté
}

void g3x_SetAutoDisplay(bool flag) {
  // Déprécié
}

void g3x_ToggleAutoDisplay(void) {
  // Déprécié
}

G3Xcamera* g3x_GetCamera(void) {
  return &camera;
}

double g3x_GetCameraDist(void) {
  return camera.dist;
}

void g3x_SetPerspective(float n, float f, float o) {
  camera.near = n;
  camera.far  = f;
  camera.open = o;
}

void g3x_SetCameraTheta(double theta) {
  camera.theta = theta;
  camera_spheric_to_cartesian();
}

void g3x_SetCameraPhi(double phi) {
  camera.phi = phi;
  camera_spheric_to_cartesian();
}

void g3x_SetCameraDist(double dist) {
  camera.dist = dist;
  camera_spheric_to_cartesian();
}

void g3x_SetCameraSpheric(double theta, double phi, double dist, G3Xpoint target) {
  memcpy(cam_tar, target, sizeof(G3Xpoint));
  camera.theta = theta;
  camera.phi = phi;
  camera.dist = dist;
  camera_spheric_to_cartesian();
}

void g3x_SetCameraCartesian(G3Xpoint position, G3Xpoint target) {
  G3Xvector xv = {1.d, 0.d, 0.d}, zv = {0.d, 0.d, 1.d};
  G3Xvector tp_theta, pt_phi;
  double sign;

  camera.pos = &cam_pos;
  memcpy(cam_pos, position, sizeof(G3Xpoint));

  camera.tar = &cam_tar;
  memcpy(cam_tar, target, sizeof(G3Xpoint));

  camera.dist = G3Xdist(cam_tar, cam_pos);

  G3Xsetvct(tp_theta, target, position);
  G3Xcopy(pt_phi, tp_theta);

  // Admet que camera.upv est Zi
  tp_theta[2] = 0.d;
  sign = (atan(G3Xprodvct2(tp_theta, xv)) >= 0)? -1.d: 1.d;
  camera.theta = sign * acos(G3Xprodscal(tp_theta, xv)/G3Xvnorm(tp_theta));

  camera.phi = PI_2 - acos(G3Xprodscal(pt_phi, zv)/G3Xvnorm(pt_phi));

  printf("theta=%f   phi=%f\n", camera.theta, camera.phi);
}

void g3x_SetCameraTracking(G3Xpoint *position, G3Xpoint *target) {
  camera.pos = position;
  camera.tar = target;
}

void g3x_SetLightAmbient(float x, float y, float z) {
  // NYI Non implémenté
}

void g3x_SetLightDiffuse(float x, float y, float z) {
  // NYI Non implémenté
}

void g3x_SetLightSpecular(float x, float y, float z) {
  // NYI Non implémenté
}

void g3x_SetLightPosition(float x, float y, float z) {
  // NYI Non implémenté
}

void g3x_SetLightDirection(float x, float y, float z) {
  // NYI Non implémenté
}

void g3x_SetSpotSteps(float dx, float dy, float dz) {
  // NYI Non implémenté
}

bool g3x_Running(void) {
  return anim_running;
}

void g3x_Stop(void) {
  anim_running = false;
}

void g3x_Continue(void) {
  anim_running = true;
}

void g3x_InitWindow(char* windname, int w, int h) {
  int argc = 1; char *argv[] = {windname, NULL};
  g3x_init_done = true;

  memset(&camera, 0, sizeof(camera));
  G3Xset(camera.upv, 0.d, 0.d, 1.d);

  memset(key_actions, 0, 256*sizeof(void*));
  key_actions[0x1B] = g3x_Quit;     // ESC
  key_actions[0x20] = toggle_anim;  // Space
  glutInit(&argc, argv);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(w, h);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow(windname);
  glutReshapeFunc(reshaped);

  glutKeyboardFunc(keyboard_listener);
  glutMouseFunc(mouse_clic_listener);
  glutMotionFunc(mouse_grab_listener);
  glutPassiveMotionFunc(mouse_move_listener);
}

void g3x_SetInitFunction(void (*f)(void)) {
  init_f = f;
}

void g3x_SetDrawFunction(void (*f)(void)) {
  draw_f = f;
}

void g3x_Show(void) {
  glutPostRedisplay();
}

void g3x_SetAnimFunction(void (*f)(void)) {
  anim_f = f;
}

void g3x_SetExitFunction(void (*f)(void)) {
  exit_f = f;
}

void g3x_SetRefreshFreq(int freq) {
  ref_freq = 1/(float)freq * 1000;
}

int g3x_MainStart(void) {
  if (init_f) init_f();
  glutDisplayFunc(draw);

  glutIdleFunc(idle);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutMainLoop();
  if (exit_f) exit_f();
  return 0;
}

void g3x_Quit(void) {
  glutLeaveMainLoop();
}

void g3x_AutoStartVideo(void) {
  // NYI Non implémenté
}

void g3x_Write(char* msg, int x, int y, G3Xcolor col) {
  GLint matrix_stack_state;
  glGetIntegerv(GL_MATRIX_MODE, &matrix_stack_state);

  // Assignes les matrices pour le rendu du texte
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity(); // Coordonnées écran: [-1: 1]

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glColor3f(col[0], col[1], col[2]);
  glRasterPos2f(x/(float)screen_w*2.-1, (y/(float)screen_h*2.-1)*-1.);
  glutBitmapString(GLUT_BITMAP_HELVETICA_10, (unsigned char*)msg);

  // rétablit les matrices
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode((GLenum)matrix_stack_state);
}

bool g3x_SetControlParameter_i(int *param, char* name, int pas, int min, int max, char mode) {
  return false; // NYI Non implémenté
}

bool g3x_SetControlParameter_d(double *param, char* name, double pas, double min, double max, char mode) {
  return false; // NYI Non implémenté
}

bool g3x_ChangeControlParameter(void *param) {
  return false; // NYI Non implémenté
}

void g3x_SetMouseMoveAction(void (*action)(G3Xpoint)) {
  mouse_move_action = action;
}

void g3x_GetMousePosition(G3Xpoint mousepos) {
  mousepos[0] = mouse_pos[0];
  mousepos[1] = mouse_pos[1];
}

bool g3x_GetClic(G3Xpoint mouseclic) {
  mouseclic[0] = mouse_last_clic[0];
  mouseclic[1] = mouse_last_clic[1];
  return true;
}

int g3x_CreateButton(char *text, char *info) {
  return 0; // NYI Non implémenté
}

int g3x_GetButton(void) {
  return 0; // NYI Non implémenté
}

bool g3x_CreateSwitch(char *txt, bool *flag, char *info) {
  return false; // NYI Non implémenté
}

int g3x_GetSwitch(void) {
  return 0; // NYI Non implémenté
}

bool g3x_CreatePopUp(char *name, void (*action)(void), char *info) {
  return false; // NYI Non implémenté
}

int g3x_GetPopUp(void) {
  return 0; // NYI Non implémenté
}

bool g3x_SetKeyAction(char key, void (*action)(void), char *info) {
  key_actions[(int)key] = action;
  return true;
}

int g3x_CreateScrollh_d(char *nom, double* prm, double min, double max, double mapfactor, char *info) {
  return 0; // NYI Non implémenté
}

int g3x_CreateScrollh_i(char *nom, int* prm, int min, int max, double mapfactor, char *info) {
  return 0; // NYI Non implémenté
}

int g3x_CreateAnonymousScrollh(char *name, double init, char *info) {
  return 0; // NYI Non implémenté
}

int g3x_CreateScrollv_d(char *nom, double* param, double min, double max, double mapfactor, char *info) {
  return 0; // NYI Non implémenté
}

int g3x_CreateScrollv_i(char *nom, int* prm, int min, int max, double mapfactor, char *info) {
  return 0; // NYI Non implémenté
}

int g3x_CreateAnonymousScrollv(char *name, double init, char *info) {
  return 0; // NYI Non implémenté
}

double g3x_GetScrollCursor(int id) {
  return 0.d; // NYI Non implémenté
}

void g3x_SetScrollWidth(int width) {
  // NYI Non implémenté
}

