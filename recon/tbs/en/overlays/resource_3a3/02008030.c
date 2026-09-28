/* NONMATCHING: resource_3a3 0x02008030, SceneData_SelectTableByWord224, from
 * FIELD/ARUTIN_MURA/MOTION_PARTICLE.C (2026-09-28).
 * The game compares gGameState.scene with scenes 0x4b and 0x4c loaded from
 * literals (ldr r3, =0x4b; cmp r2, r3), so the scene ids were link-time values.
 * A plain constant compiles to cmp r2, #0x4b. Remaining: name the scene ids
 * without an equate; Value_0000004b/4c and the Data_ tables are the retired
 * bindings' spellings, kept so the difference stays visible. */
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

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009120;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_02009288;
    }
    return (s32)Data_020090f0;
}
