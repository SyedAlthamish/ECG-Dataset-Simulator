"""Importions """
import serial
import wfdb
import time
import matplotlib.pyplot as plt


"""Functions """
def startcomm(serialhandle):
    #Telling MC to be ready
    global Samplerate                                                                           #taking the global value of Samplerate
    serialhandle.write(b'H')                                                                    #writing a H for Hello to initiate convo
    Samplerate_bytes = Samplerate.to_bytes(1, byteorder='little', signed=False)          #converting raw int to bytes
    serialhandle.write(Samplerate_bytes)                                                        #writing to MC the sampling rate of the dataset to be sent
    #Waiting for MC to be ready
    while ser.in_waiting == 0:                                                                  # Check if there is data waiting in the buffer
        print("waiting for MC to respond...")                                                   #If no response by the MC
    datarec = ser.read(1).decode('utf-8')                                                       #If responded, then records it
    #Verifying if MC is Ready
    if datarec == "R":                                                                          # R for ready
        print("MC is ready")                                                                    # Inform the user of the MC's state
    else:
        raise Exception("The MC responded" + datarec)                                           # Inform the user of the MC's state


def send1data(datam, serialhndle):
    global datanumber
    serialhndle.write(datam[datanumber].to_bytes(1, byteorder='little', signed=False))
    #print("TX"+str(datam[datanumber]))


################Selecting The DB#################################
recordname = str(100)#str(input("Please select one record from 100-124 and 200-234"))


################Accessing   The   DB###################################
print("importion done")
record = wfdb.rdrecord(
    "C:\Althamish\Project\ECG Simulator\mit-bih-arrhythmia-database-1.0.0\mit-bih-arrhythmia-database-1.0.0\\" + recordname)
# Read the annotations
annotation = wfdb.rdann(
    "C:\Althamish\Project\ECG Simulator\mit-bih-arrhythmia-database-1.0.0\mit-bih-arrhythmia-database-1.0.0\\100",
    'atr')

all_samples = record.p_signal[:, 0]
print("Record accessed")
data = all_samples

#################Normalising Selected Data for 8bit DAC###########################
#print(data)
normaldata = ((data-min(data))/(max(data)-min(data))*(250-5))+(5)
normaldata = [int(x) for x in normaldata]
#print("Selected Data samples: ",normaldata)
downsampleddata=normaldata[0:len(normaldata):2]
# print(downsampleddata)
# print(len(downsampleddata),len(normaldata))
# plt.plot(downsampleddata)
# plt.plot(normaldata)
# plt.show()

#################Selecting the Srate of the db###########################
Samplerate = 250#int(input("Please enter the sampling rate of the dataset chosen"))

#################  Sending Data Master Code       ###############
# ######################Initialising the serial port#############################
try:
    serial_port = 'COM5'                    # Holds the port no of the serial peripheral
    baud_rate = 9600                        # The transception baud rate
    ser = serial.Serial(                    # Creates a serial handle with specified parameters
        port=serial_port,                   # Serial port name
        baudrate=9600,                      # Baud rate
        bytesize=serial.EIGHTBITS,          # Data bits
        parity=serial.PARITY_NONE,          # No parity
        stopbits=serial.STOPBITS_TWO,       # Two stop bits
        timeout=100                         # Timeout for read operations in seconds
    )
except serial.serialutil.SerialException:
    raise Exception("Please Ensure Connected Serial Peripheral")        #Informing the user to connect the serial peripheral app.

#################Sending the PreDataset Signals############################
startcomm(ser)                              #Executing the startcomm protocol

#################Sending Sequential Data#################
#Basic required declarations
datanumber = 0                          # Val to track the sample no of the dataset to be sent from lap to mc
datalength = len(downsampleddata)            # Val to store the no of samples
#Config the sendingfreq
sendingfreq = Samplerate + 50           # Making the sending freq a higher freq than sampling to ensure non-empty tx
sendinginterval = 1 / sendingfreq       # Var to hold the sending interval time

#sending routine
next_time = time.time()                 # Var to hold the time at which to send each sample for a specific sending freq
try:
    while True:
        send1data(downsampleddata, ser)      # Sends one sample from dataset
        if ser.in_waiting != 0:         # Check if there is data waiting in the buffer and presenting to lap for future diag
            print(ser.read(1))

        # Calculate the time to sleep until the next send
        next_time += sendinginterval
        sleep_time = next_time - time.time()

        if sleep_time > 0:
            time.sleep(sleep_time)
        else:
            # If we're behind schedule, skip sleeping to catch up
            next_time = time.time()


        datanumber = datanumber + 1                 # Incrementing the sample no to be sent
        if datanumber >= datalength:                # An exit routine if all data has been sent
            print("All values have been printed")
            exit()
            ser.close()
# Awaiting a key interrupt for ending the program
except KeyboardInterrupt:
    print("Exiting...")
    print(datanumber)
    ser.close()
    exit()