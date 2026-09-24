#include "TYPES.H"
#include "FIELD_EVENT.H"

#define GetOrbitingSceneObject Func_02001af4
#define AllocateEffectTransfer Func_02001aec
#define UpdateOrbitingSceneObject Value_02008c4d

#include "STAGED_ACTOR.H"

typedef struct { s32 lo, hi; } Pair;

typedef struct { s32 w0, w1, w2, w3; Pair tail; } Query;

struct Particle_02000c4c {
    u8 unknown_00[8];
    s32 x;                  /* +0x08 */
    s32 y;                  /* +0x0c */
    u8 unknown_10[0x20];
    s32 angle;              /* +0x30, 0x10000 to the turn */
    u8 unknown_34[4];
    s32 base_x;             /* +0x38 */
    s32 base_y;             /* +0x3c */
    u8 unknown_40[0x10];
    u16 *sprite;            /* +0x50 */
};

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

extern u8 Value_02008c4d;

void Func_02001026(Query result);
OrbitingSceneObject *Func_02001af4(void);
u8 *Func_02001aec(s32, s32);

/*
 * The Func_ symbols declared above name the pre-relocation call words the
 * overlay image holds, not runtime addresses. A source reached from two
 * sites carries two such names.
 */

/*
 * One symbol per call site, named at the site's PC-relative decoded
 * address. All three reach the same ARM-mode IWRAM helper that scales one
 * channel by the adjustment, and each still needs its own name.
 */

/* Constant getter; the owner includes its own pool word. */

u8 *SceneData_GetTable8f80(void) { return (u8 *)0x02008f80; }

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable8fe0(void) { return (u8 *)0x02008fe0; }

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable8ff0(void) { return (u8 *)0x02008ff0; }

/* Runs the six-word placement query and forwards a successful result. */
void SceneActor_RunPlacementQuery(void)
{
    Query result;
    Event_Begin();
    if (StagedActor_FindClearPosition(&result))
        Func_02001026(result);
    Event_End();
}

/* Scene setup for slot 11 and effect 181. */
void FieldScene_SetupActor11Effect181(void)
{
    Event_Begin();
    Actor_SetPosition(11, 0, 0);
    GameFlag_Set(0xfd3);
    Item_ShowFound(181, 3);
    Party_GiveItem(181, 0);
    Event_End();
}

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable9068(void) { return (u8 *)0x02009068; }

/*
 * The overlay's entry driver: the loader enters here through the header
 * veneer, and no call inside the image reaches it. Sets the workspace +448
 * word to 516, sets up records 8, 9 and 10, and runs two steps for slot 11
 * that each act only while their flag is clear. The 84-byte owner includes
 * its three pool words.
 */
s32 FieldScene_SetupEntryActors8To11(void)
{
    void SceneEffect_AdjustPaletteWindow(s32 id);
    void Func_02000cb4(s32 id);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0xfd3) == 0) {
        SceneEffect_InitOrbitingParticle(11);
    }
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    if (GameFlag_IsSet(0x845) == 0) {
        SceneEffect_AdjustPaletteWindow(11);
    }
    return 0;
}

/* Applies the adjustment to the protected palette window only. */
void SceneEffect_AdjustPaletteWindow(s32 adj)
{
    volatile u16 *pal = (volatile u16 *)0x05000000;
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
    KorimaPalette_Capture(); KorimaPalette_SaveSecond(); ColorBuffer_ApplyTarget(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and
 * blue fall. Control jumps over a mask literal inside the span and rejoins
 * before the common return.
 */
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

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}

s32 SceneEffect_UpdateOrbitingParticle(struct Particle_02000c4c *record)
{
    u16 *sprite = record->sprite;
    s32 lift;
    s32 tilt;
    s32 jitter;

    lift = Math_Sin(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Math_Cos(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Math_Cos(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Random_Next() << 9) >> 16);
    jitter += (s32)(((u32)Random_Next() << 9) >> 16);
    record->angle += jitter + 1024;

    return 0;
}

void SceneEffect_InitOrbitingParticle(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(181);
    transfer += 0x400;
    Vram_Load(sprite->pal, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
