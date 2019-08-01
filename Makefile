
SUBDIRS := AnalogProxy ApogeeCamera CommProxy MAGSrv MCP23017Srv MercurySrv RawProxy TM700Srv USSProxy VacuumValveSrv WaterValveSrv
.PHONY : all $(SUBDIRS)
all : $(SUBDIRS)
$(SUBDIRS) :
	$(MAKE) -C $@ clean all
