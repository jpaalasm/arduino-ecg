
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension("ECGFilters_wrap",
                             ["ECGFilters_wrap.pyx", "../src/ECGFilters/ECGFilters.cpp"],
                             include_dirs=["../src/ECGFilters"],
                             language="c++")],
      cmdclass={'build_ext': build_ext})
