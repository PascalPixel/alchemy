#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FieldScene_RunScene38b_020008f0(void);
void FieldScene_RunScene38bSequenceA(void);
void FieldScene_RunScene38b_02000d10(void);
void BiribinoMura_UpdateCornerSpawn(void);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Bilibin's scene start: fade in from the backdrop and run the scene's own
   opening; the third scene also schedules its task. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (gGameState.scene == (s32)&SceneId_BiribinoMura1) {
        FieldScene_RunScene38b_020008f0();
    } else {
        if (gGameState.scene == (s32)&SceneId_BiribinoMura3) {
            FieldScene_RunScene38bSequenceA();
            Call2((void (*)())Engine_TaskAddCallback, (s32)BiribinoMura_UpdateCornerSpawn, 0xc80);
        } else {
            if (gGameState.scene == (s32)&SceneId_BiribinoMura2) {
                FieldScene_RunScene38b_02000d10();
            }
        }
    }
    return 0;
}
