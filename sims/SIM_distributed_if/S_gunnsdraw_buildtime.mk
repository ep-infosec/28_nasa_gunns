# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
# List of all GunnsDraw network drawings that are to be exported at build time:
GUNNSDRAWINGS := \
 $(GUNNS_HOME)/sims/networks/fluid/test/distributedIf/DistributedIfFluid.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/distributedIf/DistributedIfFluidLiquid.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/distributedIf/DistributedIfFluidAccum.xml

####################################################################################################
# Don't touch the rest of this...
# Make a list of network class headers from the drawing files.  Note this requires that the exported
# code has the exact same name as the drawing!
GUNNSDRAW_HEADERS := $(subst .xml,.hh,$(GUNNSDRAWINGS))

# Trick processing of the S_define depends on these headers
S_define: $(GUNNSDRAW_HEADERS)

# Headers depend on the .xml, and are produced by invoking the export script
# TODO add the -g option once I have that working.  This will skip the drawing maintenance and error
# checks, should be a little faster...
$(GUNNSDRAW_HEADERS) : %.hh : %.xml
	@ echo $(shell python $(GUNNS_HOME)/draw/netexport.py $<)
