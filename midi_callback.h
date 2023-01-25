//
// Created by jasper on 25.01.23.
//

#ifndef MIDIFUN_MIDI_CALLBACK_H
#define MIDIFUN_MIDI_CALLBACK_H

#include "types.h"

void midi_callback(double delta_time, const unsigned char *message, size_t length, channel_t *userData);

#endif //MIDIFUN_MIDI_CALLBACK_H
