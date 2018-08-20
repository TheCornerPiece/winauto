#define WINVER 0x0500
#include <windows.h>
#include <stdio.h>
#include <Python.h>


void typeChar(INPUT *ip, char c){
    ip->type = INPUT_KEYBOARD;
    ip->ki.dwFlags = KEYEVENTF_UNICODE; // Specify the key as a unicode character
    ip->ki.wScan = c; // Which keypress to simulate

    ip->ki.time = 0;
    ip->ki.dwExtraInfo = 0;
    ip->ki.wVk = 0;

    SendInput(1, ip, sizeof(*ip));

    ip->ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, ip, sizeof(*ip));
}

void pressKey(int key, int mod){
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wScan = 0;

    if (mod){
        ip.ki.dwFlags = 0;
        ip.ki.wVk = mod;
        SendInput(1, &ip, sizeof(ip));
    }

    ip.ki.dwFlags = KEYEVENTF_UNICODE; // Specify the key as a unicode character
    ip.ki.wVk = key;
    SendInput(1, &ip, sizeof(ip));

    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(ip));

    if (mod){
        ip.ki.wVk = mod;
        SendInput(1, &ip, sizeof(ip));
    }
}

int mouseClick(INPUT *ip, int button, int mode){
    ip->type=INPUT_MOUSE;
    ip->mi.dx=0;
    ip->mi.dy=0;
    ip->mi.dwFlags=(6-mode*2)<<(button*2);
    ip->mi.mouseData=0;
    ip->mi.dwExtraInfo=0;
    ip->mi.time=0;
    SendInput(1, ip, sizeof(*ip));
    return 1;
}

void typeString(char *text, int len) {
    INPUT ip;
    int i;
    for (i = 0; i < len; i++){
        typeChar(&ip, text[i]);
    }
}

int getPressed(int key){
    return GetKeyState(key) >> 8;
}

static PyObject *pyTypeStr(PyObject *self, PyObject *args) {
      char *text;
      int len;

      if (!PyArg_ParseTuple(args, "s#", &text, &len)) {
            return NULL;
      }

      typeString(text, len);
      return Py_BuildValue("i", 1);
}

static PyObject *pyPressKey(PyObject *self, PyObject *args) {
      int key, mode;

      if (!PyArg_ParseTuple(args, "ii", &key, &mode)) {
            return NULL;
      }

      pressKey(key, mode);
      return Py_BuildValue("i", 1);
}

static PyObject *pySetMousePos(PyObject *self, PyObject *args) {
      int x, y;

      if (!PyArg_ParseTuple(args, "ii", &x, &y)) {
            return NULL;
      }

      SetCursorPos(x, y);
      return Py_BuildValue("i", 1);
}

static PyObject *pyGetMousePos(PyObject *self) {
      POINT mousePos;
      GetCursorPos(&mousePos);
      return Py_BuildValue("ii", mousePos.x, mousePos.y);
}

static PyObject *pyClick(PyObject *self, PyObject *args) {
      // button (0 - lmb, 1 - rmb)
      // mode (0 - click/release, 1 - release, 2 - click/hold)
      INPUT ip;
      int button, mode;

      if (!PyArg_ParseTuple(args, "ii", &button, &mode)) {
            return NULL;
      }

      return Py_BuildValue("i", mouseClick(&ip, button, mode));
}

static PyObject *pyGetPressed(PyObject *self, PyObject *args){
      int key;

      if (!PyArg_ParseTuple(args, "i", &key)) {
            return NULL;
      }
      return Py_BuildValue("i", getPressed(key));
}

static PyMethodDef funcs[] = {
   {"press_key", (PyCFunction)pyPressKey, METH_VARARGS, NULL},
   {"type", (PyCFunction)pyTypeStr, METH_VARARGS, NULL},
   {"set_mouse_pos", (PyCFunction)pySetMousePos, METH_VARARGS, NULL},
   {"get_mouse_pos", (PyCFunction)pyGetMousePos, METH_NOARGS, NULL},
   {"click", (PyCFunction)pyClick, METH_VARARGS, NULL},
   {"get_pressed", (PyCFunction)pyGetPressed, METH_VARARGS, NULL},
   {NULL}
};

void initwinauto(void) {
   Py_InitModule3("winauto", funcs, "");
}


