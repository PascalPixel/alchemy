#include "types.h"
#include "resource.h"

struct MenuSprite {
  struct MenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

extern const u8 gData_0805c5c4[];
void Oam_AddSpriteAtDepth(s32 *entry, s32 slot);

void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot) {
  const void *data = gData_0805c5c4;
  s32 value = Resource_FindFreeEntry();

  *slot = value;
  object->oam.f.tile = VramCache_Load(value, 0x80, data);
  object->oam.f.mode = 0;
  object->oam.f.mosaic = 0;
  object->oam.f.colors = 1;
  object->oam.f.affine = 0;
  object->oam.f.affineIndex = 0;
  object->oam.f.size = 0;
  object->oam.f.shape = 2;
  object->oam.f.priority = 0;
}
