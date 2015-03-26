CC = g++

INSTDIR = .
INCLUDE = .//lib//
CFLAGS = -g -Wall -ansi
LIBPATH = $(INSTDIR)//lib//

SOURCE = main.cpp $(LIBPATH)tinystr.cpp $(LIBPATH)tinyxml.cpp $(LIBPATH)tinyxmlerror.cpp $(LIBPATH)tinyxmlparser.cpp
OBJECTS = $(SOURCE:.cpp=.o)
HEADERS = $(LIBPATH)tinystr.h $(LIBPATH)tinyxml.h
EXECUTABLE=HandleXcodeString

all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
$(OBJECTSS): %.o : %.cpp $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) $< -c $@
clean:
	rm *.o $(LIBPATH)*.o