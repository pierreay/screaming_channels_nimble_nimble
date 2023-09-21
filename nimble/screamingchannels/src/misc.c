#include <inttypes.h>
#include "screamingchannels/misc.h"
#include "console/console.h"

#define SC_CHANMAP 0x300

int IS_SC_TRAIN  = 0;
int IS_SC_ATTACK = 0;
int IS_SC_CONN = 0;

void sc_misc_set_train_mode()
{
    IS_SC_TRAIN = 1;
    IS_SC_ATTACK = 0;
}

void sc_misc_set_attack_mode()
{
    IS_SC_TRAIN = 0;
    IS_SC_ATTACK = 1;
}

void sc_misc_set_sc_conn(uint8_t * chanmap)
{
    IS_SC_CONN = sc_misc_is_sc_chanmap(chanmap);
}

// Usage: sc_misc_is_sc_chanmap(&connsm->chanmap))
// Return 1 if it is Screaming Channels channel map, 0 otherwise.
int sc_misc_is_sc_chanmap(uint8_t * chanmap)
{
    return *(uint32_t *) chanmap == SC_CHANMAP;
}
