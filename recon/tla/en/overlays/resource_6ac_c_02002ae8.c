// Draft, differ 373 of 362, skeleton gap 46/51 (tla-en, 2026-11-01)
// Rewritten from the translated form into structs and plain control flow. What
// closed: position read directly (the frame address is the pseudo the listing
// keeps in r7, no pointer local), the state halfword through a member struct
// (gives movs and its address in r8), the abs-range tests with the >= 0 arm
// first, FIELD_AT on gMainRoutineWork (keeps its base and offset apart).
// Left: gcse PRE. The listing rebuilds cellX + widthB - 1, cellX + widthA - 1,
// cellX - 1 at the outer head and i << 7 at the inner head; here PRE hoists all
// four (.07.gcse, edges 6-7 and 7-8). That costs a 60-byte frame against 56,
// and the hoisted pseudos push the frame address out of r7 onto the stack, and
// the column above effect, so most of the differ is register names.
// Tried: real for loops (the slots, step order and sp+8 giv look like loop.c,
// but it reduces x << 20, z << 20 and the cell index, skeleton 56/88 with
// heads set in the loops, 77/102 inline); either loop alone real (55/65,
// 65/86); entering at the bottom test (stops PRE, adds a b, 61/61); x set
// first, x = cellX - 1 with xB = x + widthB (58/57); i * 128; step orders.
// A copy at the top, x = cellX then x - 1 (skeleton 32/32, differ 326), stops
// the outer hoist but is a forced temporary, so it is not kept; i << 7 stays
// hoisted even with it. Call3 at the -1, -1 call is the old draft's device:
// without it the two -1 share a register. The goto loops are the old draft's.
// Whether PRE ran at all on the native is a compiler question, out of scope.
#include "types.h"
#include "call.h"

struct Vec3 {
  s32 x, y, z;
};

struct Unk64 {
  s16 value;
  u8 unk02[2];
};

struct Effect {
  u8 unk00[6];
  u16 heading;
  s32 x, y, z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[4];
  s32 unk24[3];
  u8 unk30[4];
  s32 unk34;
  s32 unk38[3];
  u8 unk44[0x17];
  u8 unk5b;
  u8 unk5c[8];
  struct Unk64 unk64;
  void (*unk68)();
  s32 unk6c;
};

struct Cell {
  u8 unk00[2];
  u8 unk02;
  u8 unk03;
};

struct Work20 {
  u8 unk00[328];
  s32 unk148;
  s32 unk14c;
  u8 unk150[32];
  struct Cell *unk170;
  u8 unk174[12];
  s32 unk180;
  s32 unk184;
  u8 unk188[32];
  struct Cell *unk1a8;
};

struct Heap {
  u8 unk00[32];
  struct Work20 *field;
  u8 unk24[72];
  u8 *unk6c;
};

struct Data20 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[96];
  struct Effect *effect;
};

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define HEAP ((struct Heap *)0x3000000)

s32 Func_02002870();
s32 Func_020029f4();
s32 Func_02003140();
s32 Func_02003168();
s32 Func_02003170();
s32 Func_02003178();
s32 Func_02003198();
s32 Func_020032b8();
s32 GameFlag_Test();
struct Effect *Actor_GetById();
s32 Vector_AddPolarOffsetFar();
extern u8 gData_020023c4[];
extern u8 gMainRoutineWork[];

void Func_02002ae8(struct Effect *effect) {
  struct Vec3 position;
  s32 i, j, cellX;
  struct Cell *cellsA, *cellsB;
  s32 widthA, widthB;
  s32 zB;
  struct Work20 *field;
  struct Effect *actor;
  struct Data20 *hold;
  u8 *work;
  s32 mode;
  struct Unk64 *state;
  s32 flag;
  s32 x, z, xA, xB, zA;
  s32 cellZ, depthA;

  work = HEAP->unk6c;
  state = &effect->unk64;
  mode = state->value;
  if (mode == 0) {
    flag = GameFlag_Test(0x106);
    if (*(s16 *)(work + 358) != 0)
      flag |= 1;
    effect->unk5b = flag;
    position.x = effect->x;
    position.y = effect->y;
    position.z = effect->z;
    Call3((void (*)())Vector_AddPolarOffsetFar, 0x80000, effect->heading, &position);
    if (Func_02003168(0, position.x, position.z) == 7) {
      effect->unk38[2] = 0x80000000;
      effect->unk38[1] = 0x80000000;
      effect->unk38[0] = 0x80000000;
      effect->unk34 = 0x4000;
      state->value = 1;
      Func_020032b8(145);
      Call3((void (*)())Func_02003198, 0x50000, 0x50000, 0x10000);
      Call3((void (*)())Func_02003198, -1, -1, 0xe666);
      if (effect->unk68 != 0)
        effect->unk68(effect);
    }
    field = HEAP->field;
    cellX = effect->x >> 20;
    cellZ = effect->z >> 20;
    cellsA = field->unk170 + cellX + (cellZ << 7);
    cellsB = field->unk1a8 + cellX + (cellZ << 7);
    widthA = field->unk148 >> 20;
    depthA = field->unk14c;
    widthB = field->unk180 >> 20;
    zB = cellZ + (field->unk184 >> 20) - 2;
    i = -1;
    z = cellZ - 1;
    zA = cellZ + (depthA >> 20) - 1;
  outer:
    xB = cellX + widthB - 1;
    j = -1;
    xA = cellX + widthA - 1;
    x = cellX - 1;
  inner:
    if (cellsA[(i << 7) + j].unk02 == 77) {
      cellsA[(i << 7) + j].unk02 = 0;
      Func_02003178(64, 64, 1, 1, xA, zA);
      Func_02003170(64, 64, 1, 1, x, z);
      Func_020032b8(0x23d);
      Func_020029f4((x << 20) + 0x80000, effect->y, (z << 20) + 0x80000,
                    effect->heading);
    }
    if (cellsB[(i << 7) + j].unk02 == 77) {
      cellsB[(i << 7) + j].unk02 = 0;
      Func_02003178(64, 64, 1, 2, xB, zB);
      Func_02003170(64, 64, 1, 1, x, z);
      Func_020032b8(0x23c);
      Func_020029f4((x << 20) + 0x80000, effect->y, (z << 20) + 0x80000,
                    effect->heading);
    }
    j++;
    xB++;
    x++;
    xA++;
    if (j <= 1)
      goto inner;
    zB++;
    i++;
    z++;
    zA++;
    if (i <= 1)
      goto outer;
    if (effect->scaleX < 0x10000) {
      effect->scaleX += 0x1000;
      effect->scaleY += 0x1000;
    }
    Func_02003140(effect, position.x, position.y, position.z);
    work = HEAP->unk6c;
    hold = (struct Data20 *)(gData_020023c4 + 0x188);
    actor = Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532));
    if (effect->x - actor->x >= 0) {
      if (effect->x - actor->x >= 0x140000)
        return;
    } else if (actor->x - effect->x >= 0x140000)
      return;
    if ((u32)(effect->y - actor->y + 0x7ffff) > 0x1ffffe)
      return;
    if (effect->z - actor->z >= 0) {
      if (effect->z - actor->z >= 0x140000)
        return;
    } else if (actor->z - effect->z >= 0x140000)
      return;
    hold->unk04 = 2;
    hold->effect = effect;
    *(u16 *)(work + 340) = hold->unk0a + 2;
  } else if (mode == 1) {
    effect->unk5b = 0;
    if (effect->scaleX > 0) {
      effect->scaleX += -0x1000;
      effect->scaleY += -0x1000;
      Func_02002870(effect);
    } else {
      effect->z = 0;
      effect->y = 0;
      effect->x = 0;
      effect->unk24[2] = 0;
      effect->unk24[1] = 0;
      effect->unk24[0] = 0;
      effect->unk6c = 0;
    }
  }
}
