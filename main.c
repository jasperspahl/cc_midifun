#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <rtmidi/rtmidi_c.h>

#include "types.h"
#include "midi_callback.h"

bool done = false;
void finish() {
    done = true;
}

int main() {
    RtMidiInPtr midi_in = rtmidi_in_create_default();
    if (!midi_in->ok) {
        fprintf(stderr, "Error creating RtMidiIn: %s\n", midi_in->msg);
        goto cleanup;
    }
    unsigned int port_count = rtmidi_get_port_count(midi_in);
    if (!midi_in->ok) {
        fprintf(stderr, "Error getting port count: %s\n", midi_in->msg);
        goto cleanup;
    }
    if (port_count == 0) {
        fprintf(stderr, "No ports found!\n");
        goto cleanup;
    }
    printf("Found %d ports\n", port_count);
    for (unsigned int i = 0; i < port_count; i++) {
        int buf_len = 0;
        rtmidi_get_port_name(midi_in, i, NULL, &buf_len);
        if (!midi_in->ok) {
            fprintf(stderr, "Error getting port name length: %s\n", midi_in->msg);
            goto cleanup;
        }
        char *buf = malloc(buf_len);
        rtmidi_get_port_name(midi_in, i, buf, &buf_len);
        if (!midi_in->ok) {
            fprintf(stderr, "Error getting port name: %s\n", midi_in->msg);
            goto cleanup;
        }
        printf("\t %d: %s\n", i, buf);
        free(buf);
    }
    printf("Connecting to port 0\n");

    rtmidi_open_port(midi_in, 0, "Fun with Midi\n");
    if (!midi_in->ok) {
        fprintf(stderr, "Error opening port: %s\n", midi_in->msg);
        goto cleanup;
    }
    channel_t channels[16] = {0};
    for (int i = 0; i < 16; ++i) {
        channels[i].pitch = 0x2000;
    }
    rtmidi_in_set_callback(midi_in, (RtMidiCCallback) &midi_callback, channels);
    if (!midi_in->ok) {
        fprintf(stderr, "Error setting callback: %s\n", midi_in->msg);
        goto cleanup;
    }
    signal(SIGINT, finish);
    while(!done)
    {
        printf("\033[2J\033[1;1H");
        printf("Channels:\n");
        for (int i = 0; i < 1; ++i) {
            printf("\tChannel %d:\n", i);
            printf("\t\tPitch: %x\n", channels[i].pitch);
            printf("\t\tControl values:\n");
            for (int j = 0; j < 128; ++j) {
                if (channels[i].control_values[j] != 0) {
                    printf("\t\t\t%d: %d\n", j, channels[i].control_values[j]);
                }
            }
            printf("\t\tNotes:\n");
            for (int j = 0; j < 128; ++j) {
                if (channels[i].notes[j].playing) {
                    printf("\t\t\t%d: %d\n", j, channels[i].notes[j].status);
                }
            }
        }
        printf("\n");
        sleep(1);
    }

cleanup:
    rtmidi_in_free(midi_in);
    return 0;
}
