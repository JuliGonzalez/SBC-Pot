import spidev
from time import sleep

spi = spidev.SpiDev()
spi.open(1, 0)

detectar_agua = 0
humedad_agua_int = 1
humedad_agua_ext = 2
# nivel_agua = 1

def read(channel):
    rawData = spi.xfer([1, (8 + channel) << 4, 0])
    processData = ((rawData[1] & 3) << 8) + rawData[2]

    return processData

while True:
    water_detector_value = read(detectar_agua)
    #print(water_detector_value)
    sleep(0.5)

    humidity_detector_value = read(humedad_agua_int)
    print("valor humedad suelo INT:" + str(humidity_detector_value))
    sleep(0.5)

    humidity_detector_value = read(humedad_agua_ext)
    print("valor humedad suelo EXT: " + str(humidity_detector_value))
    sleep(0.5)

