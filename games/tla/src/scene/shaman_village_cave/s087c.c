#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "scene_tables.h"

void FieldEvent_Wait();
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
void Actor_SetModeById();
void FieldMessage_ShowAtActor();
void Message_CloseFinishedWindows();

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

void Func_0200087c(void) {
  s32 angle = 0xa000;

  Func_02001080();
  Call1(Func_020011e0, 0);
  Call4(Camera_MoveWithinMapBounds, PIXELS(199), -1, PIXELS(273), 0);
  Call1(Func_02001138, (s32)MsgHeyTheseGuys);
  Call4(Func_020010f0, 4, PIXELS(184), PIXELS(229), angle);
  Call4(Func_020010f0, 0, PIXELS(200), PIXELS(221), angle);
  {
    /* FAKEMATCH: the local starts the facing before the other arguments, as the
     * native has (one reordered row as a plain argument). */
    s32 turn = 0xc000;

    Call4(Func_020010f0, 18, PIXELS(218), PIXELS(320), turn);
  }
  Call2(Actor_SetModeById, 4, 19);
  Call2(Actor_SetModeById, 0, 39);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Motion_SetVarCbAndRefresh, 21, 2);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 263, 30)
  Call3(Func_02001148, 21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 263, 30)
  Call2(FieldMessage_ShowAtActor, 19, 0);
  if (Value1((s32 (*)())Func_020011e8, 1) == 0) {
    Message_CloseFinishedWindows();
    Call3(Func_02001148, 20, 0, 5);
    EVENT_MESSAGE++;
  } else {
    Message_CloseFinishedWindows();
    EVENT_MESSAGE++;
    Call3(Func_02001148, 20, 0, 5);
  }
  Call2(Motion_SetModeAndWaitAnimation, 21, 4);
  Call3(Func_02001148, 21, 0, 5);
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001148, 19, 0, 5);
  Call2(Func_02001170, 0x8000, 0x1000);
  Call4(Camera_MoveWithinMapBounds, PIXELS(199), -1, PIXELS(373), 1);
  Call1(FieldEvent_Wait, 100);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 261, 1)
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02001190, (s16)gMainRoutineWork.savedScene,
        (s16)gMainRoutineWork.savedEntrance);
#if LANGUAGE != LANGUAGE_JAPANESE
  Call2(Func_02001208, gMainRoutineWork.scene, gMainRoutineWork.entrance);
#endif
  ((struct {
     u8 unk000[0x1ac];
     s32 value;
   } *)Ram_HeapSlots->eventWork)
      ->value = 0;
}
