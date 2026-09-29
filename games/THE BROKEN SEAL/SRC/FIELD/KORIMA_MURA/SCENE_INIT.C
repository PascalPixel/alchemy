#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KORIMA_MURA.H"

extern u8 KorimaMura_EntryActions[];
extern u8 KorimaMura_EntryScript[];

void FieldScene_RunFlag845And847Branches(void);
void FieldScene_RunExtendedActorSequence(void);
void Event_CallWithLastActiveObjectId(u8 *script);

/*
 * Kori's scene start: the third scene runs its flag branches and the second
 * opens with the window transition. The village itself hides actors 23 to
 * 26 behind their entry actions, keeps the plaza closed until flag 0x845,
 * and once flag 0x843 is set sends the party's companions and the villagers
 * away and runs the entry script.
 */
s32 Scene_Initialize(void)
{
    s16 scene;
    u32 actor;
    u8 *tbl;
    s32 x;
    s32 y;

    scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorimaMura3) {
        FieldScene_RunFlag845And847Branches();
        return 0;
    }

    if (scene == (s32)&SceneId_KorimaMura2) {
        *(s32 *)(gWork + 0x1c0) = 0x204;
        return 0;
    }

    Actor_SetSpriteFlags(Engine_ActorGet(23), 0);
    Actor_SetSpriteFlags(Engine_ActorGet(24), 0);
    Actor_SetSpriteFlags(Engine_ActorGet(25), 0);
    Actor_SetSpriteFlags(Engine_ActorGet(26), 0);

    tbl = KorimaMura_EntryActions;
    Actor_EnableActionCallback(23, tbl);
    Actor_EnableActionCallback(24, tbl);
    Actor_EnableActionCallback(25, tbl);
    Actor_EnableActionCallback(26, tbl);

    if (GameFlag_IsSet(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Actor_SetSpriteFlags(Engine_ActorGet(actor), 0);
        }
        Map_CopyCellAttributes(13, 9, 1, 1, 13, 8);
        Map_CopyCellAttributes(13, 9, 1, 1, 15, 8);
        x = 14;
        y = 9;
        Map_CopyCellAttributes(13, 9, 1, 1, x, y);
    }

    if (GameFlag_IsSet(0x843) == 0) {
        if (gGameState.entrance == 1) {
            FieldScene_RunExtendedActorSequence();
        }
    }

    if (GameFlag_IsSet(0x843) != 0) {
        Actor_Destroy(ACTOR_GERALD);
        Actor_Destroy(ACTOR_IVAN);
        Actor_Destroy(ACTOR_MIA);
        Actor_Destroy(17);
        Actor_Destroy(18);
        Actor_Destroy(19);
        Actor_Destroy(20);
        Actor_Destroy(21);
        Actor_Destroy(22);
        Event_CallWithLastActiveObjectId(KorimaMura_EntryScript);
    }

    return 0;
}
