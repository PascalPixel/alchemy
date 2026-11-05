// Draft, differ 161 of 720, skeleton gap 57 (tla-en, 2026-11-01)
// A Shaman Village scene around object 14, in the idiom of Air's Rock's
// Func_02001f7c (games/tla/src/scene/airs_rock/s1f7c.c): loads the whirlwind's pictures into background
// 1, sets the leader and object 14 on their action variants, sound 246 while
// for 45 frames object 14 sheds circling, blinking objects (type 672) every
// fourth frame; then the camera to (168, 424), sound 207 with the screen
// shaking and a blend over 64 frames, the map's cells redrawn in stages
// (Func_02004844) while effects (Func_020000b8) rise and object 14 flickers,
// eight rounds of the background's halves swapped, sound 390, the blend
// cleared, the palette restored (Func_0200488c from the options' hue and
// brightness) and flag 2380 set.
//
// Left: the loops are goto loops as in that twin: the native hoists nothing
// and CSE carries REG_BLDALPHA's address and 16 past them. The -mthumb-slow-loads
// removal closed nothing here; the differences below are all allocation or
// constants. Everything from the top to the spawn now matches but for these.
// - The blocker is the effect spawn's allocation, and it looks like the
//   compiler's reference weight, not source (K1, K3: out of scope, so this
//   stops here). The native has the velocity pointer r6, 127 r8, the options
//   pointer r10 and j r9, with r4 free and a 72-byte frame. Here 127 is r6, the
//   velocity pointer r4, options r8, j r10, frame 76. The frame loop sits at
//   flow depth 2, so refs weigh 3: 127 has 9 refs across 1 call, options 12
//   across 2, and CALLER_SAVE_PROFITABLE (4 * calls < refs) sends a pseudo to r4
//   before r8; r4 then saves round the Random16Far call at sp+16. Only weight 1
//   explains the native (127 at weight 2 still takes r4), and it also orders the
//   velocity pointer above 127. A real branch entering the stage and frame loops
//   past their headers makes both irreducible (depth 0) and the spawn then
//   allocates as the native. No plain loop form gets there. Pascal's lead:
//   whether the native's flow weighs refs by loop notes, which goto loops lack,
//   where alchemy-gcc weighs CFG natural loops.
// - Constants: the shake and camera calls want each argument built alone, as
//   an inline's parameter does (The Broken Seal's event.h has MapGrid_SetShake
//   and FieldCamera_SetSpeed here). Func_02004864 and Func_020049e4 are those
//   calls. Set-once locals do nothing, cprop folds them. Through Call2 and Call3
//   this is differ 123, gap 29; S2 tags those as wrapper, so they stay out until
//   Pascal says whether that layer counts as source.
// - BG0CNT's control wants a block of its own: do { } while (0) (one-pass, kept
//   out) gives differ 157; a u16 or u32 local or a shared one does not.
//   All kept-out devices together: differ 119, gap 25.
// - The zero before byte 99 is shared with update and BLDCNT in the native. A
//   bit-field store or an s32 local gets the sharing, but moves the frame count
//   into r6 and the first Actor_GetById copy to the stack: frame 80, differ 184.
//   The tail's pointer and 1 are then swapped (r5 and r6): a work pointer set
//   earlier does not change that.
// - Stage call: 13 and 11 as width and height locals, as the call before the
//   loop has them, take r4 and r5 and lose 2.
// - Tried without gain: Vector3 by value (ldmia, frame 80, differ 293), velocity
//   as a Vector3 (neutral), velocity set later, a separate z, row and column
//   locals, a set-once count local, six forms of the tile mask.
// - Closed: the sprite's priority written in one expression with no block
//   locals fixed the reload rotation before the spawn, so the twin's cell =
//   spark->animation (temporary) is not needed.
// ResourceId_WhirlwindTiles and ResourceId_WhirlwindMap (ids 0x1be, 0x1bf,
// Field_WhirlwindTiles and Field_WhirlwindMap) need names in
// resource_directory.s on adoption: the ids load from the pool, as symbols.
// The stage loop's i - (i / 4) * 4 is the native's form; i % 4 emits a mask.
#include "types.h"
#include "call.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/field_effect.h"

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
};

extern s32 gFrameCount;
extern char ResourceId_WhirlwindTiles;
extern char ResourceId_WhirlwindMap;
extern const s32 gData_02004ae0[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void *Resource_GetTableEntry(s32 id);
void Func_0200479c(void *resource, void *destination);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Import_SoundPlay(s32 cue);
struct Actor *Actor_GetById(u32 id);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Effect_CircleAndBlink(void);
s32 Camera_FollowActor(s32 id, s32 flag);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020049f4(void);
void Func_020049e4(s32 a, s32 b);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02004864(s32 a, s32 b, s32 c);
void Func_02004844(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200484c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_0200486c(void);
void Func_020047d4(void);
void Func_020047bc(void);
void Func_0200488c(s32 hue, s32 brightness);
void Func_0200482c(void);
void GameFlag_SetBit(s32 flag);
void Func_02000458(void);
void Func_02000378(void);
void Func_02000420(void);
void Func_02000508(union FieldObject *object);

#define OBJ14_BYTE(offset) (((u8 *)Actor_GetById(14))[offset])

void Func_0200057c(void) {
  u16 *buffer;
  u16 *cell;
  struct Actor *spark;
  u32 i;
  u32 j;

  buffer = Heap_Alloc(0x680);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                buffer);
  Dma_Set(buffer, (void *)0x0600e800, 0x840000d0, REG_DMA3);
  Dma_Set(buffer + 0x1a0, (void *)0x0600ec00, 0x840000d0, REG_DMA3);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(14, 1);
  do {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  } while (0);
  REG_BG0CNT &= ~3;
  REG_BG0CNT |= 1;
  Dma_Fill16(16, (void *)0x06002000, 0x500);
  buffer = Heap_Alloc(576);
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  i = 0;
  cell = buffer;
mask: {
  s32 t = (*cell + 0xff40) & 0xfff;
  *cell = t | 0x1000;
}
  i++;
  cell++;
  if (i != 288)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x0600200f + (i << 6)), 0x84000008, REG_DMA3);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Import_SoundPlay(246);
  OBJ14_BYTE(98) = 9;
  OBJ14_BYTE(99) = 1;
  Actor_GetById(14)->update = Func_02000458;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, Actor_GetById(14)->x,
                                 Actor_GetById(14)->y, Actor_GetById(14)->z);
      ((struct SpriteAttr2High *)spark->animation)->priority =
          (((s8 *)Actor_GetById(14)->animation)[9] & 12) >> 2;
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Import_ActorSetMode((struct FieldActor *)spark, 7);
      Import_ActorSetScript((struct FieldActor *)spark, gData_02004ae0);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Effect_CircleAndBlink;
    }
    FieldEvent_Wait(1);
  }
  Call2(Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  Call2(Func_020049e4, 0x10000, 0x2000);
  Call4(Camera_MoveWithinMapBounds, PIXELS(168), -1, PIXELS(424), 1);
  Func_020049f4();
  Task_Add(Func_02000378, 0xc80);
  Import_SoundPlay(207);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Call3(Func_02004864, 0x10000, 0x10000, 0x10000);
  i = 0;
rise:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto rise;
  FieldEvent_Wait(60);
  Call3(Func_02004864, 0x30000, 0x30000, 0x10000);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f42;
  Func_02004844(2, 83, 36, 64, 16, 16);
  Func_02004844(64, 64, 2, 83, 16, 16);
  REG_BG1CNT &= ~3;
  Task_Add(Func_02000420, 0x480);
  i = 0;
swirl:
  REG_BLDALPHA = ((16 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 68)
    goto swirl;
  {
    s32 width = 13;
    s32 height = 11;

    Func_02004844(45, 90, 4, 19, width, height);
  }
  {
    s32 control = 0x602;

    REG_BG2CNT = control;
  }
  /* FAKEMATCH(one-pass): the empty block ends CSE's run here; without it, 2 of 720 instructions
   * differ in tla-en. */
  do {
  } while (0);
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0xa44;
  i = 0;
stage:
  Call6(Func_02004844, (i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19, 13, 11);
  j = 0;
frame:
  if ((j & 3) == 0) {
    s32 velocity[3];
    struct EffectOptions options;
    s32 *v = velocity;

    options.priority = 1;
    options.update = Func_02000508;
    v[0] = 0;
    v[2] = -0x20000;
    v[1] = 0;
    {
      s32 x = ((Random16Far() & 127) << 16) + (208 << 15);

      Func_020000b8(x, 0, (472 - (Random16Far() & 127)) << 16, v[0],
                    v[1], v[2], 0x1020001, &options);
    }
  }
  REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
  OBJ14_BYTE(98) = Random16Far() & 7;
  wait(1);
  j++;
  if (j != 64)
    goto frame;
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  i++;
  if (i != 8)
    goto stage;
  {
    s32 control = 0x60a;

    REG_BG2CNT = control;
  }
  REG_DISPCNT &= ~DISPCNT_BG2_ON;
  {
    s32 width = 9;
    s32 height = 11;

    Func_02004844(47, 19, 38, 19, width, height);
  }
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f42;
  i = 0;
clear:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 68)
    goto clear;
  Task_RemoveFar(Func_02000420);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Func_02004844(36, 64, 2, 83, 16, 16);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  Call3(Func_02004864, -1, -1, 0xe666);
  Import_SoundPlay(390);
  OBJ14_BYTE(98) = 6;
  i = 0;
fade:
  {
    u32 q = i >> 2;

    REG_BLDALPHA = (q << 8) | (16 - q);
  }
  FieldEvent_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  Task_RemoveFar(Func_02000378);
  {
    s32 zero = 0;

  OBJ14_BYTE(99) = zero;
  wait(30);
  Actor_GetById(14)->update = (void *)zero;
  Actor_SetPartAttribute(Actor_GetById(14), 6);
  Func_0200486c();
  FieldEvent_Wait(60);
  REG_BLDCNT = zero;
  }
  Func_020047d4();
  Func_020047bc();
  Func_0200488c(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(14)->priorityFlags |= 1;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  Func_0200482c();
  wait(1);
  Call2(Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  {
    s32 width = 6;
    s32 height = 29;

    Func_0200484c(1, 5, 9, 1, width, height);
  }
  GameFlag_SetBit(2380);
}
