#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "midi_callback.h"

#ifdef DEBUG_VERBOSE
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)  // do nothing
#endif

void midi_callback(double delta_time, const unsigned char *message, size_t length, channel_t * userData ) {
    DEBUG_PRINT("callback called\n");
    DEBUG_PRINT("delta time: %f\n", delta_time);
    DEBUG_PRINT("user data: %p\n", userData);
    uint16_t pitch_value;
    uint8_t midi_channel = message[0] & 0x0f;
    DEBUG_PRINT("Midi channel: %d\n", midi_channel);
    switch (message[0] & 0xf0) {
        case NOTE_OFF:
            assert(length == 3);
            DEBUG_PRINT("Note off:\n\tNote: %d\n\tVelocity: %d\n", message[1], message[2]);
            userData[midi_channel].notes[message[1]].playing = false;
            userData[midi_channel].notes[message[1]].status = message[2];
            break;
        case NOTE_ON:
            assert(length == 3);
            DEBUG_PRINT("Note on:\n\tNote: %d\n\tVelocity: %d\n", message[1], message[2]);
            userData[midi_channel].notes[message[1]].playing = true;
            userData[midi_channel].notes[message[1]].status = message[2];
            break;
        case POLY_PRESSURE:
            assert(length == 3);
            DEBUG_PRINT("Poly pressure:\n\tNote: %d\n\tPressure: %d\n", message[1], message[2]);
            userData[midi_channel].notes[message[1]].status = message[2];
            break;
        case CONTROL_CHANGE:
            assert(length == 3);
            DEBUG_PRINT("Control change:\n\tController: %d\n\tValue: %d\n", message[1], message[2]);
            userData[midi_channel].control_values[message[1]] = message[2];
            break;
        case PROGRAM_CHANGE:
            assert(length == 2);
            DEBUG_PRINT("Program change:\n\tProgram: %d\n", message[1]);
            break;
        case CHANNEL_PRESSURE:
            assert(length == 2);
            DEBUG_PRINT("Channel pressure:\n\tPressure: %d\n", message[1]);
            break;
        case PITCH_BEND:
            assert(length == 3);
            pitch_value = (message[2] << 7) | message[1];
            DEBUG_PRINT("Pitch bend:\n\tValue: %x\n", pitch_value);
            userData[midi_channel].pitch = pitch_value;
            break;
        default:
            DEBUG_PRINT("Unknown message: 0x%x\n", message[0] & 0xf0);
            for (int i = 0; i < length; i++) {
                DEBUG_PRINT("%x ", message[i]);
            }
            break;
    }
    DEBUG_PRINT("\n");
}
