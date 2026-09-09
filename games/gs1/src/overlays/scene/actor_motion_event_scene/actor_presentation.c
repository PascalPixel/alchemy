#include "types.h"

#define ActorPresentation_SetupActorSceneCells Func_02000c1c
#define ActorPresentation_RunActorModeOneThenZero Func_020004b4
#define ActorPresentation_RunActor13AcceptanceDialogue Func_020007d4
#define ActorPresentation_SetupActorEighteenAt312_304 Func_02000af0
#define ActorPresentation_SetupActorZeroForSceneEight Func_02000b40
#define ActorPresentation_SetupActorZeroForSceneNine Func_02000b9c
#define ActorPresentation_SetupActorZeroForSceneTwelve Func_02000cc8
#define ActorPresentation_SetFlag200AndSceneCell23 Func_02000d4c
#define ActorPresentation_SetPairedSceneCells Func_02000e30
#define ActorPresentation_SetAlternatePairedSceneCells Func_02000e60
#define FieldScene_RunActorEighteenDialogue Func_02000e90

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct Presentation_02000c1c {
    u8 unk_00[9];
    u8 flags;
};

struct SceneActor_02000c1c {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation_02000c1c *presentation;
};

/*
 * Complete actor-18 dialogue/restoration scene.  If cue 231 remains available
 * and its movement scene has not set flag 0x858, the shared scene marker at
 * +370 is enabled before the dialogue scene closes.
 */
struct SceneWork_02000e90 {
    u8 reserved000[370];
    u16 actor18_marker;
};

/*
 * Every call is named at its decoded bl site: the two Scene_GetRecord reads go
 * through two DIFFERENT per-site import veneers, and the dialogue and
 * Func_020004b4 calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */
extern void Func_02001942(s32 dialogue);                 /* Func_0808a170 */
extern struct Actor_020007d4 *Func_020018e0(s32 actor);  /* Scene_GetRecord */
extern void Func_02000ca0(s32 actor);                    /* Func_020004b4 */
extern struct Actor_020007d4 *Func_020018f2(s32 actor);  /* Scene_GetRecord */
/* Complete scene-variant six actor-zero setup wrapper through its pool. */
extern const u8 Data_02009756[];
extern struct SceneWork_02000e90 *Data_03001ebc;

/* Complete actor-mode scene wrapper through its return and alignment. */
extern void Func_020015a2(void);
void Func_020015fa(s32 actor, s32 mode);
void Func_02001642(s32 actor, s32 mode);
void Func_020015be(void);
void Func_02001ca6(s32 value);
void Func_02001b88(const void *tbl, s32 value, s32 actor);
void Func_0200158e(s32 x, s32 y, s32 value);
void Func_0200160e(s32, s32, s32);
void Func_02001be4(const void *, s32, s32);
void Func_02001c0a(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001c48(s32);
void Func_02001d02(s32);
void Func_0200166a(s32, s32, s32);
void Func_02001c40(const void *, s32, s32);
void Func_02001c66(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001ca4(s32);
void Func_02001d5e(s32);
void Func_02001794(s32, s32, s32);
void Func_02001d6c(const void *, s32, s32);
void Func_02001d92(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001dd0(s32);
void Func_02001e8a(s32);
struct SceneActor *Func_02001d28(s32);
void Func_02001de4(s32);
void Func_02001cd6(s32, s32, s32, s32, s32, s32);
void Func_02001ce6(s32, s32, s32, s32, s32, s32);
void Func_02001d3c(s32);
void Func_02001cfc(s32, s32, s32, s32, s32, s32);
void Func_02001d0c(s32, s32, s32, s32, s32, s32);
void Func_02001d62(s32);
void Func_02001d30(s32, s32, s32, s32, s32, s32);
void Func_02001738(s32, s32, s32);
void Func_02001e26(s32 flag);
void Func_02001e02(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
/* Complete paired scene-cell presentation update. */
extern void Func_02001ede(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 v1, s32 v2);
void Func_02001ee8(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);
/* Complete alternate paired scene-cell presentation update. */

/*
 * Both calls are named at their decoded bl sites, 0x02001f0e and 0x02001f18:
 * the semantic targets are the main-image pair Func_080091c0/Func_08009180,
 * but a direct bl cannot encode that displacement, and only the RAM-resident
 * sites reproduce the reference bytes.  The reference also materializes both
 * stacked constants before either store, which named locals preserve.
 */
extern void Func_02001f0e(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 v1, s32 v2);
void Func_02001f18(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);
void Func_02001f7c(void);
void Func_02002002(s32 dialogue);
void Func_02001fda(s32 actor, s32 mode);
void Func_02002004(s32 actor, s32 mode, s32 value);
void Func_02001f92(s32 frames);
void Func_02002032(s32 actor, s32 mode);
void Func_02001ffa(s32 actor, s32 mode);
s32 Func_02001f80(s32 cue);
s32 Func_02001f96(s32 flag);
void Func_02001fdc(void);

void ActorPresentation_RunActorModeOneThenZero(s32 actor)
{
    Func_020015a2();
    Func_020015fa(actor, 1);
    Func_02001642(actor, 0);
    Func_020015be();
}

void ActorPresentation_RunActor13AcceptanceDialogue(void)
{
    Func_02001942(0x1cbf);
    Func_020018e0(13)->accepted = 1;
    Func_02000ca0(13);
    Func_020018f2(13)->accepted = 0;
}

void ActorPresentation_SetupActorEighteenAt312_304(void)
{
    Func_02001ca6(158);
    Func_02001b88(Data_02009756, 50, 18);
    Func_0200158e(312, 304, 6);
}

void ActorPresentation_SetupActorZeroForSceneEight(void)
{
    struct SceneActor *actor = Func_02001c48(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001d02(158);
    Func_02001be4((const void *)0x02009782, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Func_02001c0a(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200160e(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNine(void)
{
    struct SceneActor *actor = Func_02001ca4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001d5e(158);
    Func_02001c40((const void *)0x02009798, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Func_02001c66(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200166a(296, 176, 9);
}

void ActorPresentation_SetupActorSceneCells(void)
{
    struct SceneActor_02000c1c *actor = Func_02001d28(0);
    struct Presentation_02000c1c *record = actor->presentation;
    u8 flags;

    Func_02001de4(188);
    Func_02001cd6(42, 33, 34, 16, 2, 2);
    Func_02001ce6(42, 35, 36, 16, 2, 2);
    Func_02001d3c(4);
    Func_02001cfc(40, 33, 34, 16, 2, 2);
    Func_02001d0c(40, 35, 36, 16, 2, 2);
    Func_02001d62(4);
    {
        s32 mode = 3;
        s32 value = 16;
        Func_02001d30(33, 21, 2, 2, mode, value);
    }
    actor->state_23 &= ~1;
    flags = record->flags;
    flags |= 12;
    record->flags = flags;
    Func_02001738(64, 272, 11);
}

void ActorPresentation_SetupActorZeroForSceneTwelve(void)
{
    struct SceneActor_02000c1c *actor = Func_02001dd0(0);
    struct Presentation_02000c1c *presentation = actor->presentation;
    u8 flags;

    Func_02001e8a(158);
    Func_02001d6c((const void *)0x020097c4, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Func_02001d92(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001794(72, 160, 12);
}

void ActorPresentation_SetFlag200AndSceneCell23(void)
{
    Func_02001e26(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Func_02001e02(23, 23, 4, 2, first_value, second_value);
    }
}

void ActorPresentation_SetPairedSceneCells(void)
{
    s32 v1 = 13;
    s32 v2 = 25;

    Func_02001ede(41, 43, 1, 1, v1, v2);
    Func_02001ee8(40, 42, 12, 22, 3, 3);
}

void ActorPresentation_SetAlternatePairedSceneCells(void)
{
    s32 v1 = 13;
    s32 v2 = 25;

    Func_02001f0e(37, 43, 1, 1, v1, v2);
    Func_02001f18(36, 42, 12, 22, 3, 3);
}

void FieldScene_RunActorEighteenDialogue(void)
{
    Func_02001f7c();
    Func_02002002(0x1342);
    Func_02001fda(18, 0);
    Func_02002004(18, 0, 0);
    Func_02001f92(2);
    Func_02002032(18, 0);
    Func_02001ffa(18, 1);

    if (Func_02001f80(231) != -1 && Func_02001f96(0x858) == 0) {
        Data_03001ebc->actor18_marker = 1;
    }

    Func_02001fdc();
}
