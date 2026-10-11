/* Near miss (2 instructions differ, order only): the native sets r1 to
 * 99 before it loads the scene id. Forwarding the call through the inline
 * and keeping the byte store in a one-pass block each fix one of the two
 * orders but not both together. */
#include "types.h"
#include "party_state.h"

extern char SceneId_Ov694Room1;

struct PartyState2c2 {
    u8 unknown_000[0x2c2];
    u8 value;
};

void Battle_WaitMode0(s32 frames);
void Func_020000f4(void);
void Func_02000158(void);
void Func_02003448(s32 scene, s32 entrance);
void Func_020034b0(void);

static __inline__ void Func_02003448_(s32 scene, s32 entrance)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003448(scene, entrance);
}

void Func_02001c60(void)
{
    Func_020000f4();
    Func_020034b0();
    /* FAKEMATCH: the one-pass block keeps the gPartyState address load before the offset; as a plain statement the offset is built first (differ 2) */
    do {
        ((struct PartyState2c2 *)&gPartyState)->value = 2;
    } while (0);
    Func_02003448_((s32)&SceneId_Ov694Room1, 99);
    Battle_WaitMode0(40);
    Func_02000158();
}
