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

void sc_misc_set_train_mode();
void sc_misc_set_attack_mode();
void sc_misc_set_sc_conn(uint8_t * chanmap);
int sc_misc_is_sc_chanmap(uint8_t * chanmap);

#ifdef __cplusplus
}
#endif

#endif
