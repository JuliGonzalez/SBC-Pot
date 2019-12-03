import smbus2 as smbus
import time

bus = smbus.SMBus(4)

addr = 0x44
msb = 0x24
lsb =0x16

bus.write_i2c_block_data(addr, 0x2c, [0x06])

time.sleep(0.5)

data = bus.read_i2c_block_data(addr, 0x00, 6)

print(data)
temp_raw = (data[0]*256) + data[1]
temp = -45 + (175 * (temp_raw) / (2**16 - 1))
print("temperatura: " + str(temp))

hum_raw = (data[3]*256) + data[4]
hum = 100 * (hum_raw / (2**16 - 1))
humi = (hum_raw / (2**16 -1))
print(humi)
print("humedad: " + str(hum))
print(hum)
