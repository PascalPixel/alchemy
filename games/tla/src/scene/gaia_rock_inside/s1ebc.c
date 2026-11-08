// Leaving Gaia Rock: after flag 0x927 a message; otherwise sets the next
// destinations (scene 0xb1, entrance 10, and 11 or 12 by the item 0x1c3 and
// the four flags 0x9bc to 0x9bf) and leaves. The destination setters take s32:
// as u16 the scene symbol loads through ldrh and the pools break up.
#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

extern char SceneId_Ov681Room1;

s32 GameFlag_Test(s32 flag);
void FieldMessage_Show(s32 message, s32 position);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
s32 PartyInventory_FindOwner(s32 item);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
s32 Func_02004f74(s32 entrance, s32 count);

void Func_02001ebc(void) {
  s32 count;

  if (GameFlag_Test(0x927)) {
    FieldMessage_Show((s32)MsgGaiaRockInside08, 1);
  } else {
    s32 a = GameFlag_Test(0x9bc);
    s32 b = GameFlag_Test(0x9bd);
    s32 c = GameFlag_Test(0x9be);
    s32 scene;

    count = a + b + c + GameFlag_Test(0x9bf);
    /* FAKEMATCH(barrier): the count is summed before the battle rule store;
     * without it, 8 of 68 instructions differ in tla-en. */
    __asm__ volatile("");
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(barrier): the scene address is loaded after the battle rule
     * store; without it, 4 of 68 instructions differ in tla-en. */
    __asm__ volatile("");
    scene = (s32)&SceneId_Ov681Room1;
    Party_SetFields1eeAnd1f0(scene, 10);
    if (PartyInventory_FindOwner(0x1c3) != -1) {
      if (count == 0)
        Party_SetFields1f2And1f4(scene, 12);
    } else {
      Party_SetFields1f2And1f4(scene, 11);
    }
    Func_02004f74(12, count);
  }
}
