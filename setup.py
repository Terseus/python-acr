from setuptools import Extension, setup

setup(ext_modules=[Extension(name="_libacr", sources=["ext/libacr.c"])])