import lcm
import time
from serial import Serial
from xbee import ZigBee

#Serial to LCM Mapping
TxLCMMap = ["05EBOLA","imu","gps"]  #LCM Messages to receive from plane
RxLCMMap = ["05EBOLA","OVERRIDE","pod_data"] 	#LCM Messages to Transmit to plane
#Setup Handler for LCM
lc = lcm.LCM()
channelDB = dict()
#On Cygwin, uncomment
#PORT = '/dev/ttyS2'
#On Windows, uncomment
#Port = 'COM3'
#On Unix/Linux, uncomment
PORT = '/dev/ttyUSB1'
BAUD = 9600
ser = Serial(PORT, BAUD)

def serialHandler(serialData):
    print serialData
    if 'rf_data' in serialData:
        print serialData['rf_data']
        serialData = str(serialData['rf_data'])
        #Handler converting Serial to LCM
        print serialData
        #(channelNum, data) = serialData.split('.')
        channelNum = serialData[0]
        data = serialData[2:]
        channelNum = int(channelNum)
        channelNameLCM = TxLCMMap[channelNum]
        lc.publish(channelNameLCM,data)

xbee = ZigBee(ser, callback=serialHandler)

def lcmHandler(channelNameLCM, data):
    #Handler converting LCM to Serial
    channelNum = RxLCMMap.index(channelNameLCM)
    #xbee.tx(dest_addr='\x00\x01', data='%s.%s' % (str(channelNum), data))
    xbee.tx(dest_addr_long='\x00\x13\xA2\x00\x40\xD6\x63\xBB',dest_addr='\xFF\xFE',data='%s.%s' % (str(channelNum), data) )
    print "LCM!"

subscriptions = [lc.subscribe(name,lcmHandler) for name in RxLCMMap]

print 'setup complete'

try:
    while True:
        #handle LCM
        lc.handle()
except:
    print "Module Exits from Keyboard Interrupt"
finally:
    ser.close()
    [lc.unsubscribe(x) for x in subscriptions]

print 'module end successfully'
