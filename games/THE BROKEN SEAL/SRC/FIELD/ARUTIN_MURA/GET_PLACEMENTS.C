#include "ARUTIN.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 gArutinMuraPlacements1[];
extern u8 gArutinMuraPlacements2[];
extern u8 gArutinMuraPlacementsOther[];

void FieldScene_PrepareActors(void *placements);

/* The actors placed in each of Altin's two areas, patched in place by the
   story flags: the overlay image is writable. The second area's table is
   prepared before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 id = gGameState.scene;
    if (id == (s32)&SceneId_ArutinMura1) {
        if (GameFlag_IsSet(0x909)) {
            gArutinMuraPlacements1[142] = 0;
            gArutinMuraPlacements1[166] = 0;
        }
        return (const struct ScenePlacement *)gArutinMuraPlacements1;
    }
    if (id == (s32)&SceneId_ArutinMura2) {
        if (GameFlag_IsSet(0x8fd))
            gArutinMuraPlacements2[46] = 1;
        if (GameFlag_IsSet(0x8fe) || GameFlag_IsSet(0x907))
            gArutinMuraPlacements2[94] = 1;
        FieldScene_PrepareActors(gArutinMuraPlacements2);
        return (const struct ScenePlacement *)gArutinMuraPlacements2;
    }
    return (const struct ScenePlacement *)gArutinMuraPlacementsOther;
}
