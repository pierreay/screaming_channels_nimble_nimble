#include <inttypes.h>
#include "screamingchannels/dump.h"
#include "console/console.h"

// Reference in ble_ll_conn.h
char *ENC_STATES[10] = {
    "CONN_ENC_S_UNENCRYPTED",        "CONN_ENC_S_ENCRYPTED",
    "CONN_ENC_S_ENC_RSP_TO_BE_SENT", "CONN_ENC_S_ENC_RSP_WAIT",
    "CONN_ENC_S_PAUSE_ENC_RSP_WAIT", "CONN_ENC_S_PAUSED",
    "CONN_ENC_S_START_ENC_REQ_WAIT", "CONN_ENC_S_START_ENC_RSP_WAIT",
    "CONN_ENC_S_LTK_REQ_WAIT",       "CONN_ENC_S_LTK_NEG_REPL"};

void dump_ble_ll_conn_sm(struct ble_ll_conn_sm *connsm)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("dump_ble_ll_conn_sm(connsm=%p)\n", connsm);
#endif
    // Reference in ble_ll_conn.h
    console_printf("conn_handle=%"     PRIu16 "\n", connsm->conn_handle);
    console_printf("conn_state=%"      PRIu8  "\n", connsm->conn_state);
    console_printf("conn_role=%"       PRIu8  "\n", connsm->conn_role);
    console_printf("channel_id=%"      PRIu16 "\n", connsm->channel_id);
    console_printf("data_chan_index=%" PRIu8  "\n", connsm->data_chan_index);
    console_printf("anchor_point=%"    PRIu32 "\n", connsm->anchor_point);
    console_printf("peer_addr=");
    dump_ble_addr(connsm->peer_addr);
    console_printf("\n");
    console_printf("chanmap=");
    dump_ble_chanmap(connsm->chanmap);
    console_printf("\n");
    char * enc_state = get_ble_ll_conn_enc_state(connsm->enc_data.enc_state);
    console_printf("enc_data.enc_state=%s\n", enc_state);
#endif
}

void dump_ble_ll_conn_enc_data(struct ble_ll_conn_enc_data *enc_data)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
#if MYNEWT_VAL(SC_LOG_TRACE_ENABLE)
    console_printf("dump_ble_ll_conn_enc_data(enc_data=%p)\n", enc_data);
#endif
    int cnt;
    console_printf("\nLTK (key):");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", enc_data->enc_block.key[cnt]);
    }
    console_printf("\nSKD (plain_text):");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", enc_data->enc_block.plain_text[cnt]);
    }
    console_printf("\nSession Key (cipher_text):");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", enc_data->enc_block.cipher_text[cnt]);
    }
    console_printf("\nIV:");
    for (cnt = 0; cnt < 8; ++ cnt) {
        console_printf("%02x", enc_data->iv[cnt]);
    }
    console_printf("\n");
#endif
}

void dump_ble_addr(uint8_t * addr)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    // Reference in ble.h
    console_printf("%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
#endif
}

void dump_ble_chanmap(uint8_t * chanmap)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    // Reference in ble_ll.h
    console_printf("0x%02hhx%02hhx%02hhx%02hhx%02hhx", chanmap[4], chanmap[3], chanmap[2], chanmap[1], chanmap[0]);
#endif
}

char * get_ble_ll_conn_enc_state(uint8_t enc_state)
{
#if MYNEWT_VAL(SC_LOG_DUMP_ENABLE)
    return ENC_STATES[enc_state - 1];
#endif
}
