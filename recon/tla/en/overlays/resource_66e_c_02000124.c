/* Draft, differ 0 with the empty asm barrier of overlay 652's villager talk after the facing load (keeps the quadrant offset after the load; without it differ 2). Held back: the device ratchet (tla asm-barrier 197) may not rise. Tried without it: a quadrant temporary, inline helpers for the facing and the quadrant, step-by-step arithmetic. */
#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "text/msg_ids.h"

extern const u8 Data_02001370[];
extern const u8 Data_020014c0[];

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020010e0(void);
void Func_02001180(s32 a0);
void Func_02001138(s32 a0);
void Func_02001148(s32 a0, s32 a1);
void Func_020010e8(void);
void Func_02001190(s32 slot, s32 actor);
void Func_02001198(s32 slot, s32 actor);

void Func_02000124(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001190(14, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgWheneverIGet);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}
