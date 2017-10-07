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

SRC_PATH=$(shell pwd)/src
SRC_FILES= $(wildcard $(SRC_PATH)/*.cpp)
HDR_FILES= $(wildcard $(SRC_PATH)/*.h)

.PHONY: clean all cli doxygen pack run run-gui

all: $(PROJ)

$(PROJ): $(SRC_FILES) $(HDR_FILES)
	-@cd $(SRC_PATH) && $(QMAKE) && make
	@mv $(SRC_PATH)/$(PROJ) .

clean:
	-@cd $(SRC_PATH) && make clean
	rm -f $(SRC_PATH)/Makefile
	rm -f $(SRC_PATH)/$(PROJ)
	rm -f $(PROJ)
