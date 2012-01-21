
import numpy
import time

import ECGFilters_wrap


def main():
    input_array = numpy.load("../test_data/ecg_signal.npy")
    
    qrs_filter = ECGFilters_wrap.qrs_filter_wrap()
    
    sample_interval = 1.0 / 200.0
    
    for i, input_sample in enumerate(input_array):
        respiration_phase, signal_value = qrs_filter.step(input_sample)
        
        if respiration_phase != -1:
            print respiration_phase
        
        time.sleep(sample_interval)


if __name__ == "__main__":
    main()
