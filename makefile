# Compiler Settings
CXX = g++
CXXFLAGS = -Wall -g

# Path variables
LIB = ./include/
SRC = ./src/

# Target
TARGET = main

# Compilation rules
main: Main.o Snowman.o Scene.o Sphere.o KDTree.o
	$(CXX) $(CXXFLAGS) -o main Main.o Snowman.o Scene.o Sphere.o KDTree.o

Main.o: $(SRC)Main.cpp $(LIB)Scene.h $(LIB)Math.h $(LIB)Sphere.h $(LIB)Ray.h $(LIB)Snowman.h
	$(CXX) $(CXXFLAGS) -c $(SRC)Main.cpp

Snowman.o: $(SRC)Snowman.cpp $(LIB)Snowman.h $(LIB)Sphere.h
	$(CXX) $(CXXFLAGS) -c $(SRC)Snowman.cpp

Scene.o: $(SRC)Scene.cpp $(LIB)Scene.h $(LIB)Math.h $(LIB)Sphere.h $(LIB)Ray.h $(LIB)KDTree.h
	$(CXX) $(CXXFLAGS) -c $(SRC)Scene.cpp

Sphere.o: $(SRC)Sphere.cpp $(LIB)Sphere.h
	$(CXX) $(CXXFLAGS) -c $(SRC)Sphere.cpp

KDTree.o: $(SRC)KDTree.cpp $(LIB)KDTree.h $(LIB)Photon.h $(LIB)Math.h
	$(CXX) $(CXXFLAGS) -c $(SRC)KDTree.cpp

# Clean
clean: 
	$(RM) $(TARGET) *.o *~