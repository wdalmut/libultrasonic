CC=/usr/bin/gcc
AR=/usr/bin/ar
CP=/bin/cp
MKDIR=/bin/mkdir
RM=/bin/rm

CFLAGS=-g3 -Wall -lbcm2835

BUILD_DIR=../build


all: init ultrasonic.o
	${AR} -rsc ${BUILD_DIR}/libultrasonic.a ${BUILD_DIR}/ultrasonic.o

ultrasonic.o:
ifeq ($(board),hc_sr04)
	${CC} ${CFLAGS} -c $(board)/$(board).c -I $(board)\
		-D${board}_ranging=ultrasonic_ranging -D${board}_init=ultrasonic_init \
		-o ${BUILD_DIR}/ultrasonic.o -I${board}
else
	@echo "*********************************************************************"
	@echo "* WARNING YOU HAVE TO SELECT AT LEAST ONE BOARD (board=hc_sr04)   *"
	@echo "*********************************************************************"
endif

init:
	${MKDIR} -p ${BUILD_DIR}

clean:
	${RM} ${BUILD_DIR}/*

