OUTPUT_DIR         := build
SRC_DIR            := src
SOURCES            := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJECTS_TO_COMPILE := $(SOURCES:.c=.o)
OBJECTS_COMPILED   := $(addprefix $(OUTPUT_DIR)/, $(OBJECTS_TO_COMPILE))
INCLUDE            := $(foreach sdir,$(SRC_DIR),-I$(sdir)/include)
CC                 := gcc
CFLAGS             := $(INCLUDE) -c -O0
LFLAGS             :=
PFLAGS             :=
EXECUTABLE         := nathan_barrett_morrison_longest

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS_TO_COMPILE)
	$(CC) $(LFLAGS) $(OBJECTS_COMPILED) $(PFLAGS) -o $(OUTPUT_DIR)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $(OUTPUT_DIR)/$@

clean:
	rm -f $(OBJECTS_COMPILED) $(OUTPUT_DIR)/$(EXECUTABLE)

