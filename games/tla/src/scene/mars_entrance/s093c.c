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

void Func_0200093c(void) {
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
  Func_02004d30(14);
  Battle_WaitMode0(40);
  Func_02004718();
}

// Sends object id from column 11 to x 152, then from column 10 to x 200,
// both at z 792, through Func_02004bf8.
void Func_0200096c(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  if (object->x >> 20 == 11) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    Func_02004bf8(object, 0x980000, y, 0x3180000);
  }
  /* FAKEMATCH: a volatile read of x, the y loads ahead of each call and the
   * second call's object and constants pinned to r0, r1 and r3 give the
   * native's reload of x on both paths and its argument order; plainly GCSE
   * keeps x across the first branch and sched2 builds the constants before
   * loading y (differ 14). */
  if (*(volatile s32 *)&object->x >> 20 == 10) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    {
      register struct ObjectRuntime *self asm("r0") = object;
      register s32 x asm("r1") = 0xc80000;
      register s32 z asm("r3") = 0x3180000;

      Func_02004bf8(self, x, y, z);
    }
  }
}
