#include <inttypes.h>
#include "screamingchannels/dump.h"
#include "console/console.h"

// Reference in ble_ll_conn.h
char * ENC_STATES[10] = {"CONN_ENC_S_UNENCRYPTED", 
                         "CONN_ENC_S_ENCRYPTED",
                         "CONN_ENC_S_ENC_RSP_TO_BE_SENT",
                         "CONN_ENC_S_ENC_RSP_WAIT",
                         "CONN_ENC_S_PAUSE_ENC_RSP_WAIT",
                         "CONN_ENC_S_PAUSED",
                         "CONN_ENC_S_START_ENC_REQ_WAIT",
                         "CONN_ENC_S_START_ENC_RSP_WAIT",
                         "CONN_ENC_S_LTK_REQ_WAIT",
                         "CONN_ENC_S_LTK_NEG_REPL"};

void dump_ble_ll_conn_sm(struct ble_ll_conn_sm *connsm)
{
    // Reference in ble_ll_conn.h
    console_printf("dump_ble_ll_conn_sm(%p)\n", connsm);
    console_printf("conn_handle=%"     PRIu16 "\n", connsm->conn_handle);
    console_printf("conn_state=%"      PRIu8  "\n", connsm->conn_state);
    console_printf("conn_role=%"       PRIu8  "\n", connsm->conn_role);
    console_printf("channel_id=%"      PRIu16 "\n", connsm->channel_id);
    console_printf("data_chan_index=%" PRIu8  "\n", connsm->data_chan_index);
    console_printf("anchor_point=%"    PRIu32 "\n", connsm->anchor_point);
    console_printf("peer_addr=");
    dump_ble_addr(connsm->peer_addr);
    console_printf("\n");
    char * enc_state = get_ble_ll_conn_enc_state(connsm->enc_data.enc_state);
    console_printf("enc_data.enc_state=%s\n", enc_state);
}

void dump_ble_addr(uint8_t * addr)
{
    // Reference in ble.h
    console_printf("%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
}

char * get_ble_ll_conn_enc_state(uint8_t enc_state)
{
    return ENC_STATES[enc_state];
}
