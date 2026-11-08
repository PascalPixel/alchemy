#include "types.h"
#include "sprite.h"
#include "ram_buffer.h"

/// Takes the first free animation entry of the 64 and clears it for use. With
/// none free it clears the entry at address 0.
struct AnimationEntry *AnimationObject_AllocateEmpty(void) {
  struct AnimationEntry *entry = Ram_HeapSlots->unk0c[0];
  struct AnimationEntry *found = NULL;
  s32 i = 0;
  /* FAKEMATCH: the script field's zero held from the start lives longest, so
   * the word zero takes r3 and the byte zero r2; as plain NULLs, 24
   * instructions differ in tla-en. */
  void *none = NULL;

  while (i <= 63) {
    if (entry->kind == 0) {
      found = entry;
      break;
    }
    i++;
    entry++;
  }
  found->animId = 0;
  found->scripts = NULL;
  found->pictures = NULL;
  found->script = none;
  found->mode = 0;
  found->picture = 0;
  found->pos = 0;
  found->kind = 1;
  found->param = 0;
  return found;
}
