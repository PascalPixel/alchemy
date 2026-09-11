#include "types.h"
#include "scene.h"
#include "global_cells.h"

/*
 * Sys_SetMode names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */

/*
 * val reaches the call before it is assigned, so the argument carries
 * whatever the register already holds; the trailing store keeps its place.
 */
void Runtime_ApplyValueToWork7818(u32 arg2)
{
  unsigned long val;
  s32 base;
  base = *((s32 *)ADDR_03001EEC);
  Sys_SetMode(base + 0x7818, 8, val, 0x03000164);
  val = arg2;
}
