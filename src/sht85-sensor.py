#sensor for sht85

import smbus2 as smbus
import time

bus = smbus.SMBus(5)

addr = 0x44
msb = 0x24
lsb = 0x16

bus.write_byte(addr, 0x24)

bus.write_i2c_block_data(addr, 0x88)
time.sleep(0.0005)
a = bus.read_byte_data(0x40, 6)
print(a)

bus.write_i2c_block_data(addr, 0x2c, [0x06])
data = bus.read_i2c_block_data(addr, 0x00, 6)

print(data)


while True:
    # The frequency to read the sensor should be set greater than
    # the integration time (and the power saving delay if set).
    # Reading at a faster frequency will not cause an error, but
    # will result in reading the previous data
    time.sleep(.04)  # 40ms

    word = bus.read_word_data(addr, als)

    gain = 1.8432  # Gain for 1/8 gain & 25ms IT
    # Reference www.vishay.com/docs/84323/designingveml7700.pdf
    # 'Calculating the LUX Level'

    val = word * gain
    val = round(val, 1)  # Round value for presentation

    print
    "Lux: " + str(val) + " lx"

