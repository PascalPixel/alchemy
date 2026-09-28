/* NONMATCHING: resource_3a3 0x02008874, SceneState_SyncProgressFlagsAndDispatch,
 * from FIELD/ARUTIN_MURA/MOTION_PARTICLE.C (2026-09-28).
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

/*
 * Mirror three progress flags into three scene flags, then let the scene id
 * at Data_02000240[224] select one of two continuations. Every call here
 * leaves through its own veneer, so the sites stay separate.
 */
s32 SceneState_SyncProgressFlagsAndDispatch(void)
{
    s16 scene;

    if (GameFlag_IsSet(0x8fd) != 0) {
        GameFlag_Set(0x240);
    }

    if (GameFlag_IsSet(0x8fe) != 0 || GameFlag_IsSet(0x907) != 0) {
        GameFlag_Set(0x241);
    }

    if (GameFlag_IsSet(0x8fe) != 0 && GameFlag_IsSet(0x907) != 0) {
        GameFlag_Set(0x242);
    }

    scene = gGameState.scene;
    if (scene == (s32)&Value_0000004b) {
        FieldScene_RunMiddleSequence();
    } else if (scene == (s32)&Value_0000004c) {
        FieldScene_RunScene3a3SequenceD();
    }

    return 0;
}
