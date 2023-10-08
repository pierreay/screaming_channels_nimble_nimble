#include <inttypes.h>
#include "host/ble_store.h"
#include "screamingchannels/dump.h"
#include "screamingchannels/misc.h"
#include "screamingchannels/input.h"
#include "console/console.h"

// Reference in ble_ll_conn.h
char *ENC_STATES[10] = {
    "CONN_ENC_S_UNENCRYPTED",        "CONN_ENC_S_ENCRYPTED",
    "CONN_ENC_S_ENC_RSP_TO_BE_SENT", "CONN_ENC_S_ENC_RSP_WAIT",
    "CONN_ENC_S_PAUSE_ENC_RSP_WAIT", "CONN_ENC_S_PAUSED",
    "CONN_ENC_S_START_ENC_REQ_WAIT", "CONN_ENC_S_START_ENC_RSP_WAIT",
    "CONN_ENC_S_LTK_REQ_WAIT",       "CONN_ENC_S_LTK_NEG_REPL"};

// Use with care as it introduces too much timing to complete a pairing.
void dump_tc_aes_key_sched_struct(struct tc_aes_key_sched_struct *g_ctx)
{
    // See aes.h::64 for structure definition.
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_tc_aes_key_sched_struct(g_ctx=%p)\n", g_ctx);
#endif
    for (int i = 0; i < Nb*(Nr+1); i++) {
        console_printf("[v] g_ctx->words[%d]=0x%08x\n", i, g_ctx->words[i]);
        if (i == 3)
            break; // Don't break pairing because of timing.
    }
    console_printf("[!] skip remaining words\n");
#endif
}

void dump_ble_encryption_block(struct ble_encryption_block *ecb)
{
    // See ble.h::41 for structure definition.
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_ble_encryption_block(ecb=%p)\n", ecb);
#endif
    console_printf("[v] ecb->key=0x");
    for (int i = 0; i < BLE_ENC_BLOCK_SIZE; i++) {
        console_printf("%02x", ecb->key[i]);
    }
    console_printf("\n");
    console_printf("[v] ecb->plain_text=0x");
    for (int i = 0; i < BLE_ENC_BLOCK_SIZE; i++) {
        console_printf("%02x", ecb->plain_text[i]);
    }
    console_printf("\n");
    console_printf("[v] ecb->cipher_text=0x");
    for (int i = 0; i < BLE_ENC_BLOCK_SIZE; i++) {
        console_printf("%02x", ecb->cipher_text[i]);
    }
    console_printf("\n");
#endif
}

void dump_ble_ll_conn_sm(struct ble_ll_conn_sm *connsm)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_ble_ll_conn_sm(connsm=%p)\n", connsm);
#endif
    // Reference in ble_ll_conn.h
    console_printf("[v] conn_handle=%"     PRIu16 "\n", connsm->conn_handle);
    console_printf("[v] conn_state=%"      PRIu8  "\n", connsm->conn_state);
    console_printf("[v] conn_role=%"       PRIu8  "\n", connsm->conn_role);
    console_printf("[v] channel_id=%"      PRIu16 "\n", connsm->channel_id);
    console_printf("[v] data_chan_index=%" PRIu8  "\n", connsm->data_chan_index);
    console_printf("[v] anchor_point=%"    PRIu32 "\n", connsm->anchor_point);
    console_printf("[v] peer_addr=\n");
    dump_addr(connsm->peer_addr);
    console_printf("\n");
    console_printf("[v] chanmap=");
    dump_ble_chanmap(connsm->chanmap);
    console_printf("\n");
    char * enc_state = get_ble_ll_conn_enc_state(connsm->enc_data.enc_state);
    console_printf("[v] enc_data.enc_state=%s\n", enc_state);
#endif
}

void dump_ble_ll_conn_enc_data(struct ble_ll_conn_enc_data *enc_data)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_ble_ll_conn_enc_data(enc_data=%p)\n", enc_data);
#endif
    int cnt;
    console_printf("[v] LTK:enc_data->enc_block.key=0x");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", enc_data->enc_block.key[cnt]);
    }
    console_printf("\n");
    console_printf("[v] SKD:enc_data->enc_block.plain_text=0x");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", enc_data->enc_block.plain_text[cnt]);
    }
    console_printf("\n");
    console_printf("[v] SK:enc_data->enc_block.cipher_text=0x");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", enc_data->enc_block.cipher_text[cnt]);
    }
    console_printf("\n");
    console_printf("[v] IV:enc_data->iv=0x");
    for (cnt = 0; cnt < 8; ++ cnt) {
        console_printf("%02x", enc_data->iv[cnt]);
    }
    console_printf("\n");
#endif
}

void dump_ble_addr(ble_addr_t addr)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    console_printf("addr.type=%" PRIu8 ";", addr.type);
    console_printf("addr.val=");
    dump_addr(addr.val);
#endif
}

void dump_addr(uint8_t * addr)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    // Reference in ble.h
    console_printf("%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
#endif
}

void dump_ble_chanmap(uint8_t * chanmap)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    // Reference in ble_ll_conn.h::243 and ble_ll.h::94.
    console_printf("0x%02hhx%02hhx%02hhx%02hhx%02hhx", chanmap[4], chanmap[3], chanmap[2], chanmap[1], chanmap[0]);
#endif
}

char * get_ble_ll_conn_enc_state(uint8_t enc_state)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    return ENC_STATES[enc_state - 1];
#endif
}

void dump_sc_state()
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_sc_state()\n");
#endif
    console_printf("[v] IS_SC_TRAIN=%d\n", IS_SC_TRAIN);
    console_printf("[v] IS_SC_ATTACK=%d\n", IS_SC_ATTACK);
    console_printf("[v] IS_SC_CONN=%d\n", IS_SC_CONN);
#endif
}

void dump_sc_input()
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_sc_input()\n");
#endif
    console_printf("[v] SC_INPUT_MODE=%s\n", sc_input_get_input_mode_str());
    console_printf("[v] SC_INPUT_SUB_OK=%d\n", SC_INPUT_SUB_OK);
    console_printf("[v] SC_INPUT_KS=");
    dump_hex_uint8(SC_INPUT_KS, INPUT_SIZE, SC_DUMP_BIG_ENDIAN);
    console_printf("[v] SC_INPUT_PT=");
    dump_hex_uint8(SC_INPUT_PT, INPUT_SIZE, SC_DUMP_BIG_ENDIAN);
    if (SC_INPUT_SUB_OK)
        dump_ble_store_value_sec(&SC_INPUT_VALUE_SEC);
#endif
}

void dump_hex_uint8(uint8_t * hex, int size, int endianness) {
    console_printf("0x");
    if (endianness == SC_DUMP_BIG_ENDIAN) {
        for (int i = 0; i < size; i++)
            console_printf("%02x", hex[i]);
    }
    else if (endianness == SC_DUMP_LITTLE_ENDIAN) {
        for (int i = size - 1; i >= 0; i--)
            console_printf("%02x", hex[i]);
    }
    console_printf("\n");
}

void dump_ble_store_value_sec(struct ble_store_value_sec *value_sec) {
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("[dump.c] dump_ble_store_value_sec(value_sec=%p)\n", value_sec);
#endif
    console_printf("[v] value_sec->ltk=");
    // TODO: Change this to LITTLE_ENDIAN ? To check using real pairing.
    dump_hex_uint8(value_sec->ltk, INPUT_SIZE, SC_DUMP_BIG_ENDIAN);
    console_printf("[v] value_sec->ediv=%#hx\n", value_sec->ediv);
    console_printf("[v] value_sec->rand_num=%#llx\n", value_sec->rand_num);
    console_printf("[v] value_sec->peer_addr=");
    dump_ble_addr(value_sec->peer_addr);
    console_printf("\n");
    return;
#endif
}
