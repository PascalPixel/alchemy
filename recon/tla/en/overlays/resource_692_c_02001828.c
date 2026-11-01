// Draft, differ 353 of 496, skeleton gap 131 (tla-en, 2026-11-01)
// The control flow, calls, memory accesses and constants are the native's; what
// is left in the skeleton is register allocation and the scheduling it moves.
// GCSE's PRE and the copy propagation after it fold every later sp+80 (the
// position array) into the first one, which then holds a callee-saved register
// from start to end, where the native recomputes `add rX, sp, #80` in the spawn
// loop (every pass) and again at the end; PRE also hoists i << 18 out of the
// spawn loop, which the native computes inside it. Both are full or partial
// redundancies of an expression over sfp or i, so no spelling reaches them: a
// pointer local, a block-scoped pointer, a separate base variable, reordered
// sums and a reordered declaration list all compile to the same thing.
// With those two registers taken, srcX is spilled (the native keeps it in r9)
// and baseX gets one (the native spills it): the allocator counts baseX's read
// three times at loop depth 2, which ranks it above srcX and open; the native's
// choice fits refs of 6 to 7 on baseX, dstX and dstZ instead. Native registers:
// leader r10, open r11, srcX r9, srcZ then i r8, j and the fade counter r6.
// A for loop for the rows beats goto on the skeleton (131 against 139), though
// not on differ (353 against 343); the spawn and fade loops stay goto, and a
// for loop for the fade costs more on both. Direct calls in place of Call3, Call6
// and Value1 cost 12 to 30 of gap; Call1 makes no difference. Compiling this
// draft with -fno-gcse did not bring its opcodes closer.
//
// A Magma Rock room's opening event: the leader is centred on its cell, sound
// 584, the screen shakes (sound 107); by room (2, 3 or 5, the last by entrance)
// it picks the cells to open, the flag that was set (768 to 770, cleared here)
// and where the effects play; sound 181 with the shaking, the cells are copied
// open; if that flag was set, ten rows of six effects rise along the opening
// while it widens and the blend fades over 64 steps; then sound 288 and 181,
// the cells close again, sound 584, and the leader steps back a cell.
#include "types.h"
#include "io_reg.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "call.h"

extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room5;
extern const s32 gData_0200502c[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004d18(void);
void Func_02004e90(s32 value);
void Func_02004f30(s32 sound);
void Func_02004d10(s32 a, s32 b, s32 c);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);
void Func_02004cd8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);

void Func_02001828(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 facing = (leader->angle + 0x2000) & 0xc000;
  s32 position[3];
  struct EffectOptions options;
  s32 baseX = 0;
  s32 dstX = 0;
  s32 dstZ = 0;
  s32 open = 0;
  s32 srcX = 0;
  s32 srcZ = 0;
  u8 *flags;
  u32 i;
  u32 j;

  /* FAKEMATCH(wrapper): the calls that repeat a constant, the flag calls and
   * the cell copies go through call.h's forwarding inlines, so that each call
   * builds its own constants and stack arguments; as direct calls, 438 of 496
   * instructions differ in tla-en. */
  Func_02004d40();
  Func_02004e90(0);
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, position[0] >> 16,
                            position[2] >> 16);
  FieldEvent_Wait(20);
  Func_02004f30(584);
  flags = &leader->priorityFlags;
  *flags &= ~64;
  FieldEvent_Wait(20);
  Call3((void (*)())Func_02004d10, 0x10000, 0x20000, 0x10000);
  Func_02004f30(107);
  FieldEvent_Wait(30);
  // Each test reads the scene again: the native reloads gMainRoutineWork for
  // room 3's test and reuses that load for room 5's.
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2) {
    dstZ = 26;
    srcX = 20;
    dstX = 84;
    baseX = 160 << 17;
    position[1] = 0;
    position[2] = 212 << 17;
    srcZ = 89;
    if (Value1((s32 (*)())GameFlag_Test, 768))
      open = 212;
    Call1((void (*)())GameFlag_ClearBit, 768);
    facing = 0;
    Func_02000694(0, 15, 13, 64);
    Actor_SetSpritePriority(15, 3);
    for (i = 0; i != 7; i++)
      Actor_SetModeById(i + 9, 1);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    dstX = 92;
    baseX = 224 << 17;
    dstZ = 21;
    position[1] = 0;
    position[2] = 172 << 17;
    srcX = 28;
    srcZ = 83;
    if (Value1((s32 (*)())GameFlag_Test, 769))
      open = 212;
    Call1((void (*)())GameFlag_ClearBit, 769);
    Func_02000694(0, 24, 15, 64);
    Func_02000694(0, 9, 16, 64);
    Actor_SetModeById(11, 4);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room5) {
    // The native tests and clears flag 770 in both arms.
    if (gMainRoutineWork.entrance <= 8) {
      dstZ = 13;
      srcX = 49;
      dstX = 113;
      baseX = 198 << 18;
      position[1] = 0;
      position[2] = 216 << 16;
      srcZ = 75;
      if (Value1((s32 (*)())GameFlag_Test, 770))
        open = 212;
      Call1((void (*)())GameFlag_ClearBit, 770);
    } else {
      dstX = 115;
      baseX = 206 << 18;
      dstZ = 42;
      position[1] = 0;
      position[2] = 170 << 18;
      srcX = 51;
      srcZ = 104;
      if (Value1((s32 (*)())GameFlag_Test, 770))
        open = 212;
      Call1((void (*)())GameFlag_ClearBit, 770);
    }
  }
  Func_02004f30(181);
  Call3((void (*)())Func_02004d10, 0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02004d10, -1, -1, 0xe666);
  Call6((void (*)())Func_02004cd8, srcX, srcZ, srcX, srcZ - 1, 3, 4);
  if (open == 0) {
    Func_02004d18();
    FieldEvent_Wait(20);
  } else {
    Call3((void (*)())Func_02004d10, 0x20000, 0x10000, 0x10000);
    options.type = 403;
    options.priority = 2;
    options.palette = 10;
    options.script = gData_0200502c;
    // The spawn loop is written with goto: the native shows none of the loop
    // pass work in it (i << 18 is not strength-reduced, nothing is hoisted).
    for (i = 0; i != 10; i++) {
    j = 0;
  spawn:
    position[0] = baseX + ((Random16Far() % 48) << 16);
    Func_0200015c(position[0], position[1], position[2] + (i << 18) + 0x170000,
                  0, 0x10000, 0, 0x330000, &options);
    j++;
    if (j != 6)
      goto spawn;
    wait(3);
    Call6((void (*)())Func_02004cd8, 75, 64, dstX, dstZ, 3, (i >> 1) + 1);
    }
    // The fade loop is written with goto: the native builds both register
    // addresses and loads 16 and 0x3f44 from the literal pool on every pass,
    // where the loop pass hoists all four out of a for loop; as a for loop,
    // 369 of 496 instructions differ.
    i = 0;
  fade:
    REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
    REG_BLDCNT = 0x3f44;
    wait(3);
    i++;
    if (i != 64)
      goto fade;
    Call6((void (*)())Func_02004cd8, 78, 64, dstX, dstZ, 3, 5);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2)
    Actor_GetById(15)->priorityFlags |= 1;
  Func_02004f30(288);
  Func_02004f30(181);
  Call3((void (*)())Func_02004d10, 0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02004d10, -1, -1, 0xe666);
  Call6((void (*)())Func_02004cd8, 72, 63, srcX, srcZ - 1, 3, 4);
  Func_02004d18();
  Func_02004f30(584);
  *flags |= 64;
  FieldEvent_Wait(20);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  {
    /* FAKEMATCH(temporary): the position's address held in a pointer, so
     * that the two halfword reads are taken from it as the native does
     * (movs r2, #2; ldrsh r1, [r5, r2]); as position[] and
     * ((s16 *)position)[1], 349 of 496 instructions differ in tla-en. */
    s32 *pos = position;

    pos[0] = (leader->x & 0xfff00000) + 0x80000;
    pos[1] = leader->y;
    pos[2] = (leader->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffsetFar(0x100000, facing ^ 0x8000, pos);
    leader->angle = facing;
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, ((s16 *)pos)[1],
                              ((s16 *)pos)[5]);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Func_02004d48();
}
