DISTRIBUTION = $(shell lsb_release -s -i)
CODENAME = $(shell lsb_release -s -c)
PLATFORM = $(shell uname -m)

# Basic device servers compiled for all architectures
SUBDIRS := AnalogProxy CommProxy USSProxy RawProxy Starter

ifeq ($(DISTRIBUTION), Ubuntu)
	ifeq ($(PLATFORM), x86_64)
		SUBDIRS += ApogeeCamera MAGSrv MercurySrv TM700Srv VacuumValveSrv WaterValveSrv


	endif
	ifeq ($(PLATFORM), armv7l)
		SUBDIRS += MCP23017Srv
	endif
endif	

ifeq ($(DISTRIBUTION), Raspbian)
	#SUBDIRS += 
endif	

.PHONY : all $(SUBDIRS)
all : $(SUBDIRS)
$(SUBDIRS) :
	$(MAKE) -C $@ clean all
