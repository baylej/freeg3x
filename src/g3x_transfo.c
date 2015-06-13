#include <g3x_transfo.h>
#include <g3x_macros.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

void g3x_MakeIdentity(G3Xhmat A) {
  G3Xloadidentity(A);
}

void g3x_MakeTranslationV(G3Xhmat A, G3Xvector t) {
  g3x_MakeTranslationXYZ(A, t[0], t[1], t[2]);
}

void g3x_MakeTranslationXYZ(G3Xhmat A, double tx, double ty, double tz) {
  g3x_MakeIdentity(A);
  A[a03] = tx;
  A[a13] = ty;
  A[a23] = tz;
}

void g3x_MakeHomothetieV(G3Xhmat A, G3Xvector h) {
  g3x_MakeHomothetieXYZ(A, h[0], h[1], h[2]);
}

void g3x_MakeHomothetieXYZ(G3Xhmat A, double hx, double hy, double hz) {
  g3x_MakeIdentity(A);
  A[a00] = hx;
  A[a11] = hy;
  A[a22] = hz;
}

void g3x_MakeRotationX(G3Xhmat A, double alpha) {
  g3x_MakeIdentity(A);
  A[a11] =   A[a22] = cos(alpha);
  A[a12] = -(A[a21] = sin(alpha));
}

void g3x_MakeRotationY(G3Xhmat A, double alpha) {
  g3x_MakeIdentity(A);
  A[a00] =   A[a22] = cos(alpha);
  A[a02] = -(A[a20] = sin(alpha));
}

void g3x_MakeRotationZ(G3Xhmat A, double alpha) {
  g3x_MakeIdentity(A);
  A[a00] =   A[a11] = cos(alpha);
  A[a01] = -(A[a10] = sin(alpha));
}

void g3x_ProdHMatVector(G3Xhmat A, G3Xvector V, G3Xvector W) {
  W[0] = A[a00] * V[0] + A[a01] * V[1] +A[a02] * V[2] + A[a03];
  W[1] = A[a10] * V[0] + A[a11] * V[1] +A[a12] * V[2] + A[a13];
  W[2] = A[a20] * V[0] + A[a21] * V[1] +A[a22] * V[2] + A[a23];
}

void g3x_ProdHMatPoint(G3Xhmat A, G3Xpoint P, G3Xpoint Q) {
  g3x_ProdHMatXYZ(A, P[0], P[1], P[2], Q);
}

void g3x_ProdHMatXYZ(G3Xhmat A, double x, double y, double z, G3Xcoord Q) {
  Q[0] = A[a00] * x + A[a01] * y +A[a02] * z;
  Q[1] = A[a10] * x + A[a11] * y +A[a12] * z;
  Q[2] = A[a20] * x + A[a21] * y +A[a22] * z;
}

void g3x_ProdHMat(G3Xhmat A, G3Xhmat B, G3Xhmat C) {
  C[a00] = A[a00] * B[a00] + A[a01] * B[a10] + A[a02] * B[a20] + A[a03] * B[a30];
  C[a01] = A[a00] * B[a01] + A[a01] * B[a11] + A[a02] * B[a21] + A[a03] * B[a31];
  C[a02] = A[a00] * B[a02] + A[a01] * B[a12] + A[a02] * B[a22] + A[a03] * B[a32];
  C[a03] = A[a00] * B[a03] + A[a01] * B[a13] + A[a02] * B[a23] + A[a03] * B[a33];
  C[a10] = A[a10] * B[a00] + A[a11] * B[a10] + A[a12] * B[a20] + A[a13] * B[a30];
  C[a11] = A[a10] * B[a01] + A[a11] * B[a11] + A[a12] * B[a21] + A[a13] * B[a31];
  C[a12] = A[a10] * B[a02] + A[a11] * B[a12] + A[a12] * B[a22] + A[a13] * B[a32];
  C[a13] = A[a10] * B[a03] + A[a11] * B[a13] + A[a12] * B[a23] + A[a13] * B[a33];
  C[a20] = A[a20] * B[a00] + A[a21] * B[a10] + A[a22] * B[a20] + A[a23] * B[a30];
  C[a21] = A[a20] * B[a01] + A[a21] * B[a11] + A[a22] * B[a21] + A[a23] * B[a31];
  C[a22] = A[a20] * B[a02] + A[a21] * B[a12] + A[a22] * B[a22] + A[a23] * B[a32];
  C[a23] = A[a20] * B[a03] + A[a21] * B[a13] + A[a22] * B[a23] + A[a23] * B[a33];
  C[a30] = A[a30] * B[a00] + A[a31] * B[a10] + A[a32] * B[a20] + A[a33] * B[a30];
  C[a31] = A[a30] * B[a01] + A[a31] * B[a11] + A[a32] * B[a21] + A[a33] * B[a31];
  C[a32] = A[a30] * B[a02] + A[a31] * B[a12] + A[a32] * B[a22] + A[a33] * B[a32];
  C[a33] = A[a30] * B[a03] + A[a31] * B[a13] + A[a32] * B[a23] + A[a33] * B[a33];
}

void g3x_MPrintHMat(G3Xhmat A) {
  int i;
  for (i=0; i<4; i++) {
    printf("% 4.3f  % 4.3f  % 4.3f  % 4.3f\n", A[i], A[i+4], A[i+8], A[i+12]);
  }
  putchar('\n');
}

