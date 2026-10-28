// Draft, differ 114 of 360: Gaia Rock's entry setup: records the scene, starts
// the overlay's frame callback in the first room, places its people and opens
// the cells of what the story flags have moved; in the first room after the
// flags for Kushinada's rescue, the man by the entrance speaks once. Translated
// from its listing; names and structure still to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Func_02000670[];
s32 Func_02000f0c();
s32 Actor_GetById();
s32 Task_Add();
s32 Func_02000f14();
s32 GameFlag_Test();
void Actor_SetSingleChildFlags();
void Func_02000e44();
void Func_02000e8c();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Actor_StartRepeatedMotion();
void Func_02000ed4();
void FieldEvent_WaitFrames();
void Func_02000ea4();
void Func_02000eac();
void GameFlag_SetBit();
void Func_02000e7c();
void Func_02000dd0();
void Func_02000b2c();

s32 Func_020006ec(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r6, r8, r5, r10, v0, v1, r7, r2, v2, v3, v4, v5, v6, v7, v8, v9, v10, r3,
      v11, r0, r1, v12, v13, v14, v15, v16;
  s32 stack[2];

  r6 = (s32)&gAdventure;
  r8 = 0x3000000;
  r5 = 174;
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), s32, 428) = 0x204;
  r10 = 5;
  FIELD_AT(r6, u16, 608) = r5;
  FIELD_AT(r6, u16, 610) = r10;
  /* sub (s32)stack, #8 */
  v0 = Func_02000f0c();
  v1 = Actor_GetById(v0);
  FIELD_AT(v1, u8, 35) = 32 | (FIELD_AT(v1, u8, 35));
  r7 = 0;
  r2 = FIELD_AT(r6, s16, 480);
  if ((FIELD_AT(r6, s16, 480)) == r5)
    goto L_02008744;
  goto L_0200893c;
L_02008744:
  Task_Add((s32)Func_02000670, 0x480);
  Call2((void (*)())Func_02000f14, 8, 1);
  Call2((void (*)())Func_02000f14, 9, 1);
  Call2((void (*)())Func_02000f14, 10, 1);
  Call2((void (*)())Func_02000f14, 11, 1);
  Call2((void (*)())Func_02000f14, 12, 1);
  goto L_02008788;
L_02008788:
  Call2((void (*)())Func_02000f14, 13, 1);
  Call2((void (*)())Func_02000f14, 14, 1);
  Call2((void (*)())Func_02000f14, 15, 1);
  v2 = GameFlag_Test(0x920);
  if (v2 != 0) {
    v3 = Actor_GetById(16);
    r5 = v3;
    FIELD_AT(r5, u8, 85) = r7;
    FIELD_AT(r5, s32, 8) = 0x580000;
    FIELD_AT(r5, s32, 12) = 0x800000;
    Call2((void (*)())Actor_SetSingleChildFlags, v3, 0);
    Call6((void (*)())Func_02000e44, 6, 27, 1, 1, r10, 27);
  }
  v4 = Actor_GetById(19);
  r5 = v4;
  FIELD_AT(r5, u8, 35) = r7;
  v5 = GameFlag_Test(0x921);
  if (v5 == 0)
    goto L_02008840;
  FIELD_AT(r5, u8, 85) = r7;
  FIELD_AT(r5, s32, 8) = 0x2880000;
  FIELD_AT(r5, s32, 12) = 0x400000;
  Call2((void (*)())Actor_SetSingleChildFlags, r5, 0);
  Call6((void (*)())Func_02000e44, 33, 102, 1, 2, 37, 102);
  Call6((void (*)())Func_02000e44, 27, 44, 1, 1, 40, 46);
  goto L_0200888a;
L_02008840:
  v6 = GameFlag_Test(0x109);
  if (v6 == 0)
    goto L_0200888a;
  if (((FIELD_AT(r5, s32, 8)) >> 19) != 75) {
    Call6((void (*)())Func_02000e44, 33, 102, 1, 2, 37, 102);
  }
  if (((FIELD_AT(r5, s32, 8)) >> 19) != 79)
    goto L_0200888a;
  FIELD_AT(r5, u8, 85) = r7;
  Call6((void (*)())Func_02000e44, 38, 46, 1, 1, 39, 46);
L_0200888a:
  v7 = GameFlag_Test(0x927);
  if (v7 != 0)
    goto L_0200889a;
  goto L_02008a36;
L_0200889a:
  v8 = GameFlag_Test(0x9ab);
  if (v8 == 0)
    goto L_020088aa;
  goto L_02008a36;
L_020088aa:
  Call3((void (*)())Func_02000e8c, 20, 0x31e0000, 0x3200000);
  r5 = 49;
  Call6((void (*)())Func_02000e44, 53, 52, 1, 1, r5, r5);
  Call6((void (*)())Func_02000e44, 53, 52, 1, 1, 50, r5);
  Call2((void (*)())Actor_SetModeById, 20, 5);
  v9 = GameFlag_Test(0xa61);
  if (v9 == 0)
    goto L_020088f2;
  goto L_02008a36;
L_020088f2:
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2((void (*)())Actor_StartRepeatedMotion, 20, 3);
  Call2((void (*)())Func_02000ed4, 20, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Func_02000ea4((s32)MsgIMNot);
  Call2((void (*)())Func_02000eac, 20, 0);
  Call2((void (*)())Actor_StartRepeatedMotion, 20, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call1((void (*)())GameFlag_SetBit, 0xa61);
  Func_02000e7c();
  goto L_02008a36;
L_0200893c:
  r3 = 175;
  if (r2 != (175))
    goto L_020089e8;
  Call2((void (*)())Func_02000f14, 8, 1);
  Call2((void (*)())Func_02000f14, 9, 1);
  Call2((void (*)())Func_02000f14, 10, 1);
  Call2((void (*)())Func_02000f14, 11, 1);
  Call2((void (*)())Func_02000f14, 12, 1);
  Call2((void (*)())Func_02000f14, 13, 1);
  v11 = Func_02000f14(14, 1);
  r0 = v11;
  r1 = 0x1e2;
  r2 = 0x10000;
  r3 = ((FIELD_AT(r6, u16, 482)) - 3) << 16;
  if ((u32)(((FIELD_AT(r6, u16, 482)) - 3) << 16) <= (u32)65536) {
    Func_02000dd0(r0);
  }
  v12 = GameFlag_Test(0x300);
  if (v12 != 0) {
    Func_02000b2c(17, 0x2fc0000, -0x1870000, 0x1a80000);
  }
  v13 = GameFlag_Test(0x301);
  if (v13 != 0) {
    Func_02000b2c(18, 0x3150000, -0xf80000, 0x1a80000);
  }
  v14 = GameFlag_Test(0x302);
  if (v14 == 0)
    goto L_02008a36;
  Func_02000b2c(19, 0x30a0000, -0x770000, 0x1a80000);
  goto L_02008a36;
L_020089e8:
  r3 = 176;
  if (r2 != (176))
    goto L_02008a36;
  v15 = Actor_GetById(64);
  r5 = v15;
  if (r5 != 0) {
    Call3((void (*)())Func_02000e8c, 64, 0, 0);
  }
  r5 = FIELD_AT(FIELD_AT(r8, s32, 108), s32, 84);
  FIELD_AT((FIELD_AT(r8, s32, 108)), s32, 276) = r5;
  v16 = Actor_GetById(8);
  r5 = v16;
  FIELD_AT(r5, u8, 85) = r7;
  Call6((void (*)())Func_02000e44, 11, 14, 1, 1, 13,
        (FIELD_AT(r5, s32, 16)) >> 20);
L_02008a36:
  /* add (s32)stack, #8 */
  return 0;
}
