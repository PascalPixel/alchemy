/* NONMATCHING: resource_3a3 0x0200807c, SceneData_SelectFlaggedTable, from
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

u8 *SceneData_SelectFlaggedTable(void)
{
    s32 id = gGameState.scene;
    if (id == (s32)&Value_0000004b) {
        if (GameFlag_IsSet(0x909)) {
            Data_0200940c[142] = 0;
            Data_0200940c[166] = 0;
        }
        return Data_0200940c;
    }
    if (id == (s32)&Value_0000004c) {
        if (GameFlag_IsSet(0x8fd))
            Data_020095bc[46] = 1;
        if (GameFlag_IsSet(0x8fe) || GameFlag_IsSet(0x907))
            Data_020095bc[94] = 1;
        Func_02000f9e(Data_020095bc);
        return Data_020095bc;
    }
    return Data_020093f4;
}
