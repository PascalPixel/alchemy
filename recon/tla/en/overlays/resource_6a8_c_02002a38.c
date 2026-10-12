/* Near miss, differ 10: the listing reads the entrance twice up front (ldrh r2; ldrsh r3) and tests the signed read against 6 and the unsigned one against 7 and 8; this draft reads the unsigned one after the first test. A u16 local read first is folded into one load (differ 17). */
#include "types.h"
#include "party_state.h"

#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

s32 GameFlag_Test(s32 flag);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02001c24(s32 value);
void Func_020045dc(s32 value);
void Func_02002a08(void);

void Func_02002a38(void)
{
    s32 priority;

    if (GameFlag_Test(0x109) == 0) {
        struct PartyState *party = &gPartyState;

        if (FIELD_AT(party, s16, 482) == 6)
            Func_02001c24(0);
        else if ((u32)((FIELD_AT(party, u16, 482) - 7) << 16) <= 0x10000)
            Func_02001c24(1);
    }
    Func_020045dc(0);
    priority = 0x480;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ builds the priority before it loads the callback's address (2 differ without it). */
    Scheduler_AddOrUpdateCallback(Func_02002a08, priority);
}
