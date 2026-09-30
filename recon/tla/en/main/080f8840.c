/*
 * Draft: UiIcon_CreateWithResourceVariant does not yet match. The reference
 * loads the tile resource 0x1fa from its literal pool, as a linked symbol;
 * a plain constant is built with movs/lsls instead. Needs the resource name.
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "RESOURCE.H"

s32 VramBlock_LoadCached(s32 entry_no, s32 mode, s32 resource);
s32 RenderOutput_CreateFar(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);

/* Loads the icon tiles, resource 0x1fa, into a free entry and creates an icon
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
    VramBlock_LoadCached(slot, copy_mode = resource_mode, 0x1fa);
    icon = RenderOutput_CreateFar(slot, 0x40000000, first, second, third);
  }
  return icon;
}
