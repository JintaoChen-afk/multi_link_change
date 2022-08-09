'''
access program which could filter data by source node
Last modified: 2022/08/09
'''

#!/usr/bin/env python
import sys
import socket
import os
import time
import serial
import math
import numpy
import re
from datetime import datetime
from threading import Thread
from SocketServer import ThreadingMixIn

round_counter = 1
theta_ref = 0
thetaini = 0
uini = 0
delta_t = 0.3;
yh_last = 0
yh = 1
kp = 75
ki = 1 / 1000
kd = 0
theta_last = thetaini
theta_last_last = thetaini
uout_last = uini
ki_sum = 0
compu_latency = []
tmp_net_status = 0
ser = serial.Serial("/dev/ttyUSB1", 115200)
ser2 = serial.Serial("/dev/ttyUSB0", 115200)

while True:
    try:
        if not ser.isOpen():
            ser.open()
        data = ser.readline()
        data = data.strip().decode()
        print data
        data_split = [float(x.strip()) for x in re.compile('-*\w+.\w+').findall(data.strip())]
        #data_split1 = [x.strip() for x in data.strip().split(" ")]
        #print data_split1
        #data_split = []
        #for x in data_split1:
        #    if x[-1] == ',':
        #        data_split.append(float(x[:-1]))
        #    else:
        #       data_split.append(float(x))
        data = data.strip()
        if (data[-1] == '1' or data[-1] == '4') and data[-2] == ' ':
            data_split.append(float(data[-1]))
        if data_split:
            # ser.close()
            t = time.time()
            print data_split
            #print round_counter
            #if round_counter > 50:
            #    net_status = 1
            timeT = datetime.now().strftime('%Y,%m,%d,%H,%M,%S.%f')
            # print "data=",data

            #print data_split
            if data_split[0] != 10000 and len(data_split) == 7:
                if math.fabs(data_split[6] - 4) < 0.01:
                    continue
                theta_ref = data_split[0]
                theta = data_split[1]
                theta_error = data_split[2]

                yh_last = yh
                yh = data_split[3]
                sensor_seq = data_split[4]
                # print "theta_ref=",theta_ref,", theta=",theta, "theta_error=", theta_error
                # print "theta_last=",theta_last
                # print "theta_last_last=",theta_last_last
                # print "sensor_seq", sensor_seq
                if yh_last != yh or data_split[5] == 0.0:
                    round_counter = 1
                    yh_last = yh

                # print "out last:",uout_last
                # print "kp*(theta_error - theta_last):",kp*(theta_error - theta_last)
                # print "ki*delta_t*theta_error:", ki*delta_t*theta_error
                # print "kd*(theta_error-2 * theta_last + theta_last_last)/delta_t:", kd*(theta_error-2 * theta_last + theta_last_last)/delta_t
                if round_counter == 1:
                    theta_last_last = theta_error
                    theta_last = theta_error
                    uout_last = uini
                    ki_sum = 0
                    # uout = uini
                    uout = uout_last + kp * theta_error + ki * delta_t * theta_error
                    # uout =  kp*theta_error
                # ki_sum += ki*delta_t*theta_error
                # uout += ki_sum
                # uout += kd * (theta_error/delta_t)

                else:
                    uout = uout_last + kp * (theta_error - theta_last) + ki * delta_t * theta_error + kd * (
                                theta_error - 2 * theta_last + theta_last_last) / delta_t
                # uout =  kp*theta_error
                # ki_sum += ki*delta_t*theta_error
                # uout += ki_sum
                # uout += kd * (theta_error-theta_last)/delta_t

                uout_last = uout
                theta_last_last = theta_last
                theta_last = theta_error

                print "Uout generated by python code on server:", uout
                # connection.send('% 3.4f,% 3.4f' % (uout, sensor_seq))
                # ser.open()
                # leb = ser.write('%3.4f %3.4f\n' % (uout, sensor_seq))
                print '%37.6f, %37.6f\n' % (uout, sensor_seq)
                # ser.close()

                if not ser2.isOpen():
                    ser2.open()
                ser2.write(('%37.6f, %37.6f' % (uout, sensor_seq)).encode())
                ser2.close()

                round_counter = round_counter + 1
                elapsed = time.time() - t
                print "Computational latency:", elapsed
                compu_latency.extend([elapsed])
                # if sensor_seq>1198:
                # print "Computational latency list", compu_latency
                # tmp_net_status = net_status
        else:
            # ser.write('0 0\n')
            pass
    except:
        pass
