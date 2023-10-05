#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "screamingchannels/console.h"
#include "screamingchannels/misc.h"
#include "screamingchannels/input.h"
#include "screamingchannels/dump.h"
#include "console/console.h"
#include "nimble/ble.h"
#include "host/ble_store.h"

#define INPUT_BASE_OFFSET 2 // Length of k: and p: used in commands.

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
        // Register key into the Nimble security database.
        // TODO: Continue to properly set value_sec and peer_addr before to move on AES.
        struct ble_store_value_sec value_sec;
        ble_addr_t peer_addr;
        peer_addr.type = 0;
        peer_addr.val[5] = 0x00;
        peer_addr.val[4] = 0x19;
        peer_addr.val[3] = 0x0e;
        peer_addr.val[2] = 0x19;
        peer_addr.val[1] = 0x79;
        peer_addr.val[0] = 0xd8;
        value_sec.peer_addr = peer_addr;
        value_sec.key_size = 16;
        value_sec.ediv = 0xdead;
        value_sec.rand_num = 0xdeadbeefdeadbeef;
        value_sec.ltk[0] = 0xde;
        value_sec.ltk[1] = 0xad;
        value_sec.ltk[2] = 0xbe;
        value_sec.ltk[3] = 0xef;
        value_sec.ltk_present = 1;
        int rc = ble_store_write_our_sec(&value_sec);
        if (rc == 0) {
            SC_INPUT_MODE = SC_INPUT_MODE_SUB;
            SC_INPUT_SUB_OK = 1;
        }
        else {
            console_printf("error: rc=%d", rc);
        }
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
        console_printf("commands: mode_train mode_attack mode_dump input_sub k: p: input_gen input_dump\n");
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
