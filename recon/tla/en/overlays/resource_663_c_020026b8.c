// Draft, differ 146 of 484 (was 218): a beam of light in Air's Rock: BG0 is
// hidden while its tiles (two blocks) and its map (offset to its tiles and
// copied in 18 rows of 32 bytes) are loaded; object 12 glows (Func_0200092c)
// and sheds a spark (object type 0x2a0, Func_020008ac) every fourth frame for
// 45 frames; the camera moves to the light, the screen flashes, BG0 is shown
// and scrolled (Func_0200257c, cue 163) while the map work's beam at 0x178
// sweeps over 272 frames, then the beam flickers (Func_02000ef4) for 257
// frames and fades over 129 (cue 390); the display is put back and flag 0x87a
// set. Written with the forms that matched overlay 664's Func_02003654.
// Left: the native saves only r8 (the beam pointer) with an 8-byte stack frame,
// keeps the counters in r7, spills the first object of each spark to the
// stack and keeps the frame count's low bits in r6 as the sparks' 0; this
// draft keeps the low bits in r8 and the beam in r10. Measured: without the
// row pointer reuse 184; a counter of its own for the flicker or fade loop
// 160 to 206; the sweep through the counter 256.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "lib/call.h"

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};
struct ObjBits {
  u8 unk00[35];
  u8 flags : 8;
};
extern s32 gFrameCount;
extern char ResourceId_WhirlwindMap;
extern char ResourceId_WhirlwindTiles;
extern const u8 gData_02003d1c[];
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02003af0(void *resource, void *dest);
void Func_02003cd8(s32 cue);
struct Actor *Actor_GetById(u32 id);
void Func_0200092c(void);
void Func_020008ac(void);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
s32 Camera_FollowActor(s32 id, s32 a);
void Func_02003c30(void);
void Func_02003c20(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Task_RemoveFar(void (*func)(void));
void Task_Add(void (*func)(void), s32 priority);
void Func_020024b0(void);
void Func_0200257c(void);
void Func_02000ef4(void);
void Func_02003c58(s32 a, s32 b);
s32 Func_02003c50(s32 a, s32 b);
s32 Func_02003c60(s32 frames);
void Func_02003b98(s32 a, s32 b, s32 c);
void Func_02003ba0(void);
void Func_02003b00(void);
void Func_02003af8(void);
void Func_02003bb8(s32 hue, s32 brightness);
void GameFlag_SetBit(s32 flag);

void Func_020026b8(void) {
  u16 *buffer = Heap_Alloc(0x680);
  s32 *beam = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  u16 *cell;
  struct Actor *spark;
  u32 i;
  s32 x, z;

  Func_02003bc8();
  Func_02003c88(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles), buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  do {
    s32 control = 0x40e;

    REG_BG0CNT = control;
  } while (0);
  Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  buffer = Heap_Alloc(0x240);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  i = 0;
  cell = buffer;
offset:
  *cell += 0xff40;
  i++;
  cell++;
  if (i != 0x120)
    goto offset;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x600200f + (i << 6)), 0x84000008,
          (volatile u32 *)0x040000d4);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Func_02003cd8(246);
  ((u8 *)Actor_GetById(12))[98] = 9;
  ((u8 *)Actor_GetById(12))[99] = 1;
  Actor_GetById(12)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(12)->x,
                                  Actor_GetById(12)->y, Actor_GetById(12)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(12)->animation)[9] & 12;

        cell = spark->animation;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      ((struct ObjBits *)spark)->flags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Call2((void (*)())Func_02003c20, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x980000, -1, 0x900000, 1);
  Func_02003c30();
  Task_RemoveFar(Func_020024b0);
  REG_BG1CNT &= ~3;
  REG_BG1CNT = REG_BG1CNT;
  Call2((void (*)())Func_02003c58, 0x10000, 0);
  Call2((void (*)())Func_02003c50, 0x7fff, 0);
  Func_02003c60(1);
  wait(1);
  Call2((void (*)())Func_02003c58, 0x7fff, 0);
  Call2((void (*)())Func_02003c50, 0x10000, 0);
  Func_02003c60(8);
  Task_Add(Func_0200257c, 0xc80);
  Func_02003cd8(163);
  REG_DISPCNT |= 0x100;
  Call3((void (*)())Func_02003b98, 0x30000, 0x30000, 0x10000);
  z = 0xffff8000;
  x = 0x4000;
sweep:
  beam[6] = x;
  beam[7] = z;
  ((u8 *)Actor_GetById(12))[98] = Random16Far() & 7;
  wait(1);
  x += 0x400;
  z += 0xfffffc00;
  if (x != 0x48000)
    goto sweep;
  Task_RemoveFar(Func_0200257c);
  Task_Add(Func_02000ef4, 0xc80);
  REG_BLDALPHA = 8;
  REG_BLDCNT = 0x3f42;
  i = 0;
flicker:
  REG_BLDALPHA = (8 - (i >> 5)) | 0x1000;
  ((u8 *)Actor_GetById(12))[98] = Random16Far() & 7;
  FieldEvent_Wait(1);
  i++;
  if (i != 257)
    goto flicker;
  REG_DISPCNT &= ~0x200;
  wait(30);
  Call1((void (*)())Func_02003cd8, 390);
  REG_BLDALPHA = 0x10;
  REG_BLDCNT = 0x3f41;
  Call3((void (*)())Func_02003b98, -1, -1, 0xe666);
  ((u8 *)Actor_GetById(12))[98] = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 3) << 8) | (0x10 - (i >> 3));
  i++;
  FieldEvent_Wait(1);
  if (i != 129)
    goto fade;
  Task_RemoveFar(Func_02000ef4);
  ((u8 *)Actor_GetById(12))[99] = 0;
  wait(30);
  Actor_GetById(12)->update = 0;
  Actor_SetPartAttribute(Actor_GetById(12), 6);
  Func_02003ba0();
  FieldEvent_Wait(60);
  REG_BLDCNT = 0;
  Func_02003b00();
  Func_02003af8();
  Func_02003bb8(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(12)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  GameFlag_SetBit(0x87a);
  Func_02003bd0();
}
