// Draft, differ 172 of 496, skeleton gap 62 (tla-en, 2026-11-01)
// Re-measured after the compiler change: the frame (92), the sp offsets and the
// instruction shapes stand, and the arm statement orders below were refitted by
// moving each statement in turn (207 to 172). What is left is allocation and
// two gcse effects that no plain C here reproduces. All of 1 to 3 point the same
// way, so try a source form that does them together, not three fixes.
// Native registers: leader r10, open r11, srcX r9, i r8, j r6, options r7, p r5;
// baseX, dstX and dstZ are spilled (sp+32, 28, 24). Draft: i r10, leader r9,
// dstZ r11, open r8, and srcX is spilled instead.
// 1. baseX, dstX and dstZ outrank srcX and open by one reference. Each has 5
//    sets plus its reads in the column loops: 8 weighted references, where 7
//    would cross floor_log2 (lreg: 8 refs over 452 to 502 insns against srcX's 8
//    over 552 and open's 7 over 352). Removing their zero initialisers as a
//    probe gives skeleton gap 85 where this draft then had 101, but the native
//    stores those zeros. Exactly the three variables read inside the column
//    loops are over-weighted, and srcX and open are not, so the native's reads
//    weigh less than the CFG's depth here says (depth 2 and 1). A DImode pair
//    for dst spills them but misorders the zero stores (209).
// 2. PRE hoists i << 18 out of the column loop (the native's mov r2, r8; lsls r3,
//    r2, #18 in each pass), and that register takes r8. A tail form with the
//    reset beside the back edge (if (i != 10) { column = 0; goto spawn; }) still
//    hoists (211). The sibling Func_02001350 needs asm("" : "+r"(row)) for this.
// 3. p's sets come out as add rX, sp, #80; adds r5, rX, #0: GCSE's reaching
//    register for the frame address has a REG_EQUIV and reload rebuilds it. The
//    native has the add straight into r5. Moving p's set does nothing. The
//    sibling needs asm("" : : : "sfp") for this.
// Items 1 to 3 together look like a compiler question, which is out of scope for
// a draft (K1 to K3); the -Os lead in the round notes would explain all three,
// but that is inference and was not measured. No device was added for them.
// Devices to retire before this can be adopted:
// - j is reused for the position's address, the room 2 counter, the fade counter
//   and the address of currentOwner: a local reused for a second value. A
//   separate room pointer costs 137 (the frame grows to 96), an owner pointer
//   148, a room counter or a fade counter 8 each, both of those 58. The column
//   counter has been split off at no cost, and i also holds the source row (srcZ)
//   before it counts rows; rowAbove keeps srcZ - 1 for the closing copy (sp+16).
// - The Call3, Call6 and Value1 wrappers: direct everywhere costs 121. One
//   GameFlag_ClearBit family and the second closing Func_02004d10 went direct at
//   no cost; each other site costs 1 to 31.
// Tried without gain: for loops for the columns (202) and fade (214), the zero
// declarations moved, the options and closing stores reordered, *flags |= 64
// respelled, i * 0x40000, the operands of the sums swapped.
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
  u8 *flags;
  u32 i = 0;
  u32 j;
  u32 column;
  s32 *p;
  s32 rowAbove;

  /* FAKEMATCH(wrapper): the calls that repeat a constant, the flag tests and
   * the cell copies go through call.h's forwarding inlines, so that each call
   * builds its own constants and stack arguments; as direct calls, 293 of 496
   * instructions differ in tla-en. */
  Func_02004d40();
  Func_02004e90(0);
  // One variable for the address, the counters and the owner's address: see the
  // header. The native keeps all of them in r6.
  j = (u32)position;
  ((s32 *)j)[0] = (leader->x & 0xfff00000) + 0x80000;
  ((s32 *)j)[2] = (leader->z & 0xfff00000) + 0x80000;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, ((s32 *)j)[0] >> 16,
                            ((s32 *)j)[2] >> 16);
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
    srcX = 20;
    baseX = 160 << 17;
    ((s32 *)j)[1] = 0;
    dstZ = 26;
    i = 89;
    dstX = 84;
    ((s32 *)j)[2] = 212 << 17;
    if (Value1((s32 (*)())GameFlag_Test, 768))
      open = 212;
    GameFlag_ClearBit(768);
    facing = 0;
    Func_02000694(0, 15, 13, 64);
    Actor_SetSpritePriority(15, 3);
    for (j = 0; j != 7; j++)
      Actor_SetModeById(j + 9, 1);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    dstZ = 21;
    baseX = 224 << 17;
    dstX = 92;
    i = 83;
    srcX = 28;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 172 << 17;
    if (Value1((s32 (*)())GameFlag_Test, 769))
      open = 212;
    GameFlag_ClearBit(769);
    Func_02000694(0, 24, 15, 64);
    Func_02000694(0, 9, 16, 64);
    Actor_SetModeById(11, 4);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room5) {
    // The native tests and clears flag 770 in both arms.
    if (gMainRoutineWork.entrance <= 8) {
      dstZ = 13;
      i = 75;
      dstX = 113;
      srcX = 49;
      baseX = 198 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 216 << 16;
      if (Value1((s32 (*)())GameFlag_Test, 770))
        open = 212;
      GameFlag_ClearBit(770);
    } else {
      dstZ = 42;
      baseX = 206 << 18;
      dstX = 115;
      i = 104;
      srcX = 51;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 170 << 18;
      if (Value1((s32 (*)())GameFlag_Test, 770))
        open = 212;
      GameFlag_ClearBit(770);
    }
  }
  Func_02004f30(181);
  Call3((void (*)())Func_02004d10, 0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02004d10, -1, -1, 0xe666);
  rowAbove = i - 1;
  Call6((void (*)())Func_02004cd8, srcX, i, srcX, rowAbove, 3, 4);
  if (open == 0) {
    Func_02004d18();
    FieldEvent_Wait(20);
  } else {
    Call3((void (*)())Func_02004d10, 0x20000, 0x10000, 0x10000);
    options.type = 403;
    options.priority = 2;
    options.palette = 10;
    options.script = gData_0200502c;
    // The column loop is written with goto: the native shows none of the loop
    // pass work in it (i << 18 is not strength-reduced); as a for loop, 202 of
    // 496 instructions differ.
    for (i = 0; i != 10; i++) {
    column = 0;
  spawn:
    p = position;
    p[0] = baseX + ((Random16Far() % 48) << 16);
    Func_0200015c(p[0], p[1], p[2] + (i << 18) + 0x170000,
                  0, 0x10000, 0, 0x330000, &options);
    column++;
    if (column != 6)
      goto spawn;
    wait(3);
    Call6((void (*)())Func_02004cd8, 75, 64, dstX, dstZ, 3, (i >> 1) + 1);
    }
    // The fade loop is written with goto: the native builds both register
    // addresses and loads 16 and 0x3f44 from the literal pool on every pass,
    // where the loop pass hoists all four out of a for loop; as a for loop,
    // 214 of 496 instructions differ.
    j = 0;
  fade:
    REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
    REG_BLDCNT = 0x3f44;
    wait(3);
    j++;
    if (j != 64)
      goto fade;
    Call6((void (*)())Func_02004cd8, 78, 64, dstX, dstZ, 3, 5);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2)
    Actor_GetById(15)->priorityFlags |= 1;
  Func_02004f30(288);
  Func_02004f30(181);
  Func_02004d10(0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02004d10, -1, -1, 0xe666);
  Call6((void (*)())Func_02004cd8, 72, 63, srcX, rowAbove, 3, 4);
  Func_02004d18();
  Func_02004f30(584);
  *flags |= 64;
  FieldEvent_Wait(20);
  j = (u32)&gMainRoutineWork.currentOwner;
  Actor_GetById(*(s32 *)j)->actionFlags &= ~1;
  /* FAKEMATCH(temporary): the position's address held in a pointer, so that
   * the two halfword reads are taken from it as the native does (movs r2, #2;
   * ldrsh r1, [r5, r2]); as position[] and ((s16 *)position)[1], 220 of 496
   * instructions differ in tla-en. */
  p = position;
  p[0] = (leader->x & 0xfff00000) + 0x80000;
  p[1] = leader->y;
  p[2] = (leader->z & 0xfff00000) + 0x80000;
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, (s32)facing ^ 0x8000, (s32)p);
  leader->angle = facing;
  Actor_SetPositionAndReset(*(s32 *)j, ((s16 *)p)[1], ((s16 *)p)[5]);
  Actor_GetById(*(s32 *)j)->actionFlags |= 1;
  Func_02004d48();
}
