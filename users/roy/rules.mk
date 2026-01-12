UNICODE_ENABLE = yes

NAGINATA_PATH = $(USER_PATH)/../naginata_v17
VPATH += $(NAGINATA_PATH)
SRC += $(NAGINATA_PATH)/naginata_v17.c
SRC += $(NAGINATA_PATH)/twpair_on_jis.c
SRC += $(NAGINATA_PATH)/nglist.c
SRC += $(NAGINATA_PATH)/nglistarray.c

