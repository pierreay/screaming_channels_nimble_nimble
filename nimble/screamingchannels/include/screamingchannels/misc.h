#ifndef MISC_H
#define MISC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Global variables used accross other Nimble packages used to know if the
// current connection is a Screaming Channels attack or profile collection.
extern int IS_SC_TRAIN;
extern int IS_SC_ATTACK;
extern int IS_SC_CONN;

void set_is_sc_train(uint8_t * chanmap);
void set_is_sc_attack(uint8_t * chanmap);
int is_sc_chanmap(uint8_t * chanmap);

#ifdef __cplusplus
}
#endif

#endif
