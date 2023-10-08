#ifndef DUMP_H
#define DUMP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "tinycrypt/aes.h"
#include "host/ble_store.h"
#include "controller/ble_ll_conn.h"
#include "nimble/ble.h"

// Number must be read in memory address incrementation.
#define SC_DUMP_BIG_ENDIAN 0
// Number must be read in memory address decrementation.
#define SC_DUMP_LITTLE_ENDIAN 1

/* * General structures */

/** Dump hexadecimal number from uint8_t array into console. */
void dump_hex_uint8(const uint8_t * hex, int size, int endianness);

/* * Screaming Channels structures */

void dump_sc_state();

/** Dump Screaming Channels Input
*
* Dump information relative to the sc_input module (input generation mode and
* values).
*/
void dump_sc_input();

/* * Nimble structures */

void dump_tc_aes_key_sched_struct(struct tc_aes_key_sched_struct *g_ctx);
void dump_ble_encryption_block(struct ble_encryption_block *ecb);
void dump_ble_ll_conn_sm(struct ble_ll_conn_sm *connsm);
void dump_ble_ll_conn_enc_data(struct ble_ll_conn_enc_data *enc_data);
void dump_ble_addr(ble_addr_t addr);
void dump_addr(uint8_t * addr);
void dump_ble_chanmap(uint8_t * chanmap);
char * get_ble_ll_conn_enc_state(uint8_t enc_state);
/** Dump the values contained inside struct ble_store_value_sec into console. */
void dump_ble_store_value_sec(const struct ble_store_value_sec *value_sec);
/** Dump the values contained inside struct ble_store_key_sec into console. */
void dump_ble_store_key_sec(const struct ble_store_key_sec *key_sec);

#ifdef __cplusplus
}
#endif

#endif
