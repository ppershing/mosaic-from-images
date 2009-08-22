INC=  -I../include -I/usr/include/SDL
CFLAGS= -W -Wall -pedantic -O2 ${INC} -pg -g 
CFLAGS= -W -Wall -pedantic -O3 ${INC} -g \
	-finline-functions -fomit-frame-pointers
CC=g++
LDIR=../lib
LIBS=-lSDLmain\
	-l ${LDIR}/libSDL\
	-l ${LDIR}/libSDL_image\
	-l SDL_gfx\
	-l ${LDIR}/libSDL

H=../include
OBJ=../obj


../mosaic.exe: Mosaic.cpp ${OBJ}/Errors.o \
	${OBJ}/Exceptions.o ${OBJ}/MyStringUtils.o \
	${OBJ}/Preferences.o \
	${OBJ}/CacheData.o ${OBJ}/Cache.o \
	${OBJ}/Project.o ${OBJ}/Bitmap.o \
	${OBJ}/Engine.o ${OBJ}/ThumbnailStack.o\
	${OBJ}/ThumbnailMatrix.o ${OBJ}/ColorDistance.o\
	${OBJ}/BitmapDistance.o ${OBJ}/Timer.o\
	${OBJ}/ThumbnailFitData.o \
	${OBJ}/ProgressStatistics.o \
	${OBJ}/ColorAdjust.o \
	${OBJ}/VariableOptimizer.o \
	${OBJ}/Version.o 
	g++  ${CFLAGS} $^ ${LIBS} -o ../Mosaic.exe

${OBJ}/Version.o: Version.cpp Mosaic.ver
	$(CC) $(CFLAGS) Version.cpp -o $@ -c
	cat Mosaic.ver | awk '{ if ( $$2 == "BUILD" ) print $$1 " " \
	$$2 " " $$3+1; else print $$0  };'> Mosaic.ver2
	mv -f Mosaic.ver2 Mosaic.ver

$(OBJ)/Errors.o: Errors.cpp $(H)/Errors.h $(H)/Exceptions.h $(H)/MyAssert.h
	$(CC) $(CFLAGS) Errors.cpp -o $@ -c

$(OBJ)/Exceptions.o: Exceptions.cpp $(H)/Exceptions.h
	$(CC) $(CFLAGS) Exceptions.cpp -o $@ -c

$(OBJ)/MyStringUtils.o: MyStringUtils.cpp $(H)/MyStringUtils.h
	$(CC) $(CFLAGS) MyStringUtils.cpp -o $@ -c

$(OBJ)/Preferences.o: Preferences.cpp $(H)/Preferences.h
	$(CC) $(CFLAGS) Preferences.cpp -o $@ -c

$(OBJ)/CacheData.o: CacheData.cpp $(H)/CacheData.h
	$(CC) $(CFLAGS) CacheData.cpp -o $@ -c

$(OBJ)/Cache.o: Cache.cpp $(H)/Cache.h
	$(CC) $(CFLAGS) Cache.cpp -o $@ -c

$(OBJ)/Project.o: Project.cpp $(H)/Project.h
	$(CC) $(CFLAGS) Project.cpp -o $@ -c

$(OBJ)/Bitmap.o: Bitmap.cpp $(H)/Bitmap.h
	$(CC) $(CFLAGS) Bitmap.cpp -o $@ -c

$(OBJ)/Engine.o: Engine.cpp $(H)/Engine.h
	$(CC) $(CFLAGS) Engine.cpp -o $@ -c

$(OBJ)/ThumbnailStack.o: ThumbnailStack.cpp $(H)/ThumbnailStack.h
	$(CC) $(CFLAGS) ThumbnailStack.cpp -o $@ -c

$(OBJ)/ThumbnailMatrix.o: ThumbnailMatrix.cpp $(H)/ThumbnailStack.h
	$(CC) $(CFLAGS) ThumbnailMatrix.cpp -o $@ -c

$(OBJ)/ThumbnailFitData.o: ThumbnailFitData.cpp $(H)/ThumbnailStack.h
	$(CC) $(CFLAGS) ThumbnailFitData.cpp -o $@ -c


$(OBJ)/ColorDistance.o: ColorDistance.cpp $(H)/ColorDistance.h
	$(CC) $(CFLAGS) ColorDistance.cpp -o $@ -c

$(OBJ)/BitmapDistance.o: BitmapDistance.cpp $(H)/BitmapDistance.h
	$(CC) $(CFLAGS) BitmapDistance.cpp -o $@ -c

$(OBJ)/Timer.o: Timer.cpp $(H)/Timer.h
	$(CC) $(CFLAGS) Timer.cpp -o $@ -c

$(OBJ)/ProgressStatistics.o: ProgressStatistics.cpp $(H)/ProgressStatistics.h
	$(CC) $(CFLAGS) ProgressStatistics.cpp -o $@ -c

$(OBJ)/ColorAdjust.o: ColorAdjust.cpp $(H)/ColorAdjust.h
	$(CC) $(CFLAGS) ColorAdjust.cpp -o $@ -c

$(OBJ)/VariableOptimizer.o: VariableOptimizer.cpp $(H)/VariableOptimizer.h
	$(CC) $(CFLAGS) VariableOptimizer.cpp -o $@ -c
