#!/usr/bin/env python3

import serial
from sys import argv
import argparse as ap
from datetime import datetime
from time import sleep
import matplotlib.pyplot as plt
import multiprocessing as mp
from collections import namedtuple

transfer_data = namedtuple('transfer_data', 'timestamp cell active_area V I')

attempt_threshold = 20

expected_receive_on_connect = b'SOLAR_SOAKER_FW\n'
send_on_connect = b'SS_EXTERNAL\x00'

start_str = b'START\n'
end_str =   'END'
# port = 'COM3'

parser = ap.ArgumentParser(description="Solar Soaker v0.1")
parser.add_argument('port', type=str, help="USB port to connect ('COM3', '/dev/ttyUSB3', etc)")


def get_args():
    if len(argv[1:]) == 0:
        parser.print_help()
        parser.exit()
    args = parser.parse_args()
    return args


def get_timestamp():
    return datetime.now().strftime("%m/%d/%Y - %H:%M:%S")


def check_device_is_SS(ser):
    ser.readline()

    is_SS = ser.readline() == expected_receive_on_connect
    if is_SS:
        ser.write(send_on_connect)
    return is_SS


def serial_connect(port):
    ser = serial.Serial()
    ser.port = port

    try:
        ser.open()
    except:
        print( f"Waiting to connect on port '{port}'" )
        for count in range(1, attempt_threshold + 1):
            try: ser.open()
            except: next

            if not ser.is_open:
                print( f"  Attempt {count}/{attempt_threshold}", end='\r')
                sleep(5)
            else:
                break

    if ser.is_open:
        if check_device_is_SS(ser):
            print( f"Connected on port '{port}'      {get_timestamp()}" )
        else:
            exit( f"Connected on port '{port}' but device is not Solar Soaker, exiting" )
    else:
        exit( "Connection Timeout" )
    return ser


def plot_received_data(data):
    plt.figure()
    plt.title(f"Cell {data.cell} {'-':^8} Active Area {data.active_area} {'-':^8} {data.timestamp}")
    plt.ylabel("Current Output (A)")
    plt.xlabel("Applied Voltage (V)")
    plt.plot(data.V, data.I)
    plt.show()


def data_transfer(ser):
    timestamp   = get_timestamp()
    print(f"Data Transfer Started         {timestamp}")

    cell        = ser.readline().decode('utf-8').strip('\n')
    active_area = ser.readline().decode('utf-8').strip('\n')

    ser.readline()
    line = ser.readline().decode('utf-8').strip('\n')
    V_data, I_data = [],[]

    while line != end_str:
        data_unpack = line.split()
        V_data.append(float(data_unpack[0]))
        I_data.append(float(data_unpack[1]))

        line = ser.readline().decode('utf-8').strip('\n')

    data = transfer_data(timestamp, cell, active_area, V_data, I_data)
    print("Data Transfer Complete")
    plot_received_data(data)


def main():
    args = get_args()

    print("\nSolar Soaker ☀  v0.1\n")
    ser = serial_connect(args.port)
    
    while(ser.is_open):
        line = ser.readline()
        if line == start_str:
            data_transfer(ser)


main()

exit()