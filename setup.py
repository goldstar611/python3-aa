#!/usr/bin/python3

from distutils.core import setup, Extension
setup(name = 'AA', version = '1.0',  \
   ext_modules = [Extension('AA', 
                            sources = ['aa.c'], 
                            libraries = ['apparmor'], 
                            library_dirs = ['/usr/lib/x86_64-linux-gnu'])])
