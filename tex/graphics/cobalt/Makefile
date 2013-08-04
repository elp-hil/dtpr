OPT = -g
KASPER_PATH = /kalinka/home/rovedo/Kasper/install
HOME_PATH = /kalinka/home/rovedo/Analysis/yttriumRate
MUON_ANALYSIS = /kalinka/home/rovedo/Analysis/muonAnalysis/install
SOURCE = src
BUILD = build

CXXFLAGS = -Wall\
						-Wextra\
						`root-config --cflags`\
						-O0\
						-I$(KASPER_PATH)/include\
						-I$(MUON_ANALYSIS)/include\
						$(OPT) 
LDFLAGS = 	`root-config --glibs`\
						-L$(KASPER_PATH)/lib\
						-L$(MUON_ANALYSIS)/lib\
						-lkali\
						-lkasper-common\
						-lmuonanalysis
						
OBJECT_FILES = 
#$(wildcard *.o)
#$(patsubst src/%.cpp,build/%.o,$(CPP_FILES)) 

.PHONY: all

all: yttrium


yttrium: yttrium.cxx
	g++ $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(BUILD)/*.o rate

