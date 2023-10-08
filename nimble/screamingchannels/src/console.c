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

/** Set DEST byte array to the address of length 6 represented by the SRC string. */
void set_ble_addr(char * src, uint8_t * dest) {
    sscanf(src, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &dest[5], &dest[4], &dest[3], &dest[2], &dest[1], &dest[0]);
}

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
        // PROG: Continue to properly set SC_INPUT_VALUE_SEC and SC_INPUT_PEER_ADDR before to move on AES.
        SC_INPUT_PEER_ADDR.type = 0;
        set_ble_addr("00:19:0e:19:79:d8", SC_INPUT_PEER_ADDR.val);
        SC_INPUT_VALUE_SEC.peer_addr = SC_INPUT_PEER_ADDR;
        SC_INPUT_VALUE_SEC.key_size = 16;
        // PROG: Set correctly those values to be able to do a connection
        // without pairing. Inspect the failure error in the ble_store file? Is
        // this an endianness issue? Current values on Reaper:
        /* addr: 00:19:0E:19:79:D8
         * ediv: 0x6d2e
         * ltk:  0943bcf900dffe4e89da71840df77a9b
         * rand: 8ec42b71e9092ba0
         */
        SC_INPUT_VALUE_SEC.ediv = 0x6d2e;
        swap_in_place(&SC_INPUT_VALUE_SEC.ediv, 2);
        SC_INPUT_VALUE_SEC.rand_num = 0x8ec42b71e9092ba0;
        swap_in_place(&SC_INPUT_VALUE_SEC.rand_num, 8);
        swap_buf(SC_INPUT_VALUE_SEC.ltk, SC_INPUT_KS, INPUT_SIZE);
        SC_INPUT_VALUE_SEC.ltk_present = 1;
        int rc = ble_store_write_our_sec(&SC_INPUT_VALUE_SEC);
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
