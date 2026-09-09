#include "types.h"

#define SceneData_SelectTableByWord224 Func_02000130
#define SceneData_ReturnZero Func_02000160
#define SceneData_GetTableA990 Func_02000164
#define SceneData_SelectScriptByScene33AndFlag881 Func_0200016c
#define SceneData_SelectTableByWord224B Func_020001ec
extern s16 Data_02000240[];
extern u8 Value_00000033;
extern u8 Data_0200a8a0[];
extern u8 Data_0200a798[];
/* The three returned addresses are even, so they name in-image script blocks
 * rather than Thumb entry points. */
extern u8 Data_0200a9e0[];
extern u8 Data_0200aa58[];
extern u8 Data_0200aad0[];
extern u8 Data_0200adb8[];
extern u8 Data_0200ac80[];

/* Func_0200239c and Func_020023c4 are the same flag query reached from two
 * mutually exclusive arms; each names its own loader-relocated call word. */
void Func_020023c6();
s32 Func_0200239c();
s32 Func_020023c4();

/*
 * Scene-script selection for resource_399: read the scenario id from the
 * shared table, branch on it and on story flag 0x881, and return the chosen
 * in-image script block.
 */

s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_00000033) {
        return (s32)Data_0200a8a0;
    }
    return (s32)Data_0200a798;
}

/* The whole four-byte owner. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes its one pool word. */
u8 *SceneData_GetTableA990(void)
{
    return (u8 *)0x0200a990;
}

/*
 * The overlay image is writable, so the chosen block is patched in place. The
 * coordinates are written as shifts, which is how a 16.16 whole number is
 * built here. The word at +0x4c is set only on this path and never read back,
 * so its meaning is unverified.
 */
u8 *SceneData_SelectScriptByScene33AndFlag881(void)
{
    u8 *script;

    if (Data_02000240[224] == ((s32)&Value_00000033)) {
        script = Data_0200aad0;
        Func_020023c6(script);
        if (Func_0200239c(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return script;
    }

    if (Func_020023c4(0x881) != 0) {
        return Data_0200aa58;
    }
    return Data_0200a9e0;
}

s32 SceneData_SelectTableByWord224B(void)
{
    if (Data_02000240[224] == (s32)&Value_00000033) {
        return (s32)Data_0200adb8;
    }
    return (s32)Data_0200ac80;
}
