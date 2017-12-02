# Makefile for compatibility with both Serial and OTA uploads

SRC_DIR = ${PWD}/smartmeter
BUILD_DIR = ${PWD}/build
MAIN_FILE = smartmeter.ino
TOOLS = ${PWD}/tools

PACKAGE = esp8266
ARCH = esp8266
BOARD = nodemcuv2
PARAMETER = CpuFrequency=160,FlashSize=4M3M,UploadSpeed=921600

ARG = --board ${PACKAGE}:${ARCH}:${BOARD}:${PARAMETER} --verbose-build --pref build.path=${BUILD_DIR}

# Define default serial port
SERIAL ?= /dev/ttyUSB0

# Define default OTA  parameters
IP ?= 192.168.0.106
PORT ?= 5222

all:
	arduino --verify ${ARG} ${SRC_DIR}/${MAIN_FILE}

deps:
	arduino --pref boardsmanager.additional.urls="http://arduino.esp8266.com/stable/package_esp8266com_index.json" --save-prefs
	arduino --install-boards esp8266:esp8266
	arduino --install-library "ArduinoThread,ArduinoJson,ESP8266 Oled Driver for SSD1306 display,Brzo I2C"

flash:
	arduino --upload ${ARG} ${SRC_DIR}/${MAIN_FILE} --port ${SERIAL}

clean:
	rm -rf ${BUILD_DIR}

upload: all
	${TOOLS}/espota.py --debug --progress -i ${IP} -p ${PORT} -f ${BUILD_DIR}/*.bin

run: all upload


# Make examples:
# make flash SERIAL=/dev/ttyUSBx
# make upload IP=192.168.0.103
