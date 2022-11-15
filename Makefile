COMPILEFLAGS	 = -I"C:\SFML-2.5.1\include" -DSFML_STATIC
LINKFLAGS	 = -o main -L"C:\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main

all: preclean compile link clean

compile:
	g++ $(COMPILEFLAGS) -c main.cpp 
	g++ $(COMPILEFLAGS) -c settings.cpp
	g++ $(COMPILEFLAGS) -c Game.cpp 
	g++ $(COMPILEFLAGS) -c AssetManager.cpp 

link:
	g++ main.o settings.o Game.o AssetManager.o $(LINKFLAGS)

clean:
	del *.o

preclean:
	del *.exe