#include "TYPES.H"

s32 Engine_ActorGet();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_000000ac[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Babi's tunnel: publish the player actor at +24 of the event work once it
 * has passed the row limit of the current area and story step. */
void BabiChika_UpdateTrackedActor(void)
{
    u8 *actor;
    u8 *work;
    s32 limit;

    actor = (u8 *)Engine_ActorGet(0);
    work = *(u8 **)0x03001ee0;
    limit = 0;
    if (Data_02000240_t[224][0] == (s32)Data_000000ac) {
        switch (Data_02000240_t[225][0]) {
        case 3:
        case 4:
            limit = 94;
            break;
        case 8:
        case 9:
            limit = 74;
            break;
        case 12:
        case 13:
            limit = 118;
            break;
        }
    } else if (Data_02000240_t[225][0] == 12) {
        limit = 93;
    }
    if ((*(s32 *)(actor + 16) >> 19) <= limit) {
        *(s32 *)(work + 24) = 0;
    } else {
        *(s32 *)(work + 24) = (s32)actor;
    }
}
