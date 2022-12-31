COMPILEFLAGS	 = -I"libs\include" -DSFML_STATIC
LINKFLAGS	 = -o main -L"libs\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main -mwindows

all: preclean compile link clean

compile:
	g++ $(COMPILEFLAGS) -c main.cpp 
	g++ $(COMPILEFLAGS) -c Settings.cpp
	g++ $(COMPILEFLAGS) -c Game.cpp 
	g++ $(COMPILEFLAGS) -c AssetManager.cpp
	g++ $(COMPILEFLAGS) -c World.cpp 
	g++ $(COMPILEFLAGS) -c Hud.cpp
	g++ $(COMPILEFLAGS) -c Entity.cpp
	g++ $(COMPILEFLAGS) -c Particle.cpp
	g++ $(COMPILEFLAGS) -c Projectile.cpp

link:
	g++ main.o Settings.o Game.o Hud.o AssetManager.o Entity.o World.o Particle.o Projectile.o res/icon.res res/info.res $(LINKFLAGS)

clean:
	del *.o

preclean:
	del *.exe