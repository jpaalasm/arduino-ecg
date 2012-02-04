
cdef extern from "ECGFilters.h":
    cdef struct return_value:
        int respiration_phase
        int signal_value
    cdef cppclass qrs_filter:
        qrs_filter()
        return_value step(short int)


cdef class qrs_filter_wrap:
    cdef qrs_filter *filter
    def __cinit__(self):
        self.filter = new qrs_filter()
    
    def step(self, short int x):
        cdef return_value result = self.filter.step(x)
        return result.respiration_phase, result.signal_value
