/* Draft, differ 6 of 72 (differ 2 with the coins in a temporary before the store): the native
 * loads the scene state's address into r3 before it loads the coins into r2 (ldr r3, =gSceneState;
 * ldr r2, [r6, #16]; str r2, [r3]); written plainly the registers swap, and with a temporary the
 * two loads come in the other order. A pointer local, a struct, inline accessors on either side
 * and one-pass blocks measured 2 or 6. Func_02001b84 is the same function for speaker 8,
 * destination 0x1fb and entrance 18. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

struct Ov694EventCount {
    u8 unknown_000[0x1c4];
    u16 refusals;
};

extern s32 gSceneState[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 mode);
void Field_SetDestination(s32 scene, s32 entrance);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_0200403c(s32 value, s32 digits);
u32 Func_0200404c(void);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);
void Func_0200422c(void);

void Func_02001abc(void)
{
    u32 price = Func_0200404c() * 10;

    Func_0200406c();
    Func_02004204(0);
    if ((u32)gPartyState.coins < price) {
        Func_0200412c((s32)MsgScene694No03);
        UiText_OpenMessageAtObject(14, 0);
        return;
    }
    Func_0200412c((s32)MsgScene694No01);
    Func_0200403c(price, 5);
    UiText_OpenMessageAtObject(14, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        gSceneState[0] = gPartyState.coins;
        Func_0200422c();
        Func_02004144(14, 0);
        GameFlag_SetBit(0x109);
        GameFlag_SetBit(0x9e5);
        WaitFrames(1);
        Field_SetDestination(0x1fd, 0);
        Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 19);
    } else {
        ((struct Ov694EventCount *)Ram_HeapSlots->event_work)->refusals++;
        Func_02004144(14, 0);
    }
    Func_02004074();
}
