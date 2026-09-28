/*
 * Draft: overlay 38f (KORIMA_MURA) at 0x02008284, between ACTOR_LINES.C
 * and EXIT_WALK.C; its rows stay in the listing.
 *
 * Remaining difference: the game compares the scene number against a pool
 * word (0x27); the constant compiles to an immediate compare.
 */

s32 SceneData_SelectTableB3b0BySelector(void)
{
    if (gGameState.scene == 0x27) {
        return (s32)KorimaMura_Scene27Scripts;
    }
    return (s32)KorimaMura_Scripts;
}
