#define OWN_AudioCommand_PlayFar
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "FIELDRUN.H"
#include "GAME_STATE.H"

#define SCENE_RECORD(id) (&Field_SceneTable[id])

extern void AudioCommand_PlayFar(s16 arg0);

void Scene_ResetFlagsOnEnter(s32 unused, s32 keep)
{
    s32 group = SCENE_RECORD(gGameState.scene)->group;
    s32 flag;

    if (keep == 0) {
        for (flag = 0x200; flag <= 0x2ff; flag++)
            GameFlag_ClearBitFar(flag);
        if (group != gGameState.scene_group) {
            for (flag = 0x300; flag <= 0x3ff; flag++)
                GameFlag_ClearBitFar(flag);
            GameFlag_SetBitFar(0x12f);
            gGameState.encounter_steps = 0;
            gGameState.hazard_steps = 0;
            GameFlag_ClearBitFar(0x110);
            GameFlag_ClearBitFar(0x111);
            GameFlag_ClearBitFar(0x112);
            GameFlag_ClearBitFar(0x113);
            gGameState.retreat_scene = gGameState.scene;
            gGameState.retreat_entrance = gGameState.entrance;
        }
        for (flag = 0x80; flag <= 0xdf; flag++)
            GameFlag_ClearBitFar(flag);
        GameFlag_ClearBitFar(0x16c);
        GameFlag_ClearBitFar(0x144);
        GameFlag_ClearBitFar(0x161);
        GameFlag_ClearBitFar(0x123);
        GameFlag_ClearBitFar(0x11c);
        gGameState.return_cue = -1;
    }
    gGameState.scene_group = group;
    GameFlag_SetBitFar((group & 0x7f) + 0x180);
    gGameState.scene_variant = SCENE_RECORD(gGameState.scene)->variant;
    if (gGameState.scene_variant == 2)
        GameFlag_SetBitFar(0x123);
    GameFlag_RefreshLureCapFar();
}

void Audio_PlayCueFromEventWork(void)
{
    AudioCommand_PlayFar(gGameState.cue);
}
