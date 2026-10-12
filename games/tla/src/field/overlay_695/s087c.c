#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_02001080();
void Func_020010f0();
void Func_02001138();
void Func_02001148();
void Func_02001160();
void Func_02001170();
void Func_02001190();
void Func_020011e0();
void Func_020011e8();
void Func_02001208();
void Motion_CamBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Object_SetModeById();
void UiText_OpenMessageAtObject();
void UiWork_FinalizePendingCore();

#define EVENT_MESSAGE (((struct { u8 unknown_000[0x1c4]; s16 message; } *)Ram_HeapSlots->event_work)->message)

#define OV695_EMOTE(actor, emote, frames) \
    { \
        /* FAKEMATCH: the native loads the emote and the frames before the actor at every emote of this overlay; pinned to their argument registers they are loaded first, as there. Through an inline the actor comes one row early wherever the emote takes two instructions, and an empty asm barrier at each call moves the literal pools of a long scene. */ \
        register s32 emote_ __asm__("r1") = (emote); \
        /* FAKEMATCH: the frames, pinned with the emote above. */ \
        register s32 frames_ __asm__("r2") = (frames); \
        Func_02001160((actor), emote_, frames_); \
    }

void Func_0200087c(void)
{
    s32 angle = 0xa000;

    Func_02001080();
    Call1(Func_020011e0, 0);
    Call4(Motion_CamBounds, 0xc70000, -1, 0x1110000, 0);
    Call1(Func_02001138, (s32)MsgHeyTheseGuys);
    Call4(Func_020010f0, 4, 0xb80000, 0xe50000, angle);
    Call4(Func_020010f0, 0, 0xc80000, 0xdd0000, angle);
    {
        s32 turn = 0xc000; /* FAKEMATCH: the local starts the facing before the other arguments, as the native has (one reordered row as a plain argument). */

        Call4(Func_020010f0, 18, 0xda0000, 0x1400000, turn);
    }
    Call2(Object_SetModeById, 4, 19);
    Call2(Object_SetModeById, 0, 39);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Call2(Motion_SetVarCbAndRefresh, 21, 2);
    Call1(Battle_WaitMode0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV695_EMOTE. */
    OV695_EMOTE(21, 263, 30)
    Call3(Func_02001148, 21, 0, 5);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV695_EMOTE. */
    OV695_EMOTE(19, 263, 30)
    Call2(UiText_OpenMessageAtObject, 19, 0);
    if (Value1((s32 (*)())Func_020011e8, 1) == 0) {
        UiWork_FinalizePendingCore();
        Call3(Func_02001148, 20, 0, 5);
        EVENT_MESSAGE++;
    } else {
        UiWork_FinalizePendingCore();
        EVENT_MESSAGE++;
        Call3(Func_02001148, 20, 0, 5);
    }
    Call2(Motion_SetModeAndWaitAnimation, 21, 4);
    Call3(Func_02001148, 21, 0, 5);
    Call2(Motion_SetModeAndWaitAnimation, 19, 3);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_02001148, 19, 0, 5);
    Call2(Func_02001170, 0x8000, 0x1000);
    Call4(Motion_CamBounds, 0xc70000, -1, 0x1750000, 1);
    Call1(Battle_WaitMode0, 100);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV695_EMOTE. */
    OV695_EMOTE(18, 261, 1)
    Call1(Battle_WaitMode0, 20);
    Call2(Func_02001190, (s16)gPartyState.saved_scene, (s16)gPartyState.saved_entrance);
    Call2(Func_02001208, gPartyState.scene, gPartyState.entrance);
    ((struct { u8 unknown_000[0x1ac]; s32 value; } *)Ram_HeapSlots->event_work)->value = 0;
}
