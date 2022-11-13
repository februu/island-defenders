COMPILEFLAGS	 = -I"C:\SFML-2.5.1\include" -DSFML_STATIC
LINKFLAGS	 = -o main -L"C:\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main

all: preclean compile link clean

compile:
	g++ $(COMPILEFLAGS) -c main.cpp 
	g++ $(COMPILEFLAGS) -c settings.cpp 

link:
	g++ main.o settings.o $(LINKFLAGS)

clean:
	del *.o

preclean:
	del *.exe