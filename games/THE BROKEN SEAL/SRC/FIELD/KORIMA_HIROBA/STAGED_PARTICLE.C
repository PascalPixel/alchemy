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
#include "FIELD_EVENT.H"

#include "STAGED_ACTOR.H"
#include "IO_REG.H"
#ifdef MODERN
// called before its definition below, which modern GCC refuses for an
// implicit int declaration of a u16 function
u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj);
#endif

enum {
    ITEM_NUT = 181
};

typedef struct { s32 lo, hi; } Pair;

typedef struct { s32 w0, w1, w2, w3; Pair tail; } Query;

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 pal;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

u8 *SceneData_GetScriptTable(void) { return KorimaHiroba_Scripts; }

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void) { return KorimaHiroba_Messages; }

u8 *SceneData_GetActorTable(void) { return KorimaHiroba_Actors; }

void SceneActor_RunPlacementQuery(void)
{
    struct StagedActorProbe result;
    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&result))
        SceneActor_MoveAndRedraw(result);
    Engine_EventEnd();
}

void KorimaHiroba_AwardHiddenNut(void)
{
    Engine_EventBegin();
    Engine_ActorSetPosition(11, 0, 0);
    Engine_GameFlagSet(0xfd3);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
}

u8 *SceneData_GetExtraTable(void) { return KorimaHiroba_Extras; }

s32 FieldScene_SetupEntryActors8To11(void)
{
    void SceneEffect_AdjustPaletteWindow(s32 id);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (Engine_GameFlagIsSet(0xfd3) == 0) {
        SceneEffect_InitOrbitingParticle(11);
    }
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    if (Engine_GameFlagIsSet(0x845) == 0) {
        SceneEffect_AdjustPaletteWindow(11);
    }
    return 0;
}

void SceneEffect_AdjustPaletteWindow(s32 adj)
{
    volatile u16 *pal = BG_PLTT;
    u32 phase;
    u32 next;
    KorimaPalette_SaveFirst();
    phase = 0;
    do {
        u32 idx = phase >> 16;
        u32 win;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            win = (idx + 0xff3f) << 16;
            if (win > 0x70000)
                pal[idx] = SceneEffect_AdjustColorChannels(pal[idx], adj);
        }
        next = phase + 0x10000;
        phase = next;
    } while (next <= 0x00df0000);
    KorimaPalette_Capture(); KorimaPalette_SaveSecond(); Engine_ColorBufferApplyTarget(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_Divide(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Math_Divide(green, adj));
    blue = (s16)(blue - Math_Divide(blue, adj));

    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
