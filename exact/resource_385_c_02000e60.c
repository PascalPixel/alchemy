#include "types.h"

/* Complete alternate paired scene-cell presentation update. */
/*
 * Both calls are named at their decoded bl sites, 0x02001f0e and 0x02001f18:
 * the semantic targets are the main-image pair Func_080091c0/Func_08009180,
 * but a direct bl cannot encode that displacement, and only the RAM-resident
 * sites reproduce the reference bytes.  The reference also materializes both
 * stacked constants before either store, which named locals preserve.
 */
extern void Func_02001f0e(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_02001f18(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);

void Func_02000e60(void)
{
    s32 first_value = 13;
    s32 second_value = 25;

    Func_02001f0e(37, 43, 1, 1, first_value, second_value);
    Func_02001f18(36, 42, 12, 22, 3, 3);
}
