#include "local.h"
#include "io_reg.h"

struct OrbitSprite {
  u8 unk00[5];
  u8 flags05Low : 5;
  u8 flags05Bit5 : 1;
  u8 flags05High : 2;
  u8 unk06[3];
  u8 flags09Low : 2;
  u8 flags09Mode : 2;
  u8 flags09High : 4;
  u8 unk0a[6];
  u8 palette;
  u8 unk11[10];
  u8 state;
};

struct OrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[12];
  s32 orbitAngle;
  u8 unk34[4];
  s32 orbitCenterX;
  s32 orbitCenterY;
  s32 orbitCenterZ;
  u8 unk44[12];
  struct OrbitSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 state;
  u8 unk57[5];
  u8 active;
  u8 unk5d[4];
  u8 visible;
  u8 unk62[10];
  u32 callback;
};

void Func_02002180(s32 a, s32 b, s32 c, s32 d);
void Func_02002058(s32 item);
void Func_02002170(void);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_020007d8(void);
s32 Func_020007fc(s32 *p);

static __inline__ void Settlements_Func_02002180(s32 a, s32 b, s32 c, s32 d) {
  Func_02002180(a, b, c, d);
}

void Func_02000860(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  u8 *transfer;

  {
    register u16 alpha __asm__("r3") = 0xc08;

    do {
    } while (0);
    REG_BLDALPHA = alpha;
  }
  REG_BLDCNT = 0x3f10;
  {
    s32 priority = 0x480;

    Task_Add(Func_020007d8, priority);
  }
  Func_02002170();
  Func_02002170();
  Settlements_Func_02002180(1, 0x203, 8, 9);
  if (GameFlag_Test(0xf5d)) {
    Func_020020c0(11, 0, 0);
    return;
  }
  actor = (struct OrbitObject *)Object_GetById(11);
  sprite = actor->sprite;
  sprite->flags09Mode = 1;
  sprite->flags05Bit5 = 0;
  sprite->flags09High = 0;
  sprite->state = 0;
  ObjectDispatch_SetSingleChildFlags((struct ObjectRuntime *)actor, 0);
  actor->active = 0;
  actor->mode = 0;
  if (GameFlag_Test(0x109) == 0)
    actor->y += 0x100000;
  actor->flags23 &= 0xfe;
  actor->visible = 1;
  transfer = HeapSlot_AllocFar(68, 0x608);
  Func_02002058(193);
  transfer += 0x400;
  VramCache_Load(sprite->palette, 128, transfer);
  HeapSlot_Release(68);
  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Func_020007fc;
  actor->state = 0;
}
