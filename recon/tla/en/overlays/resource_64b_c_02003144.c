// Draft, differ 66 of 275: the party gathers at the ship after flag 0x8df
// and talks; the actors present are listed in Data_02006be8 and restored
// after. Both loops are goto loops, as the native reloads the table address
// in each pass. Left: the native keeps x and z unshifted (r8, r7) and shifts
// them at each use; this draft keeps them shifted.
#include "types.h"
#include "actor.h"

extern char MsgWeDidIt[];
extern s32 Data_02006be8[8];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_0200050c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 id, s32 x, s32 z);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 id, s32 mode);
void Func_02004d08(s32 id, s32 animation, s32 frames);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004dc0(void);
void Func_02004e30(s32 cue);

void Func_02003144(void) {
  s32 x, z, i;

  if (GameFlag_Test(0x8df) == 0)
    return;
  Func_02004c20();
  Func_02004db8(0);
  GameFlag_SetBit(0xa78);
  Actor_SetPositionAndCommit(8, 0x1bf6, 0x2154);
  Func_0200050c();
  x = Actor_GetById(8)->x / 0x10000;
  z = Actor_GetById(8)->z / 0x10000;
  i = 0;
  {
  hide:
    if (Actor_GetById(i) != NULL) {
      Data_02006be8[i] = i;
      Actor_GetById(i)->scaleX = 0;
      Actor_SetSingleChildFlags(Actor_GetById(i), 0);
      Func_02004c88(i, x << 16, z << 16);
    } else {
      Data_02006be8[i] = -1;
    }
    i++;
    if (i <= 7)
      goto hide;
  }
  Actor_GetById(23)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_GetById(23), 0);
  Func_02004c88(23, x << 16, z << 16);
  Func_02004da8();
  Func_02004cd8((s32)MsgWeDidIt);
  Func_02004e30(390);
  Func_02004cf0(2, 0);
  Func_02004d08(8, 0x102, 40);
  Func_02004cf0(1, 0);
  Func_02004cf0(3, 0);
  Func_02004cf0(0, 0);
  FieldEvent_WaitFrames(30);
  Func_02004cf0(5, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(23, 0);
  Func_02004e30(78);
  FieldEvent_WaitFrames(50);
  Func_02004cf0(6, 0);
  Func_02004cf0(5, 0);
  FieldEvent_WaitFrames(30);
  Func_02004e30(38);
  Func_02004cf0(6, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(0, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(6, 0);
  Func_02004cf0(7, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(3, 0);
  FieldEvent_WaitFrames(30);
  Func_02004cf0(2, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(1, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(1, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(1, 0);
  Func_02004cf0(5, 0);
  Func_02004cf0(3, 0);
  Func_02004cf0(1, 0);
  FieldEvent_WaitFrames(40);
  Func_02004cf0(23, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(23, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(23, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(23, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(23, 0);
  Func_02004cf0(0, 0);
  FieldEvent_WaitFrames(20);
  Func_02004cf0(23, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(23, 0);
  FieldEvent_WaitFrames(60);
  Func_02004cf0(6, 0);
  Func_02004e30(78);
  Func_02004cf0(23, 0);
  FieldEvent_WaitFrames(40);
  Func_02004cf0(23, 0);
  Func_02004dc0();
  Func_02004db0();
  i = 0;
  {
  restore:
    if (Data_02006be8[i] >= 0) {
      Actor_SetSingleChildFlags(Actor_GetById(Data_02006be8[i]), 1);
      Actor_GetById(Data_02006be8[i])->scaleX = 0x10000;
      Func_02004c88(Data_02006be8[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Func_02004c88(23, 0, 0);
  Func_02004c28();
}
