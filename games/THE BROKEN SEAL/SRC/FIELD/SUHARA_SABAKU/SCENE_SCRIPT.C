/* The desert's scene script: it opens the screen, starts the leader's
 * progress task when flag byte 0x210 is set, keeps the encounter palette's
 * level in the first two areas and blends the visit or altar flags there;
 * the third area plays cue 0x120. */
#include "SABAKU.H"

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    u8 *map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};

extern struct FieldGlobals gMapWork;

s32 SuharaSabaku_RunSceneScript(void)
{
    u8 *map;

    map = gMapWork.map;
    gMapWork.event->start_transition = 0x201;
    if (GameFlag_GetByte(0x210) != 0) {
        gGameState.movement_mode = 2;
        Task_AddCallback(SuharaSabaku_SyncSelectedActorProgress, 0xc80);
    }
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1
        || gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        gSuharaSabakuShownLevel = EncounterPalette;
        FieldScene_RunOpeningAuxiliarySequence();
    }
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1) {
        SuharaSabaku_ApplyVisitFlagBlend();
    } else if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        SuharaSabaku_ApplyAltarFlagBlend();
    } else {
        Audio_PlayCue(0x120);
    }
    if (gGameState.entrance == 0) {
        *(u16 *)(map + 20) &= ~0x200;
    }
    return 0;
}
