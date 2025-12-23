CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -lrt -g -O2
LDFLAGS = -pthread -lrt
INCLUDES = -Iinclude

TARGET_MAIN = hospital_system
TARGET_TRIAGE = triage
TARGET_SURGERY = surgery
TARGET_PHARMACY = pharmacy
TARGET_LAB = laboratory

COMMON_OBJS = \
	src/patient_thread.o \
	src/ipc_utils.o \
	src/sync_utils.o \
	src/log_manager.o \
	src/stats_manager.o \
	src/config_parser.o \
	src/time_simulation.o

all: $(TARGET_MAIN) $(TARGET_TRIAGE) $(TARGET_SURGERY) $(TARGET_PHARMACY) $(TARGET_LAB)

$(TARGET_MAIN): src/main.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_TRIAGE): src/triage.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_SURGERY): src/surgery.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_PHARMACY): src/pharmacy.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_LAB): src/laboratory.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f src/*.o \
	$(TARGET_MAIN) $(TARGET_TRIAGE) \
	$(TARGET_SURGERY) $(TARGET_PHARMACY) $(TARGET_LAB)

.PHONY: all clean