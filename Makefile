OBJS	= main.o megjelenit.o jatekmenet.o plants.o zombie.o bullets.o
SOURCE	= main.c megjelenit.c jatekmenet.c plants.c zombie.c bullets.c
HEADER	= megjelenit.h jatekmenet.h plants.h zombie.h bullets.h debugmalloc.h
OUT	= pvz.out
CC	 = gcc
FLAGS	 = -g3 -c
LFLAGS	 = -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -lcunit

megjelenit.o: megjelenit.c
	$(CC) $(FLAGS) megjelenit.c -lcunit

jatekmenet.o: jatekmenet.c
	$(CC) $(FLAGS) jatekmenet.c -lcunit

plants.o: plants.c
	$(CC) $(FLAGS) plants.c -lcunit

zombie.o: zombie.c
	$(CC) $(FLAGS) zombie.c -lcunit

bullets.o: bullets.c
	$(CC) $(FLAGS) bullets.c -lcunit


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)

debug: $(OUT)
	valgrind $(OUT)

valgrind: $(OUT)
	valgrind $(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full $(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes $(OUT)