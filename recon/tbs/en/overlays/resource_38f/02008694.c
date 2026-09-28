/*
 * Draft: overlay 38f (KORIMA_MURA) at 0x02008694, the scene initialiser,
 * between OBJECT_SPREAD_SCENE.C and EXTENDED_SEQUENCE.C; its rows stay in
 * the listing.
 *
 * Remaining difference: the game compares the scene number against pool
 * words (0x27, 0x26) where these constants compile to immediate compares.
 */

s32 FieldScene_SetupEntryBySelector(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;
    s32 x;
    s32 y;

    step = gGameState.scene;

    if (step == 0x27) {
        FieldScene_RunFlag845And847Branches();
        return 0;
    }

    if (step == 0x26) {
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
