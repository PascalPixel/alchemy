// Leaving Gaia Rock: after flag 0x927 a message; otherwise sets the next
// destinations (scene 0xb1, entrance 10, and 11 or 12 by the item 0x1c3 and
// the four flags 0x9bc to 0x9bf) and leaves. The destination setters take s32:
// as u16 the scene symbol loads through ldrh and the pools break up.
#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

extern char SceneId_Ov681Room1;

s32 Func_02004f74(s32 entrance, s32 count);

void Func_02001ebc(void) {
  s32 count;

  if (Event_GetFlag(0x927)) {
    Message_ShowCentered((s32)MsgGaiaRockInside08, 1);
  } else {
    s32 a = Event_GetFlag(0x9bc);
    s32 b = Event_GetFlag(0x9bd);
    s32 c = Event_GetFlag(0x9be);
    s32 scene;

    count = a + b + c + Event_GetFlag(0x9bf);
    /* FAKEMATCH(barrier): the count is summed before the battle rule store;
     * without it, 8 of 68 instructions differ in tla-en. */
    __asm__ volatile("");
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(barrier): the scene address is loaded after the battle rule
     * store; without it, 4 of 68 instructions differ in tla-en. */
    __asm__ volatile("");
    scene = (s32)&SceneId_Ov681Room1;
    Event_SetAfterBattleDestination(scene, 10);
    if (FieldPartyInventory_FindOwner(0x1c3) != -1) {
      if (count == 0)
        Event_SetDefeatDestination(scene, 12);
    } else {
      Event_SetDefeatDestination(scene, 11);
    }
    Func_02004f74(12, count);
  }
}
