#include "TYPES.H"

#define GetOrbitingSceneObject Func_02001af4
#define NormalizeOrbitingSceneObject Func_02001ae6
#define IsGameFlagSet Func_02001b0c
#define AllocateEffectTransfer Func_02001aec
#define LoadEffectResource Func_02001b3c
#define ConfigurePaletteTransfer Func_02001b14
#define ReleaseEffectTransfer Func_02001b12
#define UpdateOrbitingSceneObject Value_02008c4d
#define SceneData_GetTable8f80 Func_020009dc
#define SceneData_ReturnZero Func_020009e4
#define SceneData_GetTable8fe0 Func_020009e8
#define SceneData_GetTable8ff0 Func_020009f0
#define SceneActor_RunPlacementQuery Func_020009f8
#define FieldScene_SetupActor11Effect181 Func_02000a2c
#define SceneData_GetTable9068 Func_02000a60
#define FieldScene_SetupEntryActors8To11 Func_02000a68
#define SceneEffect_AdjustPaletteWindow Func_02000abc
#define SceneEffect_AdjustColorChannels Func_02000b24
#define SceneEffect_UpdateOrbitingParticle Func_02000c4c
#define SceneEffect_InitOrbitingParticle Func_02000cb4

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

extern u8 *Data_03001ebc;
extern u8 Value_02008c4d;

void Func_0200181a(void);
void Func_02001846(void);
s32 Func_02000e7a(Query *result);
void Func_02001026(Query result);
void Func_0200184c();
void Func_0200184c_a();
void Func_02001878(void);
void Func_0200188e(s32, s32, s32);
void Func_020018bc(s32, s32);
s32 Func_0200187c(s32, s32);
void Func_020008c0(s32 id);
s32 Func_020018a2(s32 flagId);
void Func_0200164e();
u16 Func_02001614(u16, s32);
void Func_020016d2();
void Func_020016b6();
void Func_02001978();
s32 Func_020019fa(s32);
s32 Func_02001a10(s32);
s32 Func_02001a2a(s32);
s32 Func_02001a28(void);
s32 Func_02001a2e(void);
OrbitingSceneObject *Func_02001af4(void);
void Func_02001ae6(OrbitingSceneObject *, s32);
s32 Func_02001b0c(s32);
u8 *Func_02001aec(s32, s32);
void Func_02001b3c(s32);
s32 Func_02001b14(u8, s32, u8 *);
void Func_02001b12(s32);

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
s32 Func_020018d2();   /* 0x02000b44 */

s32 Func_020018e0();   /* 0x02000b52 */

s32 Func_020018ee();   /* 0x02000b60 */


/* Constant getter; the owner includes its own pool word. */


u8 *SceneData_GetTable8f80(void) { return (u8 *)0x02008f80; }

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
    Func_0200181a();
    if (Func_02000e7a(&result))
        Func_02001026(result);
    Func_02001846();
}

/* Scene setup for slot 11 and effect 181. */
void FieldScene_SetupActor11Effect181(void)
{
    Func_0200184c();
    Func_0200188e(11, 0, 0);
    Func_0200184c_a(0xfd3);
    Func_020018bc(181, 3);
    Func_0200187c(181, 0);
    Func_02001878();
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

    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Func_02001880(0xfd3) == 0) {
        Func_0200173a(11);
    }
    Func_0200134c(8);
    Func_02001352(9);
    Func_02001358(10);
    if (Func_020018a2(0x845) == 0) {
        Func_02001564(11);
    }
    return 0;
}

/* Applies the adjustment to the protected palette window only. */
void SceneEffect_AdjustPaletteWindow(s32 adj)
{
    volatile u16 *pal = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next;
    Func_0200164e();
    phase = 0;
    do {
        u32 idx = phase >> 16;
        u32 win;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            win = (idx + 0xff3f) << 16;
            if (win > 0x70000)
                pal[idx] = Func_02001614(pal[idx], adj);
        }
        next = phase + 0x10000;
        phase = next;
    } while (next <= 0x00df0000);
    Func_020016d2(); Func_020016b6(); Func_02001978(0x10000, 0);
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

    red = (s16)(red + Func_020018d2(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Func_020018e0(green, adj));
    blue = (s16)(blue - Func_020018ee(blue, adj));

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

    lift = Func_020019fa(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Func_02001a10(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Func_02001a2a(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Func_02001a28() << 9) >> 16);
    jitter += (s32)(((u32)Func_02001a2e() << 9) >> 16);
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
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->pal, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
