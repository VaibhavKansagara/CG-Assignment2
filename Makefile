main : main.o Point.o Model.o Parser.o Controller.o View.o
	g++ -o main main.o Point.o Model.o Parser.o Controller.o View.o -lglfw -lGLEW -lGLU -lGL
	./main

main.o : main.cpp Model.h View.h Parser.h Point.h

Point.o : Point.h

Model.o : Model.h Point.h

Parser.o : Parser.h Model.h

View.o : Model.h View.h

Controller.o : Controller.h

clean :
	rm *.o main
