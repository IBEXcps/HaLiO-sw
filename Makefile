SRC_DIR = ${PWD}/smartmeter
BUILD_DIR = ${SRC_DIR}/build
MAIN_FILE = smartmeter.ino
TOOLS = ${PWD}/tools

PACKAGE = esp8266
ARCH = esp8266
BOARD = nodemcuv2
PARAMETER = CpuFrequency=160

ARG = --board ${PACKAGE}:${ARCH}:${BOARD}:${PARAMETER} --verify --verbose --pref build.path=${BUILD_DIR}

ifndef IP
	IP=192.168.0.106
endif
ifndef PORT
	PORT=5222
endif

deps:
	arduino --install-library "ArduinoThread,ESP8266 Oled Driver for SSD1306 display,Brzo I2C"

all:
	arduino ${ARG} ${SRC_DIR}/${MAIN_FILE}

clean:
	rm -rf ${BUILD_DIR}

upload: all
	${TOOLS}/espota.py --debug --progress -i ${IP} -p ${PORT} -f ${BUILD_DIR}/${MAIN_FILE}.bin

run: all upload
