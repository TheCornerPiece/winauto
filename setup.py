import sys
from distutils.core import setup, Extension

static_libraries = ['user32']
static_lib_dir = '/system/lib'
libraries = []
library_dirs = ['/system/lib', '/system/lib64']
include_dirs = []

if sys.platform == 'win32':
    libraries.extend(static_libraries)
    library_dirs.append(static_lib_dir)
    extra_objects = []
else: # POSIX
    extra_objects = ['{}/lib{}.a'.format(static_lib_dir, l) for l in static_libraries]

ext = Extension('winauto',
                sources=['main.c'],
                libraries=libraries,
                library_dirs=library_dirs,
                include_dirs=include_dirs,
                extra_objects=extra_objects)   

setup(name='winauto', version='0.0.7', ext_modules=[ext])
