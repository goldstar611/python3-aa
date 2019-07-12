#include <Python.h>
#include <sys/apparmor.h>
#include <fcntl.h>

static unsigned long magic_token = 0;

static PyObject*
enter_hat(PyObject* self, PyObject* args, PyObject* keywds)
{
    int aa_ret = -1;
    const char* hat_name = NULL;
    static char* kwlist[] = {"hat_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &hat_name)) {
        return NULL;
    }

    aa_ret = aa_change_hat(hat_name, magic_token);
    if (aa_ret < 0) {
        PyObject* err_string = PyUnicode_FromFormat("aa_change_hat('%s') call failed", hat_name);
        PyErr_SetObject(PyExc_OSError, err_string);
        Py_XDECREF(err_string);
        return NULL;
    }

    printf("aa_enter_hat called successfully\n");
    Py_RETURN_NONE;
}

static PyObject*
exit_hat(PyObject* self, PyObject* args)
{
    int aa_ret;

    aa_ret = aa_change_hat(NULL, magic_token);
    if (aa_ret < 0) {
        PyErr_SetString(PyExc_OSError, "aa_change_hatv() call failed");
        return NULL;
    }

    printf("aa_exit_hat called successfully\n");
    Py_RETURN_NONE;
}

static PyObject*
change_profile(PyObject* self, PyObject* args, PyObject* keywds)
{
    int aa_ret = -1;
    const char* profile_name = NULL;
    static char* kwlist[] = {"profile_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &profile_name)) {
        return NULL;
    }

    aa_ret = aa_change_profile(profile_name);
    if (aa_ret < 0) {
        PyObject* err_string = PyUnicode_FromFormat("aa_change_profile('%s') call failed", profile_name);
        PyErr_SetObject(PyExc_OSError, err_string);
        Py_XDECREF(err_string);
        return NULL;
    }

    printf("aa_change_profile called successfully\n");
    Py_RETURN_NONE;
}

static PyObject*
change_onexec(PyObject* self, PyObject* args, PyObject* keywds)
{
    int aa_ret = -1;
    const char* profile_name = NULL;
    static char* kwlist[] = {"profile_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &profile_name)) {
        return NULL;
    }

    aa_ret = aa_change_onexec(profile_name);
    if (aa_ret < 0) {
        PyObject* err_string = PyUnicode_FromFormat("aa_change_onexec('%s') call failed", profile_name);
        PyErr_SetObject(PyExc_OSError, err_string);
        Py_XDECREF(err_string);
        return NULL;
    }

    printf("aa_change_onexec called successfully\n");
    Py_RETURN_NONE;
}

static PyObject*
stack_profile(PyObject* self, PyObject* args, PyObject* keywds)
{
    int aa_ret = -1;
    const char* profile_name = NULL;
    static char* kwlist[] = {"profile_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &profile_name)) {
        return NULL;
    }

    aa_ret = aa_stack_profile(profile_name);
    if (aa_ret < 0) {
        PyObject* err_string = PyUnicode_FromFormat("aa_stack_profile('%s') call failed", profile_name);
        PyErr_SetObject(PyExc_OSError, err_string);
        Py_XDECREF(err_string);
        return NULL;
    }

    printf("aa_stack_profile called successfully\n");
    Py_RETURN_NONE;
}

static PyObject*
stack_onexec(PyObject* self, PyObject* args, PyObject* keywds)
{
    int aa_ret = -1;
    const char* profile_name = NULL;
    static char* kwlist[] = {"profile_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist,
                                     &profile_name)) {
        return NULL;
    }

    aa_ret = aa_stack_onexec(profile_name);
    if (aa_ret < 0) {
        PyObject* err_string = PyUnicode_FromFormat("aa_stack_onexec('%s') call failed", profile_name);
        PyErr_SetObject(PyExc_OSError, err_string);
        Py_XDECREF(err_string);
        return NULL;
    }

    printf("aa_stack_profile called successfully\n");
    Py_RETURN_NONE;
}

static PyObject*
is_enabled(PyObject* self, PyObject* args)
{
    if (aa_is_enabled()) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}

static int
init(void)
{
    size_t size = sizeof(magic_token);
    int fd;

    fd = open ("/dev/urandom", O_RDONLY);
    if (fd != -1) {
        if (!read(fd, (void*) &magic_token, size)) {
            PyErr_SetString(PyExc_OSError, "Failed to read /dev/urandom.");
            return -1;
        }
        close(fd);
    } else {
        PyErr_SetString(PyExc_OSError, "Failed to open /dev/urandom");
        return -1;
    }

    printf("aa_init called successfully\n");
    return 0;
}

static PyMethodDef SupportedMethods[] = {
    { "is_enabled",                  is_enabled,     METH_NOARGS,                  "Determines if apparmor is enabled." },
    { "change_hat",     (PyCFunction)enter_hat,      METH_VARARGS | METH_KEYWORDS, "If a portion of the program needs different access permissions than other portions, the program can \"change hats\" to a different role, also known as a subprofile." },
    { "exit_hat",                    exit_hat,       METH_NOARGS,                  "Exits the current hat and transitions back to the previous profile." },
    { "change_profile", (PyCFunction)change_profile, METH_VARARGS | METH_KEYWORDS, "An AppArmor profile applies to an executable program; if a portion of the program needs different access permissions than other portions, the program can \"change profile\" to a different profile. To change into a new profile, it can use the aa_change_profile() function to do so. It passes in a pointer to the profile to transition to. Confined programs wanting to use aa_change_profile() need to have rules permitting changing to the named profile." },
    { "change_onexec",  (PyCFunction)change_onexec,  METH_VARARGS | METH_KEYWORDS, "The aa_change_onexec() function is like the aa_change_profile() function except it specifies that the profile transition should take place on the next exec instead of immediately.  The delayed profile change takes precedence over any exec transition rules within the confining profile.  Delaying the profile boundary has a couple of advantages, it removes the need for stub transition profiles and the exec boundary is a natural security layer where potentially sensitive memory is unmapped." },
    { "stack_profile",  (PyCFunction)stack_profile,  METH_VARARGS | METH_KEYWORDS, "AppArmor supports stacking two or more profiles when confining a task. The result is an intersection of all profiles which are stacked. Stacking profiles together is desirable when wanting to ensure that confinement will never become more permissive. When changing between two profiles, as performed with aa_change_profile(2), there is always the possibility that the new profile is more permissive than the old profile but that possibility is eliminated when using aa_stack_profile()." },
    { "stack_onexec",   (PyCFunction)stack_onexec,   METH_VARARGS | METH_KEYWORDS, "The aa_stack_onexec() function is like the aa_stack_profile() function except it specifies that the stacking should take place on the next exec instead of immediately. The delayed profile change takes precedence over any exec transition rules within the confining profile. Delaying the stacking boundary has a couple of advantages, it removes the need for stub transition profiles and the exec boundary is a natural security layer where potentially sensitive memory is unmapped." },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef AA = {
    PyModuleDef_HEAD_INIT,
    "AA", /* name of module */
    "Python 3 module for changing AppArmor hats and profiles.", /* module documentation */
    -1,   /* size of per-interpreter state of the module,
             or -1 if the module keeps state in global variables. */
    SupportedMethods
};

PyMODINIT_FUNC PyInit_AA(void)
{
    if (init() != 0) {
        // init() will set the appropriate error description
        return NULL;
    }

    return PyModule_Create(&AA);
}
