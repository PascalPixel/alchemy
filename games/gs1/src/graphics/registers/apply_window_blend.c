#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* graphics/apply_window_blend.c */
/* graphics/registers/apply_window_blend.c */
void Graphics_ApplyWindowBlendRegisters(void)
{
    u8 *src = *(u8 **)ADDR_03001EEC;

    *(u16 *)0x04000040 = *(u16 *)(src + 0x77bc); /* WIN0H */
    *(u16 *)0x04000044 = *(u16 *)(src + 0x77be); /* WIN0V */
    *(u16 *)0x04000042 = *(u16 *)(src + 0x77c0); /* WIN1H */
    *(u16 *)0x04000046 = *(u16 *)(src + 0x77c2); /* WIN1V */
    *(u16 *)0x04000048 = *(u16 *)(src + 0x77c4); /* WININ */
    *(u16 *)0x0400004a = *(u16 *)(src + 0x77c6); /* WINOUT */
    *(u16 *)0x04000000 = *(u16 *)(src + 0x77c8); /* DISPCNT */
    *(u16 *)0x04000050 = *(u16 *)(src + 0x77ca); /* BLDCNT */
    *(u16 *)0x04000052 = *(u16 *)(src + 0x77cc); /* BLDALPHA */
}

/* graphics/registers/apply_bg2_reference.c */
void Graphics_ApplyBg2ReferencePoint(void)
{
    u8 *base = *(u8 **)ADDR_03001EEC;
    *(u32 *)0x04000028 = *(u32 *)(base + 0x77D0);
    *(u32 *)0x0400002C = *(u32 *)(base + 0x77D4);
}

/* graphics/palette/step_fade_transfer.c */
struct Globals {
    void *target;
    u8 unknown[116];
    u8 *base;
};

extern struct Globals gBattleWork;

void Palette_StepFadeTransfer(void)
{
    u8 *base = gBattleWork.base;
    s32 *remaining = (s32 *)(base + 0x77B4);
    void *target = gBattleWork.target;

    if (*remaining > 0) {
        s32 *counter = (s32 *)(base + 0x77B8);
        s32 value = ++*counter;

        Sys_SetMode(
            (u8 *)target + 0x544,
            (void *)0x050000C0,
            0x10000 - value * 1092,
            128
        );
        (*remaining)--;
    }
}

/* runtime/memory/apply_value_to_work7818.c */
/*
 * Sys_SetModeValueToWork7818 names a bx rN veneer slot, so this is an indirect call
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
  Sys_SetModeValueToWork7818(base + 0x7818, 8, val, 0x03000164);
  val = arg2;
}
