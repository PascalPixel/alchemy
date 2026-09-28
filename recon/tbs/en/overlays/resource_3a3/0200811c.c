/* NONMATCHING: resource_3a3 0x0200811c, SceneData_GetPrimaryTable, from
 * FIELD/ARUTIN_MURA/MOTION_PARTICLE.C (2026-09-28).
 * Same scene-id literals as 0x02008030 (0x4b, 0x4c loaded from the pool).
 * Remaining: name the scene ids without an equate. */
#include "ARUTIN.H"

/* The retired bindings' spellings: scene ids as symbols, and the scene
   tables by their addresses. */
extern u8 Value_0000004b;
extern u8 Value_0000004c;
extern u8 Data_02009120[], Data_02009288[], Data_020090f0[];
extern u8 Data_0200940c[], Data_020095bc[], Data_020093f4[];
extern u8 Data_02009730[], Data_020099f4[], Data_02009724[];
void Func_02000f9e(void *);
void FieldScene_RunMiddleSequence(void);
void FieldScene_RunScene3a3SequenceD(void);

s32 SceneData_GetPrimaryTable(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009730;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_020099f4;
    }
    return (s32)Data_02009724;
}
