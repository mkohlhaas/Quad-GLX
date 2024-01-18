#include <GL/glu.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>

void DrawAQuad() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1., 1., -1., 1., 1., 20.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

  glBegin(GL_QUADS);
  glColor3f(1., 0., 0.);
  glVertex3f(-.75, -.75, 0.);
  glColor3f(0., 1., 0.);
  glVertex3f(.75, -.75, 0.);
  glColor3f(0., 0., 1.);
  glVertex3f(.75, .75, 0.);
  glColor3f(1., 1., 0.);
  glVertex3f(-.75, .75, 0.);
  glEnd();
}

int main(void) {

  Display *dpy = XOpenDisplay(NULL);

  if (dpy == NULL) {
    printf("\n\tcannot connect to X server\n\n");
    exit(EXIT_SUCCESS);
  }

  Window root = DefaultRootWindow(dpy);

  GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
  XVisualInfo *vi = glXChooseVisual(dpy, 0, att);

  if (vi == NULL) {
    printf("No appropriate visual found\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("Visual %p selected.\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
  }

  Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

  XSetWindowAttributes swa;
  swa.colormap = cmap;
  swa.event_mask = ExposureMask | KeyPressMask;

  Window win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

  XMapWindow(dpy, win);
  XStoreName(dpy, win, "VERY SIMPLE APPLICATION");

  GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
  glXMakeCurrent(dpy, win, glc);

  glEnable(GL_DEPTH_TEST);

  XWindowAttributes gwa;
  while (True) {
    XEvent xev;
    XNextEvent(dpy, &xev);

    switch (xev.type) {
    case Expose:
      XGetWindowAttributes(dpy, win, &gwa);
      glViewport(0, 0, gwa.width, gwa.height);
      DrawAQuad();
      glXSwapBuffers(dpy, win);
      break;
    case KeyPress:
      glXMakeCurrent(dpy, None, NULL);
      glXDestroyContext(dpy, glc);
      XDestroyWindow(dpy, win);
      XCloseDisplay(dpy);
      exit(EXIT_SUCCESS);
      break;
    }
  }
}
