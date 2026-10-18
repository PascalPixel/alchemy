// Draft, differ 10 of 69, instruction order only: spawns object 14 four
// pixels below this one (cue 623 unless flag bit 0 is set), starts its
// dispatch from gData_0802ec88, gives its sprite this sprite's priority,
// and in mode 2 applies child values 13 and 8. No ☀️ twin found. Left: the
// native stores the copied word before setting up the dispatch call's
// arguments, and loads the new sprite's byte 9 before its parent's.
#include "types.h"
#include "render_input.h"

struct SpawnSprite {
  u8 unk00[4];
  struct RenderOam oam;
  u8 unk0c[14];
  u8 unk1a;
};

struct SpawnObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  u8 unk18[56];
  struct SpawnSprite *sprite;
  u8 kind;
  u8 unk55;
  u8 unk56[16];
  u16 flags;
};

extern const s32 gData_0802ec88[];

struct SpawnObject *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Audio_PlayCue(s32 cue);
void ObjectDispatch_Initialize(struct SpawnObject *, const s32 *);
s32 Animation_ApplyChildArgument(struct SpawnSprite *, s32);
void Animation_ApplyChildValue(struct SpawnSprite *, s32);
void Animation_ApplyChildValuesToRecord(struct SpawnSprite *, u32);

void Func_08026fc8(struct SpawnObject *object, s32 mode) {
  struct SpawnObject *spawn;
  struct SpawnSprite *parentSprite;
  struct SpawnSprite *sprite;

  spawn = Func_08023220(14, object->x, object->y + 0x40000, object->z);
  parentSprite = object->sprite;
  if (!(object->flags & 1))
    Audio_PlayCue(623);
  if (spawn == 0)
    return;
  sprite = spawn->sprite;
  spawn->unk14 = object->unk14;
  ObjectDispatch_Initialize(spawn, gData_0802ec88);
  spawn->unk55 = 0;
  if (sprite == 0)
    return;
  Animation_ApplyChildArgument(sprite, 1);
  sprite->unk1a = 0;
  sprite->oam.priority = parentSprite->oam.priority;
  if (mode == 2) {
    Animation_ApplyChildValuesToRecord(sprite, 13);
    Animation_ApplyChildValue(sprite, 8);
  }
}
