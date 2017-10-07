PROJ=swde
QMAKE=qmake
HOST=$(shell hostname)

ifeq ("$(wildcard /usr/bin/qmake)","")
    QMAKE=qmake-qt5
endif

ifeq ("$(HOST)","merlin.fit.vutbr.cz")
    ifeq ("$(wildcard /usr/bin/qmake-eq5)","")
        QMAKE=/usr/local/share/Qt-5.5.1/5.5/gcc_64/bin/qmake
        export LD_LIBRARY_PATH:=/usr/local/share/Qt-5.5.1/5.5/gcc_64/lib:${LD_LIBRARY_PATH}
        export QT_PLUGIN_PATH:=/usr/local/share/Qt-5.5.1/5.5/gcc_64/plugins:${QT_PLUGIN_PATH}
        endif
endif

PROJ_PATH=$(shell pwd)/swde
SRC_PATH=$(PROJ_PATH)/src
SRC_FILES= $(wildcard $(SRC_PATH)/*.cpp)
HDR_FILES= $(wildcard $(SRC_PATH)/*.h)

.PHONY: clean

all: $(PROJ)

$(PROJ): $(SRC_FILES) $(HDR_FILES)
	-@cd $(PROJ_PATH) && $(QMAKE) && make
	@mv $(PROJ_PATH)/$(PROJ) .

clean:
	-@cd $(PROJ_PATH) && make clean
	rm -f $(PROJ_PATH)/Makefile
	rm -f $(PROJ_PATH)/$(PROJ)
	rm -f $(PROJ)
