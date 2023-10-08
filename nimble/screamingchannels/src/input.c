#include <inttypes.h>
#include "screamingchannels/input.h"
#include "console/console.h"

// By default, the AES inputs generation method is the Nimble generation during
// pairing.
int SC_INPUT_MODE = SC_INPUT_MODE_GEN;
// By default, the AES inputs submission state is False.
int SC_INPUT_SUB_OK = 0;

// By default, allocate needed size for inputs.
uint8_t SC_INPUT_KS[INPUT_SIZE];
uint8_t SC_INPUT_PT[INPUT_SIZE];

struct ble_store_value_sec SC_INPUT_VALUE_SEC;
ble_addr_t SC_INPUT_PEER_ADDR;

/* * Private */

/** Set DEST byte array to the address of length 6 represented by the SRC string. */
void set_ble_addr(char * src, uint8_t * dest) {
    sscanf(src, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &dest[5], &dest[4], &dest[3], &dest[2], &dest[1], &dest[0]);
}

/* * Public */

char * sc_input_get_input_mode_str()
{
    if (SC_INPUT_MODE == SC_INPUT_MODE_GEN) {
        return "SC_INPUT_MODE_GEN";
    }
    else if (SC_INPUT_MODE == SC_INPUT_MODE_SUB) {
        return "SC_INPUT_MODE_SUB";
    }
    return "";
}

int sc_input_sub() {
    // Register the BLE HCI dongle address in security database structure.
    SC_INPUT_PEER_ADDR.type = 0;
    set_ble_addr("00:19:0e:19:79:d8", SC_INPUT_PEER_ADDR.val);
    SC_INPUT_VALUE_SEC.peer_addr = SC_INPUT_PEER_ADDR;
    // Register the EDIV and RAND inside the security database structure.
    SC_INPUT_VALUE_SEC.ediv = 0xdead;
    SC_INPUT_VALUE_SEC.rand_num = 0xdeadbeefdeadbeef;
    swap_in_place(&SC_INPUT_VALUE_SEC.rand_num, 8);
    // Register the LTK inside the security database structure.
    swap_buf(SC_INPUT_VALUE_SEC.ltk, SC_INPUT_KS, INPUT_SIZE);
    SC_INPUT_VALUE_SEC.ltk_present = 1;
    SC_INPUT_VALUE_SEC.key_size = 16;
    // Register the hand-crafted structure inside the Nimble security database.
    return ble_store_write_our_sec(&SC_INPUT_VALUE_SEC);
}

void sc_input_set_to_conn_enc_data(struct ble_ll_conn_enc_data *enc_data) {
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[input.c] sc_input_set_to_conn_enc_data(enc_data=%p)\n", enc_data);
#endif
    for (int cnt = 0; cnt < 16; ++cnt)
        enc_data->enc_block.plain_text[cnt] = SC_INPUT_PT[cnt];
}
