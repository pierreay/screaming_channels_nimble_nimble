#ifndef DUMP_H
#define DUMP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "controller/ble_ll_conn.h"

void dump_ble_ll_conn_sm(struct ble_ll_conn_sm *connsm);
void dump_ble_addr(uint8_t * addr);
void dump_ble_chanmap(uint8_t * chanmap);
char * get_ble_ll_conn_enc_state(uint8_t enc_state);

#ifdef __cplusplus
}
#endif

#endif
