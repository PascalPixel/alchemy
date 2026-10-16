// Draft of overlay 655's Func_02000038, differ 2 of 51 (was 25), the
// representative of ten copies (661, 662, 664, 668, 66a, 671, 67f, 68f, 6a0,
// 6aa): for each three-halfword record up to an id of 0xffff, clear the actor's
// collision bit 0, reset its child flags and pass its mode, flag and layer to
// Func_02000128. The records are halfwords (a struct of three is padded to 8
// bytes). The loop is a goto loop with the first test on the parameter: as a
// for or while loop the 0xffff limit is kept in r10 (differ 35). The memory
// barrier at the loop head reloads the id the test read (GCSE shares it, differ
// 21); the bottom test takes snap_scale.c's limit device (the native starts
// 0xffff, loads the id, then finishes the limit), with the limit pinned to r2
// so the id takes r3. Left: the entry test, where the native also loads the id
// between movs r2, #255 and lsls r2, r2, #8; the same asm there holds the
// frame's sub sp ahead of it (differ 8), so the limit is set in a one-pass
// block, which lets sub sp sink but leaves the shift first.
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer);

void Func_02000038(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct ObjectRuntime *object;
  s16 mode;
  s16 flag;
  register s32 limit asm("r2");
  s32 first;
  do {
    limit = 0xff;
  } while (0);
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  asm("" : : : "memory");

  id = spec[0];
  object = Object_GetById(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_02000128(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  asm volatile("" : "+l"(limit));
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}
