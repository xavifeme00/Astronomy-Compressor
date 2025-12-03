
CC := $(shell command -v clang 2>/dev/null || command -v gcc 2>/dev/null)
CFLAGS = -Wall -g -Iutils -pthread
CFLAGS += -fopenmp -O3 -march=native -flto
# CFLAGS += -fsanitize=address
LIBS = -lm 

COMPRESSOR_SRC = src/compressor.c
TRAINER_SRC = src/trainer.c


COMMON_OBJ = src/utils/header.o src/utils/reader.o src/utils/sample_predictor.o src/utils/arithmetic_coder.o src/utils/context.o
COMPRESSOR_OBJ = src/compressor.o $(COMMON_OBJ)
TRAINER_OBJ = src/trainer.o $(COMMON_OBJ)



COMPRESSOR = bin/compressor
TRAINER = bin/trainer

all: $(COMPRESSOR) $(TRAINER) clean_o

$(COMPRESSOR): $(COMPRESSOR_OBJ)
	$(CC) $(CFLAGS) $(COMPRESSOR_OBJ) -o $(COMPRESSOR) $(LIBS)

$(TRAINER): $(TRAINER_OBJ)
	$(CC) $(CFLAGS) $(TRAINER_OBJ) -o $(TRAINER) $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o src/utils/*.o src/utils/*.gcno

clean_o:
	rm -f src/*.o src/utils/*.o

rebuild: clean all
