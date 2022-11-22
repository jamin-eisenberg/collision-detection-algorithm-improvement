runproject: main.o Screen.o DE1SoCfpga.o Keypad.o Line.o Polygon.o Vector2D.o Speaker.o CollisionPair.o LoIntervals.o
	g++ -g main.o Screen.o DE1SoCfpga.o Keypad.o Line.o Polygon.o Vector2D.o Speaker.o CollisionPair.o LoIntervals.o -o runproject

main.o: main.cpp DE1SoCfpga.h Screen.h Keypad.h Polygon.h
	g++ -g -Wall -c main.cpp

Screen.o: Screen.cpp DE1SoCfpga.h Screen.h
	g++ -g -Wall -c Screen.cpp

DE1SoCfpga.o: DE1SoCfpga.cpp DE1SoCfpga.h 
	g++ -g -Wall -c DE1SoCfpga.cpp

Keypad.o: Keypad.cpp Keypad.h
	g++ -g -Wall -c Keypad.cpp

Line.o: Line.cpp Line.h
	g++ -g -Wall -c Line.cpp

Polygon.o: Polygon.cpp Polygon.h
	g++ -g -Wall -c Polygon.cpp

Vector2D.o: Vector2D.cpp Vector2D.h
	g++ -g -Wall -c Vector2D.cpp

Speaker.o: Speaker.cpp Speaker.h DE1SoCfpga.h
	g++ -g -Wall -c Speaker.cpp

CollisionPair.o: CollisionPair.cpp CollisionPair.h
	g++ -g -Wall -c CollisionPair.cpp

LoIntervals.o: LoIntervals.cpp LoIntervals.h
	g++ -g -Wall -c LoIntervals.cpp

clean:
	rm *.o
	rm runproject
