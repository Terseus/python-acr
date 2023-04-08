from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="_libacr",
            sources=["ext/libacr.c"],
            libraries=["pcsclite"],
            include_dirs=["/usr/include/PCSC"],
        )
    ]
)
