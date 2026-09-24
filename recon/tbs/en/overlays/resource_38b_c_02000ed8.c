/* NONMATCHING: 100 of 100 bytes, 10 differing halfwords (2026-09-24), all
 * pool order: the code is exact, but the reference pool puts the address-zero
 * word first (0, 0x02000240, 0x02009d88). A HImode zero in the else branch
 * alone orders the pool right but keeps a second SImode entry; HImode in both
 * dumps the pool at the then-branch barrier. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Unit symbols: Data_02000240_t 0x02000240, Data_02009d88 0x02009d88 and
 * Data_00000000 0x00000000 (data). */
extern s32 Data_02000240_t[];
extern s16 Data_02009d88[];
extern u8 Data_00000000[];
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);

/* While the leader stands in the north-west corner, spawn a puff every thirty
 * frames; leaving the corner resets the counter. */
void Local_02000ed8(void)
{
    struct FieldActor *leader;

    leader = Engine_ActorGet(Data_02000240_t[125]);
    if (leader->x.fixed < 0x8e0000) {
        if (leader->y.fixed < 0x80000) {
            if (Data_02009d88[0] == 0)
                OverlayObject_SpawnKind24AtActor(leader);
            if (++Data_02009d88[0] == 30)
                Data_02009d88[0] = (s32)Data_00000000;
        } else {
            Data_02009d88[0] = (s32)Data_00000000;
        }
    }
}
