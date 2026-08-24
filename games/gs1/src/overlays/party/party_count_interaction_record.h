#ifndef GS1_PARTY_COUNT_INTERACTION_RECORD_H
#define GS1_PARTY_COUNT_INTERACTION_RECORD_H

#include "types.h"

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

#endif
