COMPILEFLAGS	 = -I"C:\SFML-2.5.1\include" -DSFML_STATIC
LINKFLAGS	 = -o main -L"C:\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main

all: preclean compile link clean

compile:
	g++ $(COMPILEFLAGS) -c main.cpp 
	g++ $(COMPILEFLAGS) -c settings.cpp
	g++ $(COMPILEFLAGS) -c Game.cpp 
	g++ $(COMPILEFLAGS) -c AssetManager.cpp
	g++ $(COMPILEFLAGS) -c World.cpp 
	g++ $(COMPILEFLAGS) -c Hud.cpp
	g++ $(COMPILEFLAGS) -c Entity.cpp
	g++ $(COMPILEFLAGS) -c Particle.cpp
	g++ $(COMPILEFLAGS) -c Projectile.cpp

link:
	g++ main.o settings.o Game.o Hud.o AssetManager.o Entity.o World.o Particle.o Projectile.o $(LINKFLAGS)

clean:
	del *.o

preclean:
	del *.exe