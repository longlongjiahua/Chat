######################################################
##                                                  ##
## Makefile Chat																		##
##                                                  ##
######################################################

##==========##
## Settings ##
##==========##

SHELL = /bin/sh
CXX = gcc
RM = rm -f

.SUFFIXES: .o .c

client = client
server = server
clientdir = ./Client
serverdir = ./Server
libdir = ./Libs
cxxflags ?= -W -Wall -Wextra -std=c99 -g #-ansi
ldflags ?=
libs ?= #-lcurl -lpcrecpp -lyaml-cpp -largtable2 -lsqlite3 -lboost_filesystem -lboost_system
includes ?= -I${libdir}
clientsource := ${shell find ${clientdir} -name *.c}
serversource := ${shell find ${serverdir} -name *.c}
libsource := ${shell find ${libdir} -name *.c}
clientobjects := ${clientsource:.c=.o}
serverobjects := ${serversource:.c=.o}
libobjects := ${libsource:.c=.o}
clientdependencies := ${clientsource:.c=.d}
serverdependencies := ${serversource:.c=.d}
libdependencies := ${libsource:.c=.d}

##===============##
## Build Targets ##
##===============##

.PHONY: all
all: ${client} ${server}
	@echo ===================================
	@echo Project has been built successfully
	@echo ===================================

#.PHONY: install
#install: ${main}

.PHONY: clean
clean:
	-@${RM} ${client} ${server} ${clientobjects} ${clientdependencies} ${serverobjects} ${serverdependencies} ${libobjects} ${libdependencies}

.PHONY: linecount
linecount:
	-@wc -l ${shell find ${clientdir} ${serverdir} ${libdir} -regex ".*\.\(h\|c\)$$"}

${client}: ${clientobjects} ${libobjects}
	${CXX} ${ldflags} ${includes} ${clientobjects} ${libobjects} -o ${client} ${libs}

${server}: ${serverobjects} ${libobjects}
	${CXX} ${ldflags} ${includes} ${serverobjects} ${libobjects} -o ${server} ${libs}

${clientdir}/%.o: ${clientdir}/%.c
	${CXX} -c ${cxxflags} ${includes} -MMD -o $@ $<

${serverdir}/%.o: ${serverdir}/%.c
	${CXX} -c ${cxxflags} ${includes} -MMD -o $@ $<

${libdir}/%.o: ${libdir}/%.c
	${CXX} -c ${cxxflags} ${includes} -MMD -o $@ $<

ifneq ($(MAKECMDGOALS),clean)
-include ${clientdependencies}
-include ${serverdependencies}
endif
