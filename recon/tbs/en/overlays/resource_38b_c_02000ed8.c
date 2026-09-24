/* NONMATCHING: 100 of 100 bytes, 27 differing halfwords (2026-09-24). Unit
 * symbols: Data_02000240_t 0x02000240 and Data_02009d88 0x02009d88 (data).
 * Remaining: the reference tests the counter with ldrsh and keeps a separate
 * ldrh copy for the call (zero index in r1), and its pool puts the address-zero
 * constant first; a HImode (u16)(u32) spelling moves the pool mid-function. */
#include "TYPES.H"

extern s32 Data_02000240_t[];
extern u16 Data_02009d88[];
extern u8 Data_00000000[];
u8 *Engine_ActorGet(s32 actor);
void OverlayObject_SpawnKind24AtActor(u8 *actor, s32 zero, s32 count);

/* While the leader stands in the north-west corner, spawn a puff every thirty
 * frames; leaving the corner resets the counter. The zero is the linker's
 * address-zero symbol, which the reference loads from the pool. */
void BiribinoMura_Func02000ed8(void)
{
    u8 *leader;
    s32 count;

    leader = Engine_ActorGet(Data_02000240_t[125]);
    if (*(s32 *)(leader + 8) < 0x8e0000) {
        if (*(s32 *)(leader + 12) < 0x80000) {
            count = Data_02009d88[0];
            if (*(s16 *)Data_02009d88 == 0)
                OverlayObject_SpawnKind24AtActor(leader, 0, count);
            count = Data_02009d88[0] + 1;
            Data_02009d88[0] = count;
            if ((count << 16) == 0x1e0000)
                Data_02009d88[0] = (s32)Data_00000000;
        } else {
            Data_02009d88[0] = (s32)Data_00000000;
        }
    }
}
