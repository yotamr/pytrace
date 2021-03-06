#include <stdlib.h>
#include "Python.h"
#include "frameobject.h"
#include "writer.h"

#define MODULE_DOC PyDoc_STR("C extension for fast function tracing.")

static int
trace_func(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg)
{
  switch (what) {
  case PyTrace_CALL:
    handle_call(frame);
    break;
  case PyTrace_RETURN:
    handle_return(frame, arg);
    break;
  case PyTrace_EXCEPTION:
    handle_exception(frame, arg);
  }
  return 0;
}

static PyObject *
start(PyObject *self, PyObject *args)
{
  PyEval_SetProfile((Py_tracefunc)trace_func,
		    (PyObject*)self);
  return Py_BuildValue("");
}

static PyObject *
stop(PyObject *self, PyObject *args_unused)
{
  PyEval_SetProfile(NULL, NULL);
  return Py_BuildValue("");
}

static PyMethodDef
methods[] = {
  {"start", (PyCFunction) start, METH_VARARGS, PyDoc_STR("Start the tracer")},
  {"stop", (PyCFunction) stop, METH_VARARGS, PyDoc_STR("Stop the tracer")},
  {NULL}
};

void
inittracer(void)
{
  init_writer();
  Py_InitModule3("cpytrace.tracer", methods, MODULE_DOC);
}
