#include <inttypes.h>
#include "screamingchannels/misc.h"
#include "console/console.h"

#define SC_CHANMAP 0x300

// Usage: is_sc_chanmap(&connsm->chanmap))
// Return 1 if it is Screaming Channels channel map, 0 otherwise.
int is_sc_chanmap(uint8_t * chanmap)
{
    return *(uint32_t *) chanmap == SC_CHANMAP;
}
