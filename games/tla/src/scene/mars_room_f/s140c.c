#include "room.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern const u8 gData_02005c84[];
extern const u8 gData_02005ce0[];
extern const u8 gData_02005d3c[];

void Func_02005548(s32 scene, s32 a1);
s32 Func_0200328c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

s32 Func_0200140c(void) {
  struct PartyState *party;
  s32 flag = 0xa30;

  party = &gPartyState;
  if (GameFlag_Test(flag))
    Func_02005548(party->map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  return 0;
}

void Func_02001448(void) { Func_0200328c(11, 12, 13, 0x4000, gData_02005c84); }

void Func_02001468(void) {
  Func_0200328c(19, 21, 22, 0x4000, gData_02005ce0);
  Func_020027ac(3, 131);
}

void Func_02001490(void) { Func_0200328c(14, 15, 16, 0x4000, gData_02005d3c); }
