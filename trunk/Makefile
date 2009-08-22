.PHONY: all
SRC=src
OBJ=obj
all:
	cd $(SRC); make

.PHONY: clean
clean:
	rm -f $(OBJ)/*.o Mosaic.exe


.PHONY: run
run: Mosaic.exe
	./Mosaic.exe

.PHONY: gdb
gdb: Mosaic.exe
	gdb ./Mosaic.exe

.PHONY: profile
profile: Mosaic.exe
	gprof ./Mosaic.exe --flat-profile > profile.txt

