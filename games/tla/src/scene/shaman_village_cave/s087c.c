#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "version.h"
#include "scene_tables.h"
#include "event_inline.h"

void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_02001080();
void Func_02001190();
void Func_02001208();
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
    Func_080d47b4((actor), emote_, frames_);                                   \
  }

void Func_0200087c(void) {
  s32 angle = 0xa000;

  Func_02001080();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(199), -1, PIXELS(273), 0);
  Event_SetMessageId((s32)MsgHeyTheseGuys);
  Actor_SetPositionAndFacing(4, PIXELS(184), PIXELS(229), angle);
  Actor_SetPositionAndFacing(0, PIXELS(200), PIXELS(221), angle);
  {
    /* FAKEMATCH: the local starts the facing before the other arguments, as the
     * native has (one reordered row as a plain argument). */
    s32 turn = 0xc000;

    Actor_SetPositionAndFacing(18, PIXELS(218), PIXELS(320), turn);
  }
  Actor_SetModeById(4, 19);
  Actor_SetModeById(0, 39);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Motion_SetVarCbAndRefresh(21, 2);
  Event_Wait(5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 263, 30)
  Event_ShowMessageThenWait(21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 263, 30)
  FieldMessage_ShowAtActor(19, 0);
  if (FieldYesNo_Ask(1) == 0) {
    Message_CloseFinishedWindows();
    Event_ShowMessageThenWait(20, 0, 5);
    EVENT_MESSAGE++;
  } else {
    Message_CloseFinishedWindows();
    EVENT_MESSAGE++;
    Event_ShowMessageThenWait(20, 0, 5);
  }
  Motion_SetModeAndWaitAnimation(21, 4);
  Event_ShowMessageThenWait(21, 0, 5);
  Motion_SetModeAndWaitAnimation(19, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 5);
  Camera_SetSpeedParameters(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(PIXELS(199), -1, PIXELS(373), 1);
  Event_Wait(100);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 261, 1)
  Event_Wait(20);
  Func_02001190((s16)gMainRoutineWork.savedScene,
                (s16)gMainRoutineWork.savedEntrance);
#if TLA_PATCH_1
  Func_02001208(gMainRoutineWork.scene, gMainRoutineWork.entrance);
#endif
  ((struct {
     u8 unk000[0x1ac];
     s32 value;
   } *)Ram_HeapSlots->eventWork)
      ->value = 0;
}
