#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "os/endian.h"
#include "screamingchannels/console.h"
#include "screamingchannels/misc.h"
#include "screamingchannels/input.h"
#include "screamingchannels/dump.h"
#include "console/console.h"
#include "nimble/ble.h"
#include "host/ble_store.h"

// Length of k: and p: used in commands.
#define INPUT_BASE_OFFSET 2

/** String hexadecimal to char decimal conversion. */
char str_hex_to_char_dec(char * str) {
    char hex_str[3];
    hex_str[0] = str[0];
    hex_str[1] = str[1];
    hex_str[2] = '\0';
    return strtol(hex_str, NULL, 16);
}

/** String hexadecimal to array of unsigned integer decimal conversion. */
void str_hex_to_uint8_dec(char * hex, uint8_t * dec, int size) {
    for (int j = 0; j < size; j++)
        dec[j] = str_hex_to_char_dec(hex + j * 2); // * 2 because 1 byte = 2 hex digit
}

static void screamingchannels_process_input(struct os_event *ev);

static struct console_input screamingchannels_console_buf;

static struct os_event screamingchannels_console_event = {
    .ev_cb = screamingchannels_process_input,
    .ev_arg = &screamingchannels_console_buf
};

/* Event callback to process a line of input from console. */
static void
screamingchannels_process_input(struct os_event *ev)
{
    char *line;
    struct console_input *input;

    input = ev->ev_arg;
    assert (input != NULL);

    line = input->line;
    /* Do some work with line */
    if (!strcmp(line, "mode_train")) {
        sc_misc_set_train_mode();
        dump_sc_state();
    }
    else if (!strcmp(line, "mode_attack")) {
        sc_misc_set_attack_mode();
        dump_sc_state();
    }
    else if (!strcmp(line, "mode_dump")) {
        dump_sc_state();
    }
    else if (!strcmp(line, "input_sub")) {
        int rc = sc_input_sub();
        if (rc == 0) {
            SC_INPUT_MODE = SC_INPUT_MODE_SUB;
            SC_INPUT_SUB_OK = 1;
        }
        else {
            console_printf("error: rc=%d", rc);
        }
    }
    // Generate a key.
    else if (line[0] == 'k' && line[1] == '?') {
        sc_input_ks_gen_print();
    }
    // Generate a plaintext.
    else if (line[0] == 'p' && line[1] == '?') {
        // TODO: Generate from random like the real SKD_P.
        // NOTE: Do not use console_printf otherwise the Python client will
        // read the undesired timestamp.
        printf("0xaabbbbbbbbbbbbbbbbbbbbbbbbbbbbaa\n");
    }
   else if (line[0] == 'k' && line[1] == ':') {
        str_hex_to_uint8_dec(line + INPUT_BASE_OFFSET, SC_INPUT_KS, INPUT_SIZE);
        SC_INPUT_SUB_OK = 0;
    }
    else if (line[0] == 'p' && line[1] == ':') {
        str_hex_to_uint8_dec(line + INPUT_BASE_OFFSET, SC_INPUT_PT, INPUT_SIZE);
        SC_INPUT_SUB_OK = 0;
    }
    else if (!strcmp(line, "input_gen")) {
        SC_INPUT_MODE = SC_INPUT_MODE_GEN;
    }
    else if (!strcmp(line, "input_dump")) {
        dump_sc_input();
    }
    else {
        console_printf("commands: mode_train mode_attack mode_dump input_sub k? p? k: p: input_gen input_dump\n");
    }
    /* Done processing line. Add the event back to the avail_queue */
    console_line_event_put(ev);
    return;
}

void sc_console_init()
{
    console_line_event_put(&screamingchannels_console_event);
    console_line_queue_set(os_eventq_dflt_get());
}
