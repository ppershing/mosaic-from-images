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
	-(DATE=`date '+%F,%T'`; \
	  mv Mosaic.log "../$$DATE.mosaic.log"; \
	  mv statistics.txt "../$$DATE.stat.txt"; \
	 )
	./Mosaic.exe

.PHONY: gdb
gdb: Mosaic.exe
	gdb ./Mosaic.exe

.PHONY: profile
profile: Mosaic.exe
	gprof ./Mosaic.exe --flat-profile > profile.txt

.PHONY: stats
stats: 
	cat statistics.txt | sed s'/[^,]*,[^,]*,\([^,]*\).*/\1/' | \
	sort -n > stat.dat

