# Compiler Settings
CXX = g++
# CXXFLAGS = -fopenmp -Wall -g
CXXFLAGS = -openmp -Wall -g

# Path variables
LIB = ./include/
SRC = ./src/
OBJ = ./obj/

# Target
TARGET = main

# Compilation rules
main: $(OBJ)Main.o $(OBJ)Snowman.o $(OBJ)Scene.o $(OBJ)Sphere.o $(OBJ)KDTree.o 
	$(CXX) $(CXXFLAGS) $(OBJ)Main.o $(OBJ)Snowman.o $(OBJ)Scene.o $(OBJ)Sphere.o $(OBJ)KDTree.o -o Main

$(OBJ)Main.o: $(SRC)Main.cpp $(LIB)Scene.h $(LIB)Math.h $(LIB)Sphere.h $(LIB)Ray.h $(LIB)Snowman.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Snowman.o: $(SRC)Snowman.cpp $(LIB)Snowman.h $(LIB)Sphere.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Scene.o: $(SRC)Scene.cpp $(LIB)Scene.h $(LIB)Math.h $(LIB)Sphere.h $(LIB)Ray.h $(LIB)KDTree.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Sphere.o: $(SRC)Sphere.cpp $(LIB)Sphere.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)KDTree.o: $(SRC)KDTree.cpp $(LIB)KDTree.h $(LIB)Photon.h $(LIB)Math.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean: 
	$(RM) $(TARGET) -rf $(OBJ)/*