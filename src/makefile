token:
	gcc -c clock.c -o obs/clock.o
	gcc -c window.c -o obs/window.o
	gcc -c input.c -o obs/input.o
	gcc -c matrix.c -o obs/matrix.o
	gcc -c drawline.c -o obs/drawline.o
	gcc -c projection.c -o obs/projection.o
	gcc -c shader.c -o obs/shader.o
	gcc -c model.c -o obs/model.o
	gcc -c libmain.c -o obs/libmain.o
	ar rcs libgenstat.a obs/clock.o obs/window.o obs/input.o obs/matrix.o obs/drawline.o obs/projection.o obs/shader.o obs/model.o obs/libmain.o
