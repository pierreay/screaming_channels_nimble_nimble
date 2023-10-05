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
