/*
 * Draft: SceneState_ForwardByRuntimeSelector does not yet match; it does not compile against ⚓️'s headers yet.
 * Links as recon/tla/raw/080de060.s.
 */
#include "YAMA.H"

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */

void SceneState_ForwardByRuntimeSelector(s32 arg)
{
    extern s32 Data_03001e40;

    s32 sel = Data_03001e40 & 7;

    if (sel == 0) {
        Object_SetPalette(arg, 2);
    } else if (sel == 2) {
        Object_SetPalette(arg, 0);
    }
}
