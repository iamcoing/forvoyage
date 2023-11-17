CC  ?= gcc
CXX ?= g++

obj ?= obj.o

GLADPATH += /home/ubuntu/OpenGL/glfw-3.3.8/deps
# GLFLAGS += ${GLADPATH}/glad_gl.c -lGL -lglfw3 -lrt -lm -ldl -lpthread
GLFLAGS += glad.c path/shader/shader.cpp path/camera/camera.cpp -lGL -lglfw3 -lrt -lm -ldl -lpthread

CXXFLAGS += -c -Wall -pedantic-parameter $(shell pkg-config --clflags opencv)
CVFLAGS += $(shell pkg-config --libs --static opencv)

all:target

target: $(obj);
	$(CXX) $< -o $@ $(GLFLAGS) -I $(GLADPATH) $(CVFLAGS)

$(objects): %.o: %.cpp;
	$(CXX) $< -o $@ $(GLFLAGS) -I $(GLADPATH) $(CVFLAGS)

.PHONY:clean cleanall cleanobj cleandiff

cleanobj:
	rm -f *.o

clean:
	rm -f target

cleanall:
	rm -f target *.o *.diff

cleandiff:
	rm -f *.diff

