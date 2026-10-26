// Draft, differ 8 of 68: leaving Gaia Rock: after flag 0x927 a message;
// otherwise sets the next destinations (scene 0xb1, entrance 10, and 11 or 12
// by the item 0x1c3 and the four flags 0x9bc to 0x9bf) and leaves. The
// destination setters take s32: as u16 the scene symbol loads through ldrh
// (pool range 60) and the pools break up (differ 19). Left: the native adds the
// fourth flag into the count before loading gPartyState; here the add follows
// the load. SceneId_Ov681Room1 (row 0xb1 of scenes.s) is not yet named there.
#include "types.h"
#include "party_state.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern char SceneId_Ov681Room1;

s32 GameFlag_Test(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
s32 PartyInventory_FindOwner(s32 item);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
s32 Func_02004f74(s32 entrance, s32 count);

void Func_02001ebc(void) {
  s32 count;

  if (GameFlag_Test(0x927)) {
    UiText_ShowPositionedMessageAndWait((s32)MsgGaiaRockInside08, 1);
  } else {
    s32 a = GameFlag_Test(0x9bc);
    s32 b = GameFlag_Test(0x9bd);
    s32 c = GameFlag_Test(0x9be);
    s32 scene;

    count = a + b + c + GameFlag_Test(0x9bf);
    gPartyState.battleRule24b = 2;
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
