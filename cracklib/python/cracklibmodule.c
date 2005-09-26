/*
 *  A Python binding for cracklib.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include PYTHON_H
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include "../lib/crack.h"

#ifdef HAVE_PTHREAD_H
static pthread_mutex_t cracklib_mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK() pthread_mutex_lock(&cracklib_mutex)
#define UNLOCK() pthread_mutex_unlock(&cracklib_mutex)
#else
#define LOCK()
#define UNLOCK()
#endif

static PyObject *
cracklib_FascistCheck(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int i;
    char *candidate, *dict;
    const char *result;
    struct stat st;
    char *keywords[] = {"pw", "dictpath", NULL};

    self = NULL;
    candidate = NULL;
    dict = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|s", keywords,
                                     &candidate, &dict))
    {
        PyErr_SetString(PyExc_ValueError, "error parsing arguments");
        return NULL;
    }

    if (candidate == NULL)
    {
        PyErr_SetString(PyExc_ValueError, "first argument was not a string!");
        return NULL;
    }
    if (dict != NULL)
    {
        if (dict[0] != '/')
        {
            PyErr_SetString(PyExc_ValueError,
                            "second argument was not an absolute path!");
            return NULL;
        }
        if (lstat(dict, &st) == -1)
        {
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, dict);
            return NULL;
        }
    } else
    {
        if (lstat(DEFAULT_CRACKLIB_DICT ".pwd", &st) == -1)
        {
            PyErr_SetFromErrnoWithFilename(PyExc_OSError,
                                           DEFAULT_CRACKLIB_DICT);
            return NULL;
        }
    }

    LOCK();
    result = FascistCheck(candidate, dict ? dict : DEFAULT_CRACKLIB_DICT);
    UNLOCK();

    if (result != NULL)
    {
        return PyString_FromString(result);
    } else
    {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyMethodDef
cracklibmethods[] =
{
    {"FascistCheck", cracklib_FascistCheck, METH_VARARGS | METH_KEYWORDS},
    {NULL, NULL},
};

void
initcracklib(void)
{
    Py_InitModule("cracklib", cracklibmethods);
}
