#include "layout_guard.h"
#include "types.h"

#define NULL ((void *)0)

struct LinkPacket_0800655c {
    u8 sequence;
    u8 acknowledgement;
    u8 receive_state;
    u8 send_state;
    u8 payload[20];
};

struct Dma_0800655c {
    const void *source;
    void *destination;
    u32 control;
};

LAYOUT_OFFSET_GUARD(
    LinkPacket0800655c_ReceiveState,
    struct LinkPacket_0800655c,
    receive_state,
    0x02);
LAYOUT_OFFSET_GUARD(
    LinkPacket0800655c_SendState,
    struct LinkPacket_0800655c,
    send_state,
    0x03);
LAYOUT_OFFSET_GUARD(
    LinkPacket0800655c_Payload,
    struct LinkPacket_0800655c,
    payload,
    0x04);
LAYOUT_SIZE_GUARD(LinkPacket0800655c_Size, struct LinkPacket_0800655c, 0x18);

static volatile struct LinkPacket_0800655c *const LocalPacket_0800655c =
    (volatile struct LinkPacket_0800655c *)0x02002220;
static volatile struct LinkPacket_0800655c *const LinkPackets_0800655c =
    (volatile struct LinkPacket_0800655c *)0x02002020;
static u8 *volatile *const ReceiveDestination_0800655c =
    (u8 *volatile *)0x020023AC;
static u8 *volatile *const SendSource_0800655c =
    (u8 *volatile *)0x02002080;
static volatile u8 *const Sequence_0800655c = (volatile u8 *)0x020023A4;
static volatile u16 *const ReceivedBytes_0800655c =
    (volatile u16 *)0x02002238;
static volatile u16 *const RemainingBytes_0800655c =
    (volatile u16 *)0x02002008;
static volatile u16 *const LinkFlags_0800655c = (volatile u16 *)0x03001F64;
static volatile u32 *const SerialControl_0800655c =
    (volatile u32 *)0x04000128;
static volatile struct Dma_0800655c *const Dma3_0800655c =
    (volatile struct Dma_0800655c *)0x040000D4;

static void CopyBlock_0800655c(const void *source, void *destination) {
    Dma3_0800655c->source = source;
    Dma3_0800655c->destination = destination;
    Dma3_0800655c->control = 0x84000005;
}

static void ReceivePeerBlock_0800655c(
    volatile struct LinkPacket_0800655c *peer
) {
    u8 *destination = *ReceiveDestination_0800655c;

    if (destination == NULL) {
        return;
    }
    if (LocalPacket_0800655c->receive_state != 1 ||
        (u8)(peer->send_state - 1) > 1) {
        LocalPacket_0800655c->sequence = 0;
        return;
    }

    if (peer->sequence == (*Sequence_0800655c & 0x7f)) {
        LocalPacket_0800655c->sequence = 0;
        if (peer->send_state == 1) {
            CopyBlock_0800655c((const void *)peer->payload, destination);
            *ReceiveDestination_0800655c += 20;
            *ReceivedBytes_0800655c += 20;
            LocalPacket_0800655c->acknowledgement =
                (u8)((LocalPacket_0800655c->acknowledgement + 1) | 0x80);
        } else if (peer->send_state == 2) {
            CopyBlock_0800655c((const void *)peer->payload, destination);
            *ReceivedBytes_0800655c += 20;
            LocalPacket_0800655c->receive_state = 2;
            LocalPacket_0800655c->acknowledgement = 0;
            LocalPacket_0800655c->sequence = 1;
        }
        *Sequence_0800655c = (u8)((*Sequence_0800655c + 1) & 0x7f);
        return;
    }

    if (*Sequence_0800655c & 0x80) {
        if (LocalPacket_0800655c->sequence & 0x80) {
            LocalPacket_0800655c->sequence = 1;
        } else if (LocalPacket_0800655c->sequence == 1) {
            LocalPacket_0800655c->sequence = 0;
            *Sequence_0800655c &= 0x7f;
        }
    } else {
        LocalPacket_0800655c->sequence = *Sequence_0800655c | 0x80;
        *Sequence_0800655c |= 0x80;
    }
}

static void SendLocalBlock_0800655c(
    volatile struct LinkPacket_0800655c *peer
) {
    u8 *source = *SendSource_0800655c;

    if (source == NULL) {
        return;
    }

    if (peer->receive_state == 1) {
        if (peer->sequence & 0x80) {
            u8 missed = (u8)((*Sequence_0800655c - peer->sequence) & 0x7f);
            s32 rewind = missed * 20;

            *SendSource_0800655c = source - rewind;
            *RemainingBytes_0800655c += rewind;
            *Sequence_0800655c =
                (u8)((*Sequence_0800655c - missed) & 0x7f);
            source = *SendSource_0800655c;
        }

        if (*RemainingBytes_0800655c != 0) {
            CopyBlock_0800655c(source, (void *)LocalPacket_0800655c->payload);
            *RemainingBytes_0800655c -= 20;
            LocalPacket_0800655c->send_state =
                *RemainingBytes_0800655c != 0 ? peer->receive_state : 2;
            LocalPacket_0800655c->sequence = *Sequence_0800655c & 0x7f;
            *SendSource_0800655c += 20;
            *Sequence_0800655c = (u8)((*Sequence_0800655c + 1) & 0x7f);
        }
    }

    if (LocalPacket_0800655c->send_state == 2 && peer->receive_state == 2) {
        *SendSource_0800655c = NULL;
        LocalPacket_0800655c->send_state = 0;
        LocalPacket_0800655c->sequence = 1;
    }
}

static void SynchronizeReceiveState_0800655c(
    volatile struct LinkPacket_0800655c *peer
) {
    if (LocalPacket_0800655c->receive_state == 2) {
        if (peer->send_state != 2) {
            *ReceiveDestination_0800655c = NULL;
            LocalPacket_0800655c->receive_state = 0;
        }
    } else {
        LocalPacket_0800655c->receive_state =
            *ReceiveDestination_0800655c != NULL ? 1 : 0;
    }
}

/* Advance one 20-byte block of the bidirectional link-transfer handshake. */
void Func_0800655c(void) {
    s32 channel;
    volatile struct LinkPacket_0800655c *peer;

    channel = 1 ^ ((*SerialControl_0800655c >> 4) & 1);
    peer = &LinkPackets_0800655c[channel];
    if ((*LinkFlags_0800655c & 3) != 3) {
        return;
    }

    ReceivePeerBlock_0800655c(peer);
    SendLocalBlock_0800655c(peer);
    SynchronizeReceiveState_0800655c(peer);
}
