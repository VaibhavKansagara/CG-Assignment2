CXX = g++ -std=c++11
obj = main.o Point.o Color.o Model.o Parser.o Controller.o Shader.o View.o 
LDFLAGS = -w -lGL -lGLU -lglfw -lGLEW

main : $(obj)
	$(CXX) $(obj) $(LDFLAGS) -o bin/main
	./bin/main

main.o : src/main.cpp include/Model.h include/View.h include/Controller.h include/Parser.h
		$(CXX) -c src/main.cpp

Point.o : include/Point.h
		$(CXX) -c src/Point.cpp

Color.o : include/Color.h
		$(CXX) -c src/Color.cpp

Model.o : include/Model.h include/Point.h
		$(CXX) -c src/Model.cpp

Parser.o : include/Parser.h include/Model.h
		$(CXX) -c src/Parser.cpp

View.o : include/Model.h include/Shader.h include/View.h
		$(CXX) -c src/View.cpp

Controller.o : include/Controller.h include/View.h
		$(CXX) -c src/Controller.cpp 

Shader.o : include/Shader.h
		$(CXX) -c src/Shader.cpp

clean :
	rm *.o bin/main
