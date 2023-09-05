#include <inttypes.h>
#include "screamingchannels/misc.h"
#include "console/console.h"

#define SC_CHANMAP 0x300

int IS_SC_TRAIN  = 0;
int IS_SC_ATTACK = 0;

void set_is_sc_train(uint8_t * chanmap)
{
    IS_SC_TRAIN = is_sc_chanmap(chanmap);
}

void set_is_sc_attack(uint8_t * chanmap)
{
    IS_SC_ATTACK = is_sc_chanmap(chanmap);
}

// Usage: is_sc_chanmap(&connsm->chanmap))
// Return 1 if it is Screaming Channels channel map, 0 otherwise.
int is_sc_chanmap(uint8_t * chanmap)
{
    return *(uint32_t *) chanmap == SC_CHANMAP;
}
