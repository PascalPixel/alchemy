#include "room.h"

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

void Func_02000414(s32 id);
void Func_02000798(void);
void Func_020019c4(void);
void Func_020046b4(void);
void Func_02004718(void);
void Func_02004bf8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004d58(s32 a0, s32 a1);
void Func_02004d60(s32 frames);
void Func_02004dc8(void);
void Func_02004dd0(void);
void BattleFx_PrepareBufferInterpolation(void);

void Func_02000c84(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party[0x2c2] = 2;
  }
  Func_02004d30(12);
  Battle_WaitMode0(40);
  Func_02004718();
}

void Func_02000cb4(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party[0x2c2] = 2;
  }
  Func_02004d30(13);
  Battle_WaitMode0(40);
  Func_02004718();
}

void Func_02000ce4(s32 unused, s32 id) {
  if (id == 8) {
    struct ObjectRuntime *object = Object_GetById(8);

    if (object->x >> 20 == 5) {
      Battle_WaitMode0(5);
      object->z += 0x10000;
      Flag_Set(0xa4e);
    }
  }
}
