#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern struct ObjectRuntime *gData_02004420;

struct ObjectRuntime *Func_02003bb0(s32 id, s32 x, s32 y, s32 z);
void Func_02003c18(s32 id);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void VramCache_Load(s32 handle, s32 size, void *source);

void Func_02003a30(void) {
  struct ObjectRuntime **slot = &gData_02004420;
  /* FAKEMATCH: the native keeps one zero in r7 for the three byte stores below;
   * plain zeros are built in r2 where they are stored (differ 22). */
  s32 zero = 0;
  struct ObjectRuntime *object = Func_02003bb0(489, 0, 0, 0);

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->animation;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
    Func_02003c18(199);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    Runtime_ReleaseHeapBlock(68);
  }
}
