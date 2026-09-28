/*
 * Draft: overlay 38f (KORIMA_MURA) at 0x0200816c..0x02008230, between
 * ACTOR_FX.C and ACTOR_LINES.C; its rows stay in the listing.
 *
 * Remaining difference: the game compares the scene number against pool
 * words (0x26, 0x27, 0x24) where these constants compile to immediate
 * compares; only relocated symbols reproduce the loads. The eight-byte
 * table getter between them would match but is left with its neighbours.
 * Tables are named by role; they are not labelled in the listing.
 */

s32 SceneData_SelectTableAe60BySelector(void)
{
    s16 v = gGameState.scene;

    if (v == 0x27) {
        return (s32)KorimaMura_Scene27Actors;
    }
    if (v == 0x26) {
        return (s32)KorimaMura_Scene26Actors;
    }
    return (s32)KorimaMura_Actors;
}

s32 SceneData_GetTableB010ForSelector26(void)
{
    if (gGameState.scene == 0x26) {
        return (s32)KorimaMura_Scene26Extras;
    }
    return 0;
}

u8 *SceneData_GetTableB040(void)
{
    return KorimaMura_Messages;
}

s32 SceneData_SelectTableB080BySelector(void)
{
    s32 v = gGameState.scene;
    if (v == 0x24) {
        if (GameFlag_IsSet(0x845) == 0) {
            SceneData_InitRecordTable((s32)KorimaMura_Scene24Records);
        }
        return (s32)KorimaMura_Scene24Records;
    }
    if (v == 0x27) {
        return (s32)KorimaMura_Scene27Records;
    }
    return (s32)KorimaMura_Records;
}
