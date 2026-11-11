#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"

struct AirsRockInsideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct AirsRockInsideSpawn {
  s32 mode;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

s32 Random16Far(void);
void Func_020002f4(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
                   struct AirsRockInsideSpawn *spawn);

void Func_02002b58(s32 x, s32 z) {
  struct AirsRockInsideSpawn spawn;
  struct AirsRockInsideVector position;
  s32 i;

  spawn.kind = 0x2a0;
  spawn.mode = 1;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = 0x30000;
  // A goto loop: the ROM rebuilds the height and the position's address on
  // every pass, which a for loop hoists (differ 55).
  i = 0;
next:
  position.x = x + ((Random16Far() & 31) << 16) - 0x100000;
  position.y = 0x40000;
  position.z = z;
  Func_020002f4(position.x, 0, position.z, 0, position.y, 0, 0x1a0000, &spawn);
  FieldEvent_Wait(5);
  i++;
  if (i != 12)
    goto next;
}
