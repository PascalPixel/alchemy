/* The title scene's driver. Entrance 10 reveals the title and waits up to a
 * minute for a key before returning to it; entrance 9 plays the ending's
 * scroll and returns to the clear screen; entrance 2 runs the title menu until
 * a saved game is chosen; any other entrance starts a new game in Lunpa's
 * village, Haidia. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "RESOURCE_IDS.H"
#include "TITLE.H"

extern u32 gKeyState;
extern u32 gKeysHeld;

void Event_SetPairWork1c0(s32 scene, s32 entrance);
void RuntimeDispatch_NoOpHook(s32 resource);
void Blend_SetDarkenTarget16(s32 target);
void Blend_WaitForTransition(void);
s32 SaveState_ScanRecordFlags(void);
void Party_ApplyStatePreset(void);
void Title_ShowSplashScreen(s32 mode);
void ScrollFar_Entry00(s32 mode);
s32 PaletteFar_Entry00(s32 mode);
void PaletteFar_Entry20(s32 mode);

s32 Title_Run(void)
{
    s32 wait;

    if (gGameState.entrance == 10) {
        Engine_ActorGet(gGameState.selected_actor)->motion_flags = 0;
        Engine_AudioPlayCue(75);
        Title_RevealScreen(0);
        Engine_TaskWait(120);
        wait = 0;
        if (gKeyState == 0) {
            do {
                Engine_TaskWait(1);
                if (++wait > 3599)
                    break;
            } while (gKeyState == 0);
        }
        Event_SetPairWork1c0((s32)&SceneId_Title, 2);
        return 0;
    }
    if (gGameState.entrance == 9) {
        Engine_AudioPlayCue(67);
        ScrollFar_Entry00(0);
        Engine_AudioPlayCue(17);
        Blend_SetDarkenTarget16(60);
        Blend_WaitForTransition();
        Engine_EventWait(240);
        Engine_AudioPlayCue(19);
        Event_SetPairWork1c0((s32)&SceneId_Clear, 2);
        return 0;
    }
    RuntimeDispatch_NoOpHook((s32)&ResourceId_PaletteFarCalls);
    if (gGameState.entrance == 2) {
    menu:
        Engine_AudioPlayCue(19);
        Title_ShowSplashScreen(0);
        PaletteFar_Entry20(0);
        if (SaveState_ScanRecordFlags() <= 0)
            goto chosen;
        Engine_AudioPlayCue(70);
        if (PaletteFar_Entry00(1) != 0)
            goto chosen;
        Engine_AudioPlayCue(17);
        Blend_SetDarkenTarget16(30);
        Blend_WaitForTransition();
        wait = 0;
        if (gKeysHeld == 0) {
            do {
                Engine_TaskWait(1);
                if (++wait > 119)
                    break;
            } while (gKeysHeld == 0);
        }
        goto menu;
    chosen:
        Event_SetPairWork1c0((s32)&SceneId_Clear, 1);
    } else {
        Engine_AudioPlayCue(64);
        PaletteFar_Entry00(0);
        Party_ApplyStatePreset();
        Event_SetPairWork1c0((s32)&SceneId_HaidiaMura, 16);
        Engine_AudioPlayCue(17);
    }
    Engine_AudioPlayCue(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    Engine_EventWait(60);
    Engine_AudioPlayCue(19);
    return 0;
}
