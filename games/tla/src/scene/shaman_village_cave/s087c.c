#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"

void FieldEvent_WaitFrames();
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
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Object_SetModeById();
void UiText_OpenMessageAtObject();
void UiWork_FinalizePendingCore();

#define EVENT_MESSAGE                                                          \
  (((struct {                                                                  \
      u8 unk000[0x1c4];                                                        \
      s16 message;                                                             \
    } *)Ram_HeapSlots->eventWork)                                              \
       ->message)

#define SHAMAN_VILLAGE_CAVE_EMOTE(actor, emote, frames)                        \
  {                                                                            \
    /* FAKEMATCH: the native loads the emote and the frames before the actor   \
     * at every */                                                             \
    /* emote of this overlay; pinned to their argument registers they are      \
     * loaded first, */                                                        \
    /* as there. Through an inline the actor comes one row early wherever the  \
     * emote takes */                                                          \
    /* two instructions, and an empty asm barrier at each call moves the       \
     * literal pools of */                                                     \
    /* a long scene. */                                                        \
    register s32 emote_ __asm__("r1") = (emote);                               \
    /* FAKEMATCH: the frames, pinned with the emote above. */                  \
    register s32 frames_ __asm__("r2") = (frames);                             \
    Func_02001160((actor), emote_, frames_);                                   \
  }

// The JA edition has its own form of this function; it stays on its listing in
// recon/tla/raw/overlays (S4).
#if (LANGUAGE != LANGUAGE_JAPANESE)
void Func_0200087c(void) {
  s32 angle = 0xa000;

  Func_02001080();
  Call1(Func_020011e0, 0);
  Call4(Camera_MoveWithinMapBounds, 0xc70000, -1, 0x1110000, 0);
  Call1(Func_02001138, (s32)MsgHeyTheseGuys);
  Call4(Func_020010f0, 4, 0xb80000, 0xe50000, angle);
  Call4(Func_020010f0, 0, 0xc80000, 0xdd0000, angle);
  {
    /* FAKEMATCH: the local starts the facing before the other arguments, as the
     * native has (one reordered row as a plain argument). */
    s32 turn = 0xc000;

    Call4(Func_020010f0, 18, 0xda0000, 0x1400000, turn);
  }
  Call2(Object_SetModeById, 4, 19);
  Call2(Object_SetModeById, 0, 39);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Motion_SetVarCbAndRefresh, 21, 2);
  Call1(FieldEvent_WaitFrames, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 263, 30)
  Call3(Func_02001148, 21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 263, 30)
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
  Call1(FieldEvent_WaitFrames, 10);
  Call3(Func_02001148, 19, 0, 5);
  Call2(Func_02001170, 0x8000, 0x1000);
  Call4(Camera_MoveWithinMapBounds, 0xc70000, -1, 0x1750000, 1);
  Call1(FieldEvent_WaitFrames, 100);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 261, 1)
  Call1(FieldEvent_WaitFrames, 20);
  Call2(Func_02001190, (s16)gPartyState.savedScene,
        (s16)gPartyState.savedEntrance);
  Call2(Func_02001208, gPartyState.scene, gPartyState.entrance);
  ((struct {
     u8 unk000[0x1ac];
     s32 value;
   } *)Ram_HeapSlots->eventWork)
      ->value = 0;
}
#endif
