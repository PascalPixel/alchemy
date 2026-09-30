#include "TYPES.H"
#include "RESOURCE.H"

void VramBlock_LoadResourceFar(s32 entry_no, s32 mode, s32 resource_id);
extern char ResourceId_UiIconTiles;
s32 RenderOutput_CreateFar(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);

/* Loads the icon tiles resource into a free entry and creates an icon
   output from it; ☀️ passes the tiles' address where ⚓️ passes the resource. */
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
    VramBlock_LoadResourceFar(slot, copy_mode = resource_mode, (s32)&ResourceId_UiIconTiles);
    icon = RenderOutput_CreateFar(slot, 0x40000000, first, second, third);
  }
  return icon;
}
