#include "IMIRU_FUCHIN.H"

extern s32 ImiruFuchin_EntryFlag;
extern u8 *gActorEffectWork;

void ImiruFuchin_ApplyEntrySetup(void);
void FieldScene_RunScene39aSequenceA(void);

void SceneState_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = &ImiruFuchin_EntryFlag;

    if (*flag != 0) {
        u8 *state = gActorEffectWork;

        *(s32 *)(state + 24) = (s32)Engine_ActorGet(0);
    }
}

void SceneState_ClearWorkWord24(void)
{
    if (ImiruFuchin_EntryFlag != 0) {
        *(s32 *)(gActorEffectWork + 24) = 0;
    }
}

/* The scene start around Imil: open with the window transition; the first
   area runs its opening sequence until flag 0x109 is set, and otherwise the
   areas are set up for the entrance. */
s32 ImiruFuchin_ApplyEntryHook(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x109) == 0 && gGameState.scene == (s32)&SceneId_ImiruFuchin1) {
        GameFlag_Set(0x144);
        FieldScene_RunScene39aSequenceA();
    } else {
        ImiruFuchin_ApplyEntrySetup();
    }
    return 0;
}
