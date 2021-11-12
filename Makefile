DISTRIBUTION = $(shell lsb_release -s -i)
CODENAME = $(shell lsb_release -s -c)
PLATFORM = $(shell uname -m)

# Basic device servers compiled for all architectures
SUBDIRS := AnalogProxy CommProxy USSProxy RawProxy Starter Modbus XIMCSrv

ifeq ($(DISTRIBUTION), Ubuntu)
	ifeq ($(PLATFORM), x86_64)
		SUBDIRS += MAGSrv MercurySrv TM700Srv VacuumValveSrv WaterValveSrv Polyscience Standa8SMC1 Picomotor DryVacSrv TPG256Srv


	endif
	ifeq ($(PLATFORM), armv7l)
		SUBDIRS += MCP23017Srv GpioSwitch
	endif
endif

ifeq ($(DISTRIBUTION), Raspbian)
	SUBDIRS += GpioSwitch SHTSrv
endif

# Device requiring special libraries
#AvantesSpectrometer
#OOSpectrometer
#ApogeeCamera


.PHONY : all $(SUBDIRS)
all : $(SUBDIRS)
$(SUBDIRS) :
	$(MAKE) -C $@ clean all

