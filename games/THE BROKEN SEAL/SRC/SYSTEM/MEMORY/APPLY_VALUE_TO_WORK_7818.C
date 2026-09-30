#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 IwramClearWords[];
extern u8 gBattleFxWork[];

/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
u32 _call_via_r3(s32, s32, u32, s32);
/*
 * val reaches the call before it is assigned, so the argument carries
 * whatever the register already holds; the trailing store keeps its place.
 */
void Runtime_ApplyValueToWork7818(u32 arg2)
{
  unsigned long val;
  s32 base;
  base = *((s32 *)((u32)&gBattleFxWork));
  _call_via_r3(base + 0x7818, 8, val, (u32)IwramClearWords);
  val = arg2;
}
