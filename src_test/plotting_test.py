
import numpy
import pylab

import ECGFilters_wrap


def filter_signal(input_array):
    qrs_filter = ECGFilters_wrap.qrs_filter_wrap()
    
    output_array = numpy.zeros(input_array.shape, dtype=int)
    
    respirations = []
    
    for i, input_sample in enumerate(input_array):
        respiration_phase, signal_value = qrs_filter.step(input_sample)
        
        if respiration_phase != -1:
            respirations.append((i, respiration_phase))
        
        output_array[i] = signal_value
    
    return output_array, respirations


def main():
    input_array = numpy.load("../test_data/ecg_signal.npy")
    
    output_array, respirations = filter_signal(input_array)
    
    respiration_x, respiration_y = zip(*respirations)
    
    pylab.subplot(2,1,1)
    pylab.plot(input_array)
    pylab.plot(output_array)
    pylab.subplot(2,1,2)
    pylab.plot(respiration_x, respiration_y, "r")
    pylab.axhline(0, color="black")
    pylab.axhline(255, color="black")
    pylab.show()


if __name__ == '__main__':
    main()

