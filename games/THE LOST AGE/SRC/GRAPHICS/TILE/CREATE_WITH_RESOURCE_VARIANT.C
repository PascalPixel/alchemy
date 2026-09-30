#include "TYPES.H"
#include "RESOURCE.H"

extern s32 VramBlock_LoadCached();
extern s32 RenderOutput_CreateFar();
extern u8 UiIcon_ResourceTiles[];

s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third);

s32 VramBlock_LoadCached(s32 entry_no, s32 mode, s32 data);
s32 RenderOutput_CreateFar(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
s32 UiIcon_CreateWithResourceVariant(s32 first, s32 second, s32 third)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
  icon = 0;
  icon = 0;
  slot = Resource_FindFreeEntry();
  resource_mode = 0x80;
  if (slot != 0)
  {
    VramBlock_LoadCached(slot, copy_mode = resource_mode, UiIcon_ResourceTiles);
    icon = RenderOutput_CreateFar(slot, 0x40000000, first, second, third);
  }
  return icon;
}

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct UiIconObject { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

void UiIcon_PrepareObject(void *object);
