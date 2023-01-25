//
// Created by jasper on 25.01.23.
//

#ifndef MIDIFUN_TYPES_H
#define MIDIFUN_TYPES_H
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t status;
    bool playing;
} note_t;

typedef struct {
    uint16_t pitch;
    note_t notes[128];
    uint8_t control_values[128];
} channel_t;

enum StatusByte{
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    POLY_PRESSURE = 0xa0,
    CONTROL_CHANGE = 0xb0,
    PROGRAM_CHANGE = 0xc0, // only 1 byte
    CHANNEL_PRESSURE = 0xd0, // only 1 byte
    PITCH_BEND = 0xe0,
};

enum MidiMask{
    CHANNEL_MASK = 0x0f,
    STATUS_MASK = 0xf0,
};
#endif //MIDIFUN_TYPES_H
