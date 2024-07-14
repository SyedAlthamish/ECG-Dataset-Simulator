import machine
import utime

# Set up ADC pin
adc_pin = machine.ADC(28 )  # Replace with your desired ADC pin

# Set up sampling rate (in Hz)
sampling_rate = 500  # 100 Hz sampling rate

# Set up sampling period (in seconds)
sampling_period = 7  # 20 seconds sampling period

# Create arrays to store data and sample times
data_array = []
time_array = []

# Calculate the sampling interval (in microseconds)
sampling_interval = int(1e6 / sampling_rate)

# Start the timer
start_time = utime.ticks_us()

# Sample ADC data for the specified period
while utime.ticks_us() - start_time < sampling_period * 1e6:
    # Read ADC value
    adc_value = adc_pin.read_u16()

    # Append data to array
    data_array.append(adc_value)

    # Calculate sample time (in seconds)
    #sample_time = (utime.ticks_us() - start_time) / 1e6

    # Append sample time to array
    #time_array.append(sample_time)

    # Wait for the next sampling interval
    utime.sleep_us(sampling_interval)

# Print the data and time arrays
print(len(data_array))
print("Data Array:", data_array)
print("Time Array:", time_array)