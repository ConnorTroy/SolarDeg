#!/usr/bin/env python3

import serial
from sys import argv
import argparse as ap
from datetime import datetime
import time
import matplotlib.pyplot as plt

attempt_threshold = 0
# port = 'COM3'

parser = ap.ArgumentParser(description="Solar Soaker v0.1")
parser.add_argument('port', type=str, help="USB port to connect ('COM3', '/dev/ttyUSB3', etc)")




# while(ser.is_open):
#     print(ser.readline().decode("utf-8"), end='')

def get_args():
    if len(argv[1:]) == 0:
        parser.print_help()
        parser.exit()
    args = parser.parse_args()
    return args


def get_timestamp():
    return datetime.now().strftime("%m/%d/%Y - %H:%M:%S")


def serial_connect(port):
    ser = serial.Serial()
    ser.port = port

    try:
        ser.open()
    except:
        print( f"Waiting to connect on port '{port}'" )

        for count in range(attempt_threshold):
            try:
                ser.open()
            except:
                print( f"\rAttempt {count}/{attempt_threshold} ", end='')
                time.sleep(5)

    if ser.is_open:
        print( f"Connected on port '{port}'  {get_timestamp()}" )
    else:
        exit( f"Could not connect on port '{port}'" )
    return ser


def main():
    args = get_args()

    print("\nSolar Soaker ☀  v0.1\n")
    ser = serial_connect(args.port)
    ser.close()


    data = open("sample_data.txt", 'r')

    time_stamp  = data.readline()
    cell        = data.readline().split()[1]
    active_area = data.readline().split()[1]

    while data.readline().find("Data - ") == -1:
        next

    I_data, V_data = [], []

    for line in data:
        data_unpacked = line.split()
        I_data.append(data_unpacked[0])
        V_data.append(data_unpacked[1])

    plt.figure()
    plt.title(f"Cell {cell} {'-':^6} Active Area {active_area} {'-':^6} {time_stamp}")
    plt.ylabel("Current Output (A)")
    plt.xlabel("Applied Voltage (V)")
    plt.plot(V_data, I_data)
    # plt.show()

main()