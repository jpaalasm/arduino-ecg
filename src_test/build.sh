#!/bin/sh

python setup.py build_ext --inplace
rm -fr build ECGFilters_wrap.cpp
