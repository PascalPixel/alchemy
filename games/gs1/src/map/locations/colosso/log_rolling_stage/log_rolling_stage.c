#include "types.h"
#include "colosso_log_rolling_stage.h"

/* map/locations/colosso/log_rolling_stage/log_rolling_stage_tail.c */
#define Colosso_PositionAndActivateActor Func_020022f4
#define Colosso_RunLogRollingInteraction Func_020024d0
#define Colosso_RestoreActorPositions Func_020025c8
#define Colosso_MarkSceneProgress Func_02002710
#define Colosso_SelectNearestObstacle Func_02002758
#define Colosso_ClearSavedActorPositions Func_02002a50
#define Colosso_RunStateInteraction Func_02002a94
#define Colosso_InitializeStateInteraction Func_02002b50
#define Colosso_ApplyItemToMatchingSlots Func_02002e18
#define Colosso_InitializeModeTask Func_020033d8
#define Colosso_RunScriptedTransition Func_02003468
#define Colosso_ResetActorMotion Func_020038dc
#define Colosso_EnsurePaletteHandle Func_020038fc
#define Colosso_StartPaletteTask Func_02003a58
#define Colosso_StartPaletteTaskFromState Func_02003abc
#define Colosso_StopPaletteTask Func_02003b18
#define Colosso_PositionScaledObject Func_02003b40
#define Colosso_SpawnPositionedObject Func_02003b80
#define Colosso_SetBalanceStateReady Func_02003cc4
#define Colosso_WaitForBalanceState Func_02003cd0
#define Colosso_SpawnRandomSceneEffect Func_02003cf4
#define Colosso_RaiseLinkedSceneEffect Func_02003d88
#define Colosso_PositionActiveActor Func_02003ddc
#define Colosso_SetupSceneDescriptor Func_02004494
#define Colosso_InitializeSceneControl Func_0200457c
#define Colosso_SetSceneControlValue Func_020045d0
#define Colosso_PushStagedActor Func_02004628
#define Colosso_FindActorAhead Func_02004790

typedef struct ActorSpriteRef {
    u8 filler00[0x28];
    s16 *entry;
} ActorSpriteRef;

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x3c];
    ActorSpriteRef *sprite;
    u8 state;
} StageActor;

typedef struct ScaledStageObject {
    u8 pad00[12];
    s32 y;                      /* +12 */
    u8 pad10[32];
    s32 scale_x;                /* +48 */
    s32 scale_z;                /* +52 */
    u8 pad38[35];
    u8 state;                   /* +91 */
} ScaledStageObject;

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 vertical_motion;
    u8 reserved_2c[41];
    u8 state;
} StageEffect;

typedef struct StageEffect_02003d88 {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[65];
    u8 state;
    u8 reserved_56[14];
    s16 linked_effect_slot;
} StageEffect_02003d88;

typedef struct SceneControl {
    s16 enabled;
    s16 active;
    s16 scene_variant;
    s16 phase;
    s16 timer;
} SceneControl;

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern void Func_02004d72(void);       /* site 0x20024e4 -> Func_0200288c veneer */
extern void Func_02006eec(void);       /* site 0x20024ea -> Func_0808a018 veneer */
extern s32 Func_02004f88(s32 actor, s32 slot); /* site 0x20024f2 -> Func_02002a94 veneer */
extern void Func_02006fb0(s32 message);  /* site 0x20024fe -> Func_0808a170 veneer (state==0 arm) */
extern void Func_02007004(s32 x, s32 z); /* site 0x200250a -> Func_0808a208 veneer */
extern void Func_0200701e(s32 x, s32 y, s32 z, s32 mode); /* site 0x200251c -> Func_0808a210 veneer */
extern void Func_0200702a(void);         /* site 0x2002520 -> Func_0808a218 veneer */
extern void Func_02006f20(s32 frames);   /* site 0x2002526 -> Func_0808a010 veneer #1 */
extern void Func_02006ff0(s32 actor, s32 mode); /* site 0x200252e -> Func_0808a180 veneer #1 */
extern void Func_02005f92(s32 x, s32 z, s32 mode); /* site 0x2002538 -> Func_02003a58 veneer */
extern void Func_02006f38(s32 frames);   /* site 0x200253e -> Func_0808a010 veneer #2 */
extern void Func_02007008(s32 actor, s32 mode); /* site 0x2002546 -> Func_0808a180 veneer #2 */
extern void Func_0200600e(s32 x, s32 z, s32 mode); /* site 0x2002550 -> Func_02003abc veneer #1 */
extern void Func_02006f50(s32 frames);   /* site 0x2002556 -> Func_0808a010 veneer #3 */
extern void Func_02007020(s32 actor, s32 mode); /* site 0x200255e -> Func_0808a180 veneer #3 */
extern void Func_02006026(s32 x, s32 z, s32 mode); /* site 0x2002568 -> Func_02003abc veneer #2 */
extern void Func_02006f68(s32 frames);   /* site 0x200256e -> Func_0808a010 veneer #4 */
extern void Func_02007038(s32 actor, s32 mode); /* site 0x2002576 -> Func_0808a180 veneer #4 */
extern void Func_02006094(void);         /* site 0x200257a -> Func_02003b18 veneer */
extern void Func_02006f7a(s32 frames);   /* site 0x2002580 -> Func_0808a010 veneer #5 */
extern void Func_0200707a(s32 actor, s32 mode); /* site 0x2002588 -> Func_0808a200 veneer */
extern void Func_020050e2(s32 actor, s32 slot); /* site 0x2002590 -> Func_02002b50 veneer */
extern void Func_0200704e(s32 message);  /* site 0x200259c -> Func_0808a170 veneer (state==1 arm) */
extern void Func_02007066(s32 actor, s32 mode); /* site 0x20025a4 -> Func_0808a180 veneer #5 */
extern s32 Func_0200515c(s32 state, s32 actor, s32 slot); /* site 0x20025ae -> Func_02002bac veneer */
extern void Func_02006fbc(void);         /* site 0x20025b2 -> Func_0808a020 veneer */
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;
extern u16 Data_0200dbd0;
extern u16 Data_0200dba4;
extern u16 Data_0200dbdc;
extern s32 Data_0200dbe0;
extern u16 Data_0200dc38;
extern u16 Data_0200dbac;
extern s32 Data_0200dbb0;
extern s32 Data_0200d9a6;   /* default handler (mode not 2/3/4) */
extern s32 Data_0200ccba;   /* handler for mode == 2 */
extern s32 Data_0200d9d2;   /* handler for mode == 4 */
extern s32 Data_0200cce2;   /* handler for mode == 3, parameter != 0 */
extern s32 Data_0200da50;   /* handler for mode == 3, parameter == 0 */
extern void Func_02007e72(void);              /* Func_0808a018 veneer #1 (mode==0 arm) */
extern void Func_02007fc6(void);              /* Func_0808a360 veneer #1 */
extern void Func_02007fda(void);              /* Func_0808a370 veneer #1 */
extern void Func_02007e78(s32 frames);        /* Func_0808a010 veneer #1 */
extern void Func_02008026(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void Func_020062e0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_0200686c(s32 style, s32 variant); /* Func_020033d8 veneer #1 */
extern void Func_02007e92(s32 frames);        /* Func_0808a010 veneer #2 */
extern void Func_02007ea6(void);              /* Func_0808a020 veneer #1 */
extern void Func_02008046(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void Func_02007eaa(void);              /* Func_0808a018 veneer #2 */
extern void Func_02007ffe(void);              /* Func_0808a360 veneer #2 */
extern void Func_02008012(void);              /* Func_0808a370 veneer #2 */
extern void Func_02007eba(s32 frames);        /* Func_0808a010 veneer #3 */
extern void Func_0200806a(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void Func_02006324(s32 mode);          /* Func_02002e54 veneer #2 */
extern void Func_020068b0(s32 style, s32 variant); /* Func_020033d8 veneer #2 */
extern void Func_02007ed6(s32 frames);        /* Func_0808a010 veneer #4 */
extern void Func_02007d26(s32 frames);        /* Func_080000c0 veneer (loop body) */
extern s32 Func_02008092(void);               /* Func_080f9048 veneer (loop check) */
extern void Func_02008094(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void Func_0200634e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_020068da(s32 style, s32 variant); /* Func_020033d8 veneer #3 */
extern void Func_020080a8(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void Func_02007f06(s32 frames);        /* Func_0808a010 veneer #5 */
extern void Func_020068ee(s32 style, s32 variant); /* Func_020033d8 veneer #4 */
extern void Func_020080bc(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void Func_02007f1a(s32 frames);        /* Func_0808a010 veneer #6 */
extern void Func_0200637c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_02006908(s32 style, s32 variant); /* Func_020033d8 veneer #5 */
extern void Func_020080d6(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void Func_02007f34(s32 frames);        /* Func_0808a010 veneer #7 */
extern void Func_02006396(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_02006922(s32 style, s32 variant); /* Func_020033d8 veneer #6 */
extern void Func_020080f0(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void Func_020080cc(void);              /* Func_0808a4f0 veneer */
extern void Func_02007f60(void);              /* Func_0808a020 veneer #2 */
extern void Func_02007f16(s32 flag);          /* GameFlag_Set veneer */
extern s16 Data_0200dace;
extern u16 Data_0200dc34;
extern u16 Data_0200dbc0;
extern u16 Data_0200db98;
extern u16 Data_0200dbb4;
extern u16 Data_0200dbcc;
extern u16 Data_0200dba0;
extern u16 Data_0200dc40;
extern u16 Data_0200dbe4;
extern u16 Data_0200dbfc;
extern u16 Data_0200db90;
extern u8 Data_0200ce50[];
extern u8 Data_0200ce74[];
extern u8 Data_0200cd80[];
extern u8 *Data_03001f3c;
extern u8 Data_0200bef1[];
extern u32 Data_0200cfc0[];

s32 Func_020070e6(s32);
StageActor *Func_02007198(s32);
void Func_02007186(void);
StageActor *Func_020071a6(s32);
void Func_020072a0(s32);
void Func_020072b8(s32, s32);
void Func_02007210_a();
void Func_02007210_b();
void Func_02007374(void);
void Func_02007380(void);
void Func_02007200(s32, s32);
void Func_0200735a(s32);
void Func_0200720a(s32);
void Func_02004e9a(s32);
void Func_020073ae(void);
void Func_020073c2(void);
void Func_02007274(void);
void Func_0200742a(s32, s32);
void Func_02007434(s32, s32);
void Func_0200743e(s32, s32);
void Func_02007448(s32, s32);
void Func_02007452(s32, s32);
void Func_0200745c(s32, s32);
void Func_0200760c(void);
void Func_0200741c(s32, s32);
void Func_0200757a(s32);
void Func_02007592(s32, s32);
s32 Func_0200748c(s32);
s32 Func_0200749c(s32);
s32 Func_0200747e(s32);
void Func_020074cc(s32);
void Func_020075ca(s32);
s32 Func_020075da(s32, s32);
s32 Func_0200753a(s32, s32);
void Func_020074d2(s32, s32);
void Func_02007632(s32);
void Func_0200764a(s32, s32);
void Func_02007c38(s32 taskAddress, s32 frameBudget);
void Func_020082d4(s32 taskAddress, s32 frameBudget);
void Func_02008330(s32 taskAddress, s32 frameBudget);
void Func_0200b91c(void);
void Func_0200836e(void (*callback)(void));
void Func_020083c8(s32 slot);
ScaledStageObject *Func_020086b0();
void Func_02008464();
void Func_0200844c();
void Func_02008488();
u8 *Func_020086f0(s32 object_id);
void Func_020084a4(void);
void Func_0200848c(u8 *object, s32 mode);
void Func_020084c8(u8 *object, s32 x, s32 y, s32 z);
void Func_020084d6(u8 *object);
void Func_020084a6(u8 *object, s32 enabled);
u32 Func_02008568(void);
s32 Func_02008584(void);
s32 Func_0200858a(void);
void Func_020085a6(s32, s32, s32 *);
StageEffect *Func_0200863a(s32, s32, s32, s32);
void Func_020086b6(StageEffect *, s32);
void Func_0200864e_a(StageEffect *, s32);
void Func_0200864e_b(StageEffect *, s32);
void Func_02008656(StageEffect *, s32);
StageEffect *Func_020087b4(s16);
void Func_020086be(StageEffect *, s32, s32, s32);
void Func_020086a6(StageEffect *, s32);
void Func_02008964(s32);
u8 *Func_02008818_b();
u8 *Func_02008d2e();
s32 Func_02008d48();
u8 *Func_02008f08();
u8 *Func_02008f10();
s32 Func_02008eb0();
void Func_02008dbe();
s32 Func_02008de2();
s32 Func_02008dee();
void Func_02008d8e();
void Func_02008ddc();
void Func_0200c0d0(void);
s32 Func_02008e56(void);
void Func_02008e26(s32, s32);
s32 Func_02008f44(s32);
void Func_02008dfa(s32, s32);
SceneRecord *Func_0200905c();
SceneRecord *Func_02008c56(Position3 *, SceneRecord *);
SceneRecord *Func_02008c80(Position3 *, SceneRecord *);
SceneRecord *Func_02008cac(Position3 *, SceneRecord *);
s32 Func_0200905e(SceneRecord *, Position3 *);
void Func_02008ffc(SceneRecord *, s32);
void Func_02008f62(s32);
void Func_02009042(SceneRecord *, s32, s32, s32);
void Func_02009052(SceneRecord *, s32, s32, s32);
void Func_020092e8(s32);
void Func_02009066(SceneRecord *);
void Func_020092f6(s32);
void Func_0200904c(SceneRecord *, s32);
u8 *Func_020091bc();
void Func_02009046();
s32 *Func_02008dc6();
void Func_02009078();
s32 *Func_02008df8();

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence) -- declared/named as the literal per-site
 * targets, not the shared ultimate-destination symbol. */

/*
 * The two rate words and state byte remain byte-offset accesses because this
 * owner needs the temporary pointer walk to preserve register allocation.
 * Its six imported calls are per-site veneers whose semantics are unproven.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

                                /* allocate/fetch a record by (id, size) */

                                /* reserve a graphics handle */

                                /* release a graphics handle */

                                /* upload image data to a handle */

                                /* upload a palette ramp */

                                /* next palette slot index */

                                /* install a per-frame task (callback, rate) */

                                /* test a story flag */

                                /* scene record for an actor selector */

/* In-image data at file offset 0x3f14 (0x0200cd80 - 0x8000). */

/* The per-frame task this owner installs; in-image code at file offset
 * 0x33a0 (0x0200c0d0 - 0x8000). */

/* Store the caller's halfword in the scene workspace field at offset 220. */

                                /* scene record for a subject handle */

                                /* select presentation mode (record, mode) */

                                /* place the record at (x, y, z) */

                                /* re-attach the camera to a record */

                                /* terrain probe; positive result blocks */

                                /* wait n frames */

                                /* play a cue */

/* This overlay's own byte-exact occupancy lookup. */

/* In-image direction table at file offset 0x4154 (0x0200cfc0 - 0x8000):
 * sixteen packed steps, high half x, low half z. */

s32 *Func_02006e64();          /* entity by selector, established */

void Func_02006c06();          /* unestablished */

void Func_02006bee();          /* established (record, mode) */

void Func_02006c2a();          /* established (record, x, y, z) */

void Func_02006c38();          /* unestablished, single argument */

void Func_02006c08();          /* established (record, mode) */

s32 Func_02006f98();           /* Func_080770e0 veneer #1 */

s32 Func_02006fa2();           /* Func_080770e0 veneer #2 */

void Func_02007058();          /* Func_0808a0f0 veneer #1 */

s32 Func_02006fc0();           /* Func_080770e0 veneer #3 */

s32 Func_02006fca();           /* Func_080770e0 veneer #4 */

void Func_0200707c();          /* Func_0808a0f0 veneer #2 */

s32 Func_02006fe4();           /* Func_080770e0 veneer #5 */

s32 Func_02006fee();           /* Func_080770e0 veneer #6 */

void Func_020070a0();          /* Func_0808a0f0 veneer #3 */

u8 *Func_020077bc();           /* veneer, item/party record by id, established */

s32 Func_020077ce();           /* veneer, established (handle, item) */

void Func_020077ea();          /* veneer, established (handle, slot) */

u8 *Func_020082f8();           /* scene-record accessor, established (veneer to Scene_GetRecord) */

void Func_020081e6();          /* unestablished */

s32 Func_0200829e(void);       /* established (veneer to Func_080153b8) */

void Func_02007362();          /* sibling item-28 owner, via per-site veneer */

void Func_02008520();          /* advance the task scheduler, established (veneer to Func_080000c0) */

s32 Func_020087a0();           /* veneer to GameFlag_IsSet */

u8 *Func_02008818_a();         /* veneer to Scene_GetRecord */

void Func_0200876a();          /* veneer to Object_SetPosition */

void Func_02008750();          /* veneer to Object_SetCallback */

void Func_020086b6_a();          /* veneer to Func_080000c0 */

s32 Func_02006ca2();           /* local thunk to Func_020020e8, site A */

s32 Func_02006cb4();           /* local thunk to Func_020020e8, site B */

void Func_0200880a();          /* veneer to UiText_DrawQuantity, site A */

void Func_0200881c();          /* veneer to UiText_DrawQuantity, site B */

void Func_0200882c_a();          /* shared veneer, selector refresh + 0x96a */

void Func_0200882c_b();          /* shared veneer, selector refresh + 0x96a */

void Func_020087ca();          /* veneer to Func_08009148 */

void Colosso_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e64(selector);
    if (record != 0) {
        Func_02006c06();
        Func_02006bee(record, 5);
        Func_02006c2a(record, x << 16, record[3], z << 16);
        Func_02006c38(record);
        Func_02006c08(record, 1);
    }
}

void Colosso_RunLogRollingInteraction(s32 actor)
{
    extern s16 Data_02000240[];

    s32 state;

    if (Data_02000240[225] == 2) {
        Func_02004d72();
        return;
    }

    Func_02006eec();
    state = Func_02004f88(actor, 6);

    if (state == 0) {
        Func_02006fb0(0x20c7);
        Func_02007004(0x30000, 0x6000);
        Func_0200701e(0x5080000, -1, 0x980000, 1);
        Func_0200702a();
        Func_02006f20(30);
        Func_02006ff0(actor, 0);
        Func_02005f92(0xb4, 0x58, 0);
        Func_02006f38(60);
        Func_02007008(actor, 0);
        Func_0200600e(0x20, 0x54, 10);
        Func_02006f50(30);
        Func_02007020(actor, 0);
        Func_02006026(0x60, 0x54, 30);
        Func_02006f68(60);
        Func_02007038(actor, 0);
        Func_02006094();
        Func_02006f7a(2);
        Func_0200707a(0, 0);
        Func_020050e2(actor, 6);
    } else if (state == 1) {
        Func_0200704e(0x20c6);
        Func_02007066(actor, 0);
    }

    Func_0200515c(state, actor, 6);
    Func_02006fbc();
}

void Colosso_RestoreActorPositions(void)
{
    s32 center;

    {
        s32 x = Func_02006f98(896);
        s32 z = Func_02006fa2(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_02007058(1, x, z);
    }
    {
        s32 x = Func_02006fc0(912);
        s32 z = Func_02006fca(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_0200707c(2, x, z);
    }
    {
        s32 x = Func_02006fe4(928);
        s32 z = Func_02006fee(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_020070a0(3, x, z);
    }
}

void Colosso_MarkSceneProgress(void)
{
    extern s16 Data_02000240[];

    u8 *state;
    s16 *table;
    s32 slotValue;
    s32 value;
    u16 *field;

    state = *(u8 **)0x03001ebc;
    table = Data_02000240;
    slotValue = *(s32 *)&table[250];
    if (slotValue != 0) {
        if ((s16)*(u16 *)(state + 382) >> 10 == slotValue) {
            if (Func_020070e6(0x141) != 0) {
                field = (u16 *)(state + 386);
                value = 99;
                *field = value;
            }
        }
    }
}

void Colosso_SelectNearestObstacle(void)
{
    extern s16 Data_02000240[];

    u8 *state;
    s16 *table;
    StageActor *target;
    StageActor *actor;
    s32 *frame;
    s32 active_slot;
    s32 slot;
    s32 best;
    s32 best_slot;
    s32 dx;
    s32 adx;
    s32 dz;
    s32 base;
    s32 z;

    state = *(u8 **)0x03001ebc;
    best_slot = 8;
    best = 0x100000;
    table = Data_02000240;
    active_slot = *(s32 *)&table[250];
    target = Func_02007198(active_slot);
    Func_02007186();
    for (slot = 8; slot <= 66; slot++) {
        actor = Func_020071a6(slot);
        if (actor == 0) {
            continue;
        }
        if (actor->state != 1) {
            continue;
        }
        if (*actor->sprite->entry != 165) {
            continue;
        }
        dx = (target->x - actor->x) / 65536;
        dz = (target->z - actor->z) / 65536;
        if (dz > 0) {
            continue;
        }
        adx = dx;
        if (adx < 0) {
            adx = -adx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (adx + dz < best) {
            best_slot = slot;
            best = adx + dz;
        }
    }
    Func_020072a0(0x2085);
    Func_020072b8(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Func_02007210_a(20);
    Func_02007374();
    Func_02007380();
    base = active_slot * 16;
    Func_02007200(base + 880, target->x >> 20);
    z = target->z >> 20;
    Func_02007210_b(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Func_0200735a(10);
        Func_0200720a(282);
    } else {
        Func_02004e9a(active_slot);
        Func_020073ae();
        Func_020073c2();
        *frame = 0;
    }
    Func_02007274();
}

void Colosso_ClearSavedActorPositions(void)
{
    Func_0200742a(896, 0);
    Func_02007434(904, 0);
    Func_0200743e(912, 0);
    Func_02007448(920, 0);
    Func_02007452(928, 0);
    Func_0200745c(936, 0);
}

s32 Colosso_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 Data_02000240[];

    s32 stage_variant;
    s32 script_id;
    s32 result;

    Func_0200760c();
    Func_0200741c(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Func_0200757a(script_id);
    Func_02007592(actor_handle, 0);
    if (Func_0200748c(interaction_base + 512) != 0) {
        return 2;
    }
    if (Func_0200749c(interaction_base + 520) != 0) {
        result = Func_0200747e(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    Func_020074cc(interaction_base + 520);
    Func_020075ca((s32)&Value_0000207c);
    Func_020075da(actor_handle, 0);
    return Func_0200753a(0, 0);
}

void Colosso_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 Data_02000240[];

    s32 stage_variant;
    s32 script_id;

    Func_020074d2(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Func_02007632(script_id + 1);
    Func_0200764a(actor_handle, 0);
}

void Colosso_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Func_020077bc(handle);
    Func_020077ce(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Func_020077ea(handle, slot);
        }
    }
}

void Colosso_InitializeModeTask(u32 mode, u32 parameter)
{
    s32 handler;

    Data_0200dbd0 = (u16)mode;
    Data_0200dba4 = (u16)(parameter << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200aee9;
        Func_02007c38(task, budget);
    }

    handler = (s32)&Data_0200d9a6;
    if (mode == 2) {
        handler = (s32)&Data_0200ccba;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200d9d2;
    }
    if (mode == 3) {
        if (parameter != 0) {
            handler = (s32)&Data_0200cce2;
        } else {
            handler = (s32)&Data_0200da50;
        }
    }

    Data_0200dbdc = 0;
    Data_0200dbe0 = handler;
    Data_0200dc38 = 0;
    Data_0200dbac = 0;
    Data_0200dbb0 = 0;
}

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Audio_PlayCue, Func_02002e54, Func_020033d8, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */
void Colosso_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Func_02007e72();
        Func_02007fc6();
        Func_02007fda();
        Func_02007e78(30);
        Func_02008026(0x59);
        Func_020062e0(0);
        Func_0200686c(1, 0);
        Func_02007e92(120);
        Func_02007ea6();
        return;
    }

    Func_02008046(0xf7);
    Func_02007eaa();
    Func_02007ffe();
    Func_02008012();
    {
        s16 *base = (s16 *)0x0200d9a6;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Func_02007eba(30);
    Func_0200806a(mode + 0x5a);
    Func_02006324(mode);
    Func_020068b0(1, 0);
    Func_02007ed6(120);

    goto check_transition;
wait_transition:
    Func_02007d26(1);
check_transition:
    if (Func_02008092() != 0)
        goto wait_transition;

    Func_02008094(0x121);
    Func_0200634e(5);
    Func_020068da(2, 0);
    Func_020080a8(0xec);
    Func_02007f06(60);
    Func_020068ee(2, 1);
    Func_020080bc(0xec);
    Func_02007f1a(60);
    Func_0200637c(6);
    Func_02006908(2, 0);
    Func_020080d6(0xec);
    Func_02007f34(60);
    Func_02006396(7);
    Func_02006922(4, 0);
    Func_020080f0(0xed);
    Func_020080cc();
    Func_02007f60();
    Func_02007f16(0x123);
}

void Colosso_ResetActorMotion(s32 selector)
{
    u8 *record;

    record = Func_020082f8(selector);
    Func_020081e6();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

void Colosso_EnsurePaletteHandle(void)
{
    s16 *cursor = &Data_0200dace;

    if (*cursor == -1) {
        *cursor = Func_0200829e();
    }
}

void Colosso_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Func_02007362(first_value, second_value, mode);

    Data_0200dc34 = (u16)first_value;
    Data_0200dbc0 = (u16)second_value;
    Data_0200db98 = (u16)(mode & 3);
    Data_0200dbb4 = 0;
    Data_0200dbcc = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_020082d4(task, budget);
    }
}

void Colosso_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    Data_0200dba0 = (u16)first_value;
    Data_0200dc40 = (u16)second_value;
    Data_0200dbe4 = Data_0200dc34;
    Data_0200dbfc = Data_0200dbc0;
    Data_0200dbcc = (u16)mode;
    Data_0200db90 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_02008330(task, budget);
    }
}

void Colosso_StopPaletteTask(void)
{
    Func_0200836e(Func_0200b91c);
    Func_020083c8(Data_0200dace);
    Data_0200dace = -1;
}

void Colosso_PositionScaledObject(s32 id, s32 x, s32 z)
{
    ScaledStageObject *object = Func_020086b0(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        Func_02008464(object);
        Func_0200844c(object, 5);
        Func_02008488(object, x << 16, object->y, z << 16);
    }
}

void Colosso_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    u8 *object = Func_020086f0(object_id);

    if (object == 0) {
        return;
    }

    {
        s32 move_rate = 0x14000;
        u8 *state_byte = object;
        u8 zero = 0;
        *(s32 *)(object + 0x30) = move_rate;
        *(s32 *)(object + 0x34) = move_rate >> 1;
        state_byte += 0x5b;
        *state_byte = zero;
    }

    Func_020084a4();
    Func_0200848c(object, 5);
    Func_020084c8(object, x << 16, *(s32 *)(object + 12), z << 16);
    Func_020084d6(object);
    Func_020084a6(object, 1);
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void Colosso_SetBalanceStateReady(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}

void Colosso_WaitForBalanceState(void)
{
    extern s16 Data_02001000;

    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02008520(1);
    }
}

void Colosso_SpawnRandomSceneEffect(StageEffect *source)
{
    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Func_02008568();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Func_02008584();
        radius = Func_0200858a();
        Func_020085a6(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Func_0200863a(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Func_020086b6(effect, 0);
            Func_0200864e_b(effect, (s32)Data_0200ce50);
            Func_0200864e_a(effect, 1);
            Func_02008656(effect, 0);
        }
    }
}

s32 Colosso_RaiseLinkedSceneEffect(StageEffect_02003d88 *source)
{
    StageEffect_02003d88 *effect = Func_020087b4(source->linked_effect_slot);

    Func_020086be(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Func_020086a6(effect, (s32)Data_0200ce74);
    Func_02008964(83);
    source->linked_effect_slot = 0;
    return 0;
}

s32 Colosso_PositionActiveActor(s32 first_handle, s32 second_handle)
{
    extern u8 Data_02000240[];

    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_020087a0(0x211);

    shared = Data_02000240;
    record = Func_02008818_a(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Func_0200876a(record, x, 0, z);
    Func_02008818_b(0x211);
    Func_02008750(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Func_020086b6_a(1);
    }

    if (flag == 0) {
        Func_02006ca2(0, first_handle);
        Func_0200880a(first_handle, 2);
    } else {
        Func_02006cb4(0, second_handle);
        Func_0200881c(second_handle, 2);
    }

    shared = Data_02000240;
    Func_0200882c_a(*(s32 *)(shared + 500), 1);
    Func_0200882c_b(0x96a, 3);
    Func_020087ca(record);

    return flag;
}

void Colosso_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_02008d2e(59, 0x7170);
    handle = Func_02008d48(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_02008f08(first_actor);
    second_record = Func_02008f10(second_actor);

    if (Func_02008eb0(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02008dbe(Data_0200cd80, handle);

    extent = Func_02008de2();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_02008dee((s16)extent, 512, handle);

    Func_02008d8e((s32)Func_0200c0d0 + 1, 0xc76);

    Func_02008ddc(handle);
}

void Colosso_InitializeSceneControl(void)
{
    extern SceneControl Data_02001000;

    u8 *scene_state = Data_03001f3c;
    SceneControl *control = &Data_02001000;

    Func_02008e26(Func_02008e56(), (s32)(scene_state + 240));
    if (Func_02008f44(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(scene_state + 224);
        control->timer = 0;
        control->phase = 0;
    }
    {
        s32 event_id = 0xc85;

        Func_02008dfa((s32)Data_0200bef1, event_id);
    }
}

void Colosso_SetSceneControlValue(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}

void Colosso_PushStagedActor(void)
{
    extern s16 Data_02000240[];

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)Data_02000240 + (data_index << 1));
    subject = Func_0200905c(subject_handle);

    direction = subject->facing >> 12;

    step = Data_0200cfc0[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Func_02008c56(&position, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Func_02008c80(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
    position.z = target->z;

    blocker = Func_02008cac(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Func_0200905e(target, &position) > 0) {
        return;
    }

    Func_02008ffc(subject, 8);
    Func_02008f62(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02009042(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02009052(subject, position.x, position.y, position.z);

    Func_020092e8(0xee);
    Func_02009066(target);
    Func_020092f6(0x120);                                /* 144 << 1 */

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_0200904c(subject, 1);
}

/* This overlay's own byte-exact occupancy lookup. */
s32 *Colosso_FindActorAhead(void)
{
    extern s16 Data_02000240[];

    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Func_020091bc(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_02009046(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_02008dc6(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_02009078(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_02008df8(position, record);
    }

    return occupant;
}
#undef Colosso_PositionAndActivateActor
#undef Colosso_RunLogRollingInteraction
#undef Colosso_RestoreActorPositions
#undef Colosso_MarkSceneProgress
#undef Colosso_SelectNearestObstacle
#undef Colosso_ClearSavedActorPositions
#undef Colosso_RunStateInteraction
#undef Colosso_InitializeStateInteraction
#undef Colosso_ApplyItemToMatchingSlots
#undef Colosso_InitializeModeTask
#undef Colosso_RunScriptedTransition
#undef Colosso_ResetActorMotion
#undef Colosso_EnsurePaletteHandle
#undef Colosso_StartPaletteTask
#undef Colosso_StartPaletteTaskFromState
#undef Colosso_StopPaletteTask
#undef Colosso_PositionScaledObject
#undef Colosso_SpawnPositionedObject
#undef Colosso_SetBalanceStateReady
#undef Colosso_WaitForBalanceState
#undef Colosso_SpawnRandomSceneEffect
#undef Colosso_RaiseLinkedSceneEffect
#undef Colosso_PositionActiveActor
#undef Colosso_SetupSceneDescriptor
#undef Colosso_InitializeSceneControl
#undef Colosso_SetSceneControlValue
#undef Colosso_PushStagedActor
#undef Colosso_FindActorAhead

/* map/locations/colosso/log_rolling_stage/party.c */
#define GetPartyInteractionRecord Func_020073ca
#define FinishPartyInteractionRecordRead Func_020073be
#define GetPartyMemberCount Func_0200739a
#define SetInteractionCue Func_0200747c
#define CheckActorInteraction Func_0200749c_a
#define PlaceSelectedActor Func_02007448_a
#define SetInteractionStep Func_020073fe
#define PlaceActorZero Func_02007460
#define PlaceSupportActor Func_0200746c
#define StartInteractionPhase Func_02007578
#define AdvanceInteractionPhase Func_02007584
#define SelectInteractionStep Func_0200753a_a
#define SetLargePartyInteractionCue Func_020074e2
#define RunLargePartyInteraction Func_020074fa
#define FinishPartyCountInteraction Func_02007446
#define RunPartyCountInteractionCopyB Func_020029ac

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *Func_020073ca(void);
void Func_020073be(void);
s32 Func_0200739a(void);
void Func_0200747c(s32 cue);
s32 Func_0200749c_a(s32 actorId, s32 mode);
void Func_0200740e(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_0200741c_a(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_02007448_a(s32 actorId, s32 x, s32 y);
void Func_020073fe(s32 step);
void Func_02007460(s32 actorId, s32 x, s32 y);
void Func_0200746c(s32 actorId, s32 x, s32 y);
void Func_02007578(void);
void Func_02007584(void);
void Func_0200753a_a(s32 step);
void Func_020074e2(s32 cue);
void Func_020074fa(s32 actorId, s32 mode);
void Func_02007446(void);

static inline void InitializeActorZero(void)
{
    Func_0200740e(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Func_0200741c_a(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteractionCopyB(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    FinishPartyInteractionRecordRead();

    if (GetPartyMemberCount() <= 1) {
        SetInteractionCue(0x20e5);
        if (CheckActorInteraction(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            PlaceSelectedActor(actorId, x, y + 0x40);
            SetInteractionStep(15);
            PlaceActorZero(0, x, y);
            PlaceSupportActor(0, x, y + 0x20);
            StartInteractionPhase();
            AdvanceInteractionPhase();
            SelectInteractionStep(11);
        }
    } else {
        SetLargePartyInteractionCue(0x20e8);
        RunLargePartyInteraction(actorId, 0);
    }

    FinishPartyCountInteraction();
}
#undef GetPartyInteractionRecord
#undef FinishPartyInteractionRecordRead
#undef GetPartyMemberCount
#undef SetInteractionCue
#undef CheckActorInteraction
#undef PlaceSelectedActor
#undef SetInteractionStep
#undef PlaceActorZero
#undef PlaceSupportActor
#undef StartInteractionPhase
#undef AdvanceInteractionPhase
#undef SelectInteractionStep
#undef SetLargePartyInteractionCue
#undef RunLargePartyInteraction
#undef FinishPartyCountInteraction
#undef RunPartyCountInteractionCopyB

/* map/locations/colosso/log_rolling_stage/run_scene_four_step_actor_motion.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_0200499e();
s32 Func_02004bb4();
void Func_02004dd0();
s32 Func_02004e4c();
s32 Func_020057b2();
void Func_02005a8c();
void Func_02006b02();
void Func_02006b10();
void Func_02006b18();
void Func_02006b24();
void Func_02006b32();
void Func_02006b44();
void Func_02006b4e();
void Func_02006b52();
void Func_02006b64();
void Func_02006b72();
void Func_02006ba8();
s32 Func_02006bda();
void Func_02006bdc();
void Func_02006bde();
void Func_02006be4();
void Func_02006c00();
void Func_02006c1e();
void Func_02006c26();
void Func_02006c32();
void Func_02006c4c();
void Func_02006c50();
void Func_02006c58();
s32 Func_02006c68();
void Func_02006c70();
void Func_02006c92();
void Func_02006cac();
void Func_02006cae();
void Func_02006cb2();
void Func_02006cd2();
void Func_02006cd4();
void Func_02006d2a();
void Func_02006d3e();
void Func_02006d56();
void Func_02006d68();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Scene_RunFourStepActorMotion(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 rec5;
    s32 rec7;
    s32 record;
    s32 r10;
    s32 v6;
    u8 *p6;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200499e();
        v6 = r10;
    } else {
        Func_02006b18();
        rec5 = Value2(Func_02004bb4, a0, 4);
        if (rec5 != 0) {
            v6 = r10;
        } else {
            Call1(Func_02006bde, 0x20bf);
            Call2(Func_02006c32, 0x30000, 0x6000);
            Call4(Func_02006c4c, 0x3580000, -1, 0xa80000, 1);
            Func_02006c58();
            Func_02006b4e(30);
            Func_02006c1e(a0, 0);
            Func_02006c26(a0, 0);
            Value3(Func_020057b2, 0, 0x330, 200);
            Call3(Func_02006ba8, 0, 0x10000, 0x8000);
            Call3(Func_02006bdc, 0, 0x348, 200);
            Value3(Func_02006c68, 0, 0xc000, 20);
            Func_02006cd4();
            Call4(Func_02006cae, -1, -1, -1, 0);
            Call3(Func_02006be4, 0, 0x8000, 0x4000);
            rec7 = Value1(Func_02006bda, 0);
            p8 = *(volatile s32 *)(rec7 + 12);
            p6 = *(volatile s32 *)(rec7 + 8);
            Call3(Func_02006c00, 0, 0x8000, 0x4000);
            Func_02006c50(0, 10);
            p9 = (0x60000 + p8);
            Func_02006b02(rec7, (s32)p6, p9, *(volatile s32 *)(rec7 + 16));
            Func_02006b10(rec7);
            Func_02006c70(0, 14);
            p10 = (0x400000 + (s32)p6);
            Func_02006b24(rec7, p10, p9, *(volatile s32 *)(rec7 + 16));
            Func_02006b32(rec7);
            Func_02006c92(0, 10);
            Func_02006b44(rec7, p10, (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Func_02006b52(rec7);
            Func_02006cb2(0, 15);
            v6 = ((s32)p6 + 0x300000);
            v6 = ((s32)p6 + 0x300000);
            Func_02006b64(rec7, ((s32)p6 + 0x300000), (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Func_02006b72(rec7);
            Func_02006cd2(0, 12);
            Func_02006d2a(a0, 0);
            Func_02005a8c(0);
            Func_02006d68(0, 0);
            Func_02004dd0(a0, 4);
            goto L_02002298;
        }
        if (rec5 == 1) {
            Call1(Func_02006d3e, 0x20be);
            Func_02006d56(a0, 0);
        }
        L_02002298:;
        Value3(Func_02004e4c, rec5, a0, 4);
        Func_02006cac();
    }
    p10b = v6;
    p11 = a0;
}

/* map/locations/colosso/log_rolling_stage/run_scene_late_sequence.c */
#define Scene_RunMiddleSequence Func_02002bac

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
void Func_02007464();
s32 Func_020075b4();
s32 Func_020075c0();
s32 Func_020075de();
void Func_02007640();
s32 Func_0200764c_a();
s32 Func_0200764c_b();
void Func_02007654();
void Func_0200767e();
void Func_02007690();
void Func_020076dc();
s32 Func_020076ec();
void Func_02007718();
void Func_02007726();
s32 Func_0200772a();
void Func_02007734();
void Func_02007772();
void Func_0200778a_a();
void Func_0200778a_b();
void Func_02007788();
void Func_02007782();
void Func_0200777c();
s32 Func_0200778c();
void Func_020077a2();
void Func_020077a0();
void Func_020077b2();
s32 Func_020077b4();
void Func_020077bc();
void Func_020077d0();
void Func_020077ce();
void Func_020077e6();
s32 Func_020077e2();
s32 Func_02007816();
void Func_020077f8();
void Func_02007802();
void Func_02007862();
void Func_020078fc();

void Func_020077ae();
void Func_020077be();
void Func_020077ee();
void Func_020077f6();
/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void Scene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1(Func_020075de, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_020075b4);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Func_02007772(0x2083);
            Func_0200778a_a(owner, 0);
            return;
        }
        if (Value1(Func_020075c0, base + 512) != 0) {
            Func_02007772(0x2084);
            Func_0200778a_a(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Func_02007464(6);
        } else {
            Call1(Func_020076dc, 0x207d);
            Value2(Func_020076ec, owner, 0);
            state = Value2(Func_0200764c_a, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_02007640((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_02007654((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_02007816);
            for (i = 0; i < count; i++) {
                Func_0200767e((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02007690((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Func_02007772(0x207e);
    Func_0200778a_a(owner, 0);
    return;
L_main:
    ((void (*)())Func_0200764c_b)(obj, 1);
    Call1(Func_0200778a_b, 0x207f);
    Func_020077a2(owner, 0);
    Call3(Func_02007718, 0, 0x10000, 0x8000);
    Call3(Func_02007726, obj, 0x10000, 0x8000);
    Call3(Func_02007734, owner, 0x10000, 0x8000);
    record = Value1(Func_0200772a, 0);
    if (record != 0) {
        Func_02007788(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Func_02007782(obj, p9, hi);
    lo = p9 + 16;
    Value3(Func_0200778c, 0, lo, hi);
    Func_020077e6(obj, 0, 30);
    Func_020077be(obj, 3);
    tail = hi - 32;
    Func_020077d0(0, 3);
    Func_020077b2(owner, p9, tail);
    Value3(Func_020077b4, owner, lo, tail);
    Func_020078fc(0, obj);
    Func_020077ce(obj, p9, tail);
    Func_020077f6(owner, 1);
    Call3(Func_02007862, owner, 0x8000, 0);
    Func_020077ee(obj, p9, p11 - 48);
    Func_020077f8(owner, p9, tail);
    Func_02007802(owner, p9, p11);
    Func_020077a0(obj);
    Func_0200777c(base + 512);
    rec = Value1(Func_020077e2, obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Func_020077ae((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Func_020077bc((obj << 4) + 888, sy);
}
#undef Scene_RunMiddleSequence

/* map/locations/colosso/log_rolling_stage/run_second_arrival_sequence.c */
/* AUDITED GENERATED CALL SCRIPT for Scene_RunSecondArrivalSequence:
 * state-routed scene setup and all 40 calls with their scene arguments. */

#define Scene_RunSecondArrivalSequence Func_02001df8

void Func_0200469a();
s32 Func_020048b0();
void Func_02004aaa();
void Func_02004b24();
s32 Func_020054a8();
void Func_02005766();
void Func_02005960();
void Func_020059ce();
void Func_020059de();
void Func_020059ee();
s32 Func_02005a00();
void Func_02005a0c();
void Func_02005a5c();
void Func_02006814();
void Func_0200684c();
void Func_0200688a();
void Func_0200689e();
void Func_020068da();
void Func_02006910();
void Func_0200691e();
void Func_02006920();
void Func_0200692e();
void Func_02006930();
void Func_02006948();
void Func_02006956();
void Func_0200cad0();
void Func_0200ca08();
void Func_020069a4();
void Func_020069a8();
void Func_020069be();
void Func_0200cb30();
void Func_020069ca();
void Func_020069e8();
void Func_020069ea();
void Func_02006a00();
void Func_02006a08();
void Func_02006a16();
void Func_02006a2e();
void Func_02006a42();


/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

extern s16 Data_02000240[];

void Func_02001df8(s32 scene)
{
    s32 state;

    if (Data_02000240[225] == 2) {
        Func_0200469a();
        return;
    }
    Func_02006814();
    state = Func_020048b0(scene, 2);
    if (state == 0) {
    Call1(Func_020068da, 8375);
    Call2(Func_0200692e, 196608, 24576);
    Call4(Func_02006948, 24641536, -1, 9961472, 1);
    Func_02006956();
    Func_0200684c(30);
    Func_0200691e(scene, 0);
    Func_020054a8(0, 280, 200);
    Call3(Func_0200689e, 0, 98304, 49152);
    Func_02005a00(0, 280, 152);
    Call3(Func_02005a0c, 0, 296, 152);
    Func_0200688a(10);
    Func_0200cb30();
    Call4(Func_020069a8, -1, -1, -1, 0);
    Value3(Func_0200cad0, 0, 49152, 15);
    Func_020069e8();
    Call4(Func_020069ca, -1, -1, -1, 0);
    Value3(Func_020069a4, 0, 0, 15);
    Func_02006a08();
    Call4(Func_020069ea, -1, -1, -1, 0);
    Call3(Func_0200cad0, 0, 16384, 15);
    Func_020069be(scene, 0);
    Value3(Func_02005960, 96, 40, 0);
    Func_020059ce(128, 40, 10);
    Func_02006910(30);
    Func_020059de(160, 40, 10);
    Func_02006920(30);
    Func_020059ee(160, 72, 10);
    Func_02006930(30);
    Func_02006a00(scene, 0);
    Func_02005a5c();
    Func_02005766(0);
    Func_02006a42(0, 0);
    Func_02004aaa(scene, 2);
    } else if (state == 1) {
        Call1(Func_02006a16, 0x20b6);
        Func_02006a2e(scene, 0);
    }
    Value3(Func_02004b24, state, scene, 2);
    Func_0200ca08();
}
#undef Scene_RunSecondArrivalSequence

/* map/locations/colosso/log_rolling_stage/scene_primary_script_head.c */
#define ObjectTable_DestroyById_1(args...) Func_02005ea2(args)
#define ObjectTable_DestroyById_2(args...) Func_02005ea8(args)
#define Audio_PlayCue_1(args...) Func_02006034_a(args)
#define Battle_Reset_1(args...) Func_02005e98(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02005f0e, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02005f22_a(args)
#define Object_SetModeById_2(args...) Func_02005f2c(args)
#define Motion_EnableActCb_1(a0, a1) Value2(Func_02005ef4, a0, a1)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_02005f3a, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02005ef0(args)
#define Motion_EnableActCb_2(a0, a1) Value2(Func_02005f14, a0, a1)
#define Object_SetModeById_3(args...) Func_02005f5c(args)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02005f22_b, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02005f30, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02005f3e, a0, a1, a2)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_02005f8c, a0, a1, a2)
#define Motion_SetHPosTerrain_4(a0, a1, a2) Call3(Func_02005f9a, a0, a1, a2)
#define Motion_SetHPosTerrain_5(a0, a1, a2) Call3(Func_02005fa8, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02005f5e, a0)
#define Scene_GetRecord_3(a0) Value1(Func_02005f66, a0)
#define Scene_GetRecord_4(a0) Value1(Func_02005f6e, a0)
#define Motion_SetPosReset_1(args...) Func_02006056_a(args)
#define Battle_WaitMode0_1(a0, a1) Call2(Func_020060be, a0, a1)
#define Battle_WaitMode0_2(args...) Func_020060d2(args)
#define Motion_SetSpeed_4(args...) Func_020060e6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_0200603c, a0)
#define Battle_WaitMode0_3(args...) Func_02005f8a(args)
#define Motion_EnableActCb_3(args...) Func_02005fcc(args)
#define Scene_GetRecord_5(args...) Func_02005fba(args)
#define Scene_GetRecord_6(args...) Func_02005fc8(args)
#define Motion_CallWaitAnim_1(args...) Func_02006034_b(args)
#define Scene_GetRecord_7(args...) Func_02005fda(args)
#define Battle_WaitMode0_4(args...) Func_02005fca(args)
#define Scene_GetRecord_8(args...) Func_02005ff0(args)
#define Battle_WaitMode0_5(args...) Func_02005fdc(args)
#define Motion_EnableActCb_4(a0, a1) Value2(Func_0200601c, a0, a1)
#define BattleEv_RunWait_1(args...) Func_020060b4(args)
#define Battle_WaitMode0_6(args...) Func_02005ff2(args)
#define Motion_SetPosReset_2(a0, a1, a2) Call3(Func_02006056_b, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_020060e2, a0, a1, a2)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Value3(Func_020060fc_a, a0, a1, a2)
#define BattleEv_RunWait_2(args...) Func_020060e4(args)
#define Object_LinkObjectAndSetCallback_1(args...) Func_020061b4(args)
#define Battle_WaitMode0_7(args...) Func_0200602a(args)
#define Motion_SetPosReset_3(a0, a1, a2) Call3(Func_0200608e, a0, a1, a2)
#define Motion_ResetPosMode2_1(a0, a1, a2) Call3(Func_02006092, a0, a1, a2)
#define Motion_SetPosReset_4(a0, a1, a2) Call3(Func_020060a6, a0, a1, a2)
#define Object_SetModeById_4(args...) Func_020060ce(args)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_0200613a, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02006146, a0, a1, a2)
#define Motion_SetVarCbObj_1(args...) Func_02006106(args)
#define Battle_WaitMode0_8(args...) Func_0200607c(args)
#define Object_LinkObjectAndSetCallback_2(args...) Func_02006214(args)
#define Battle_WaitMode0_9(args...) Func_0200608a(args)
#define Motion_CallWaitAnim_2(args...) Func_02006112_a(args)
#define Battle_WaitMode0_10(args...) Func_02006098(args)
#define BattleEv_RunWait_3(args...) Func_02006168(args)
#define Object_LinkObjectAndSetCallback_3(args...) Func_02006238(args)
#define Object_LinkObjectAndSetCallback_4(args...) Func_02006240(args)
#define Motion_SetPosReset_5(a0, a1, a2) Call3(Func_02006114, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_5(args...) Func_02006254(args)
#define Object_LinkObjectAndSetCallback_6(args...) Func_0200625c(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_020061c0, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_7(args...) Func_02006270(args)
#define Object_LinkObjectAndSetCallback_8(args...) Func_02006278(args)
#define Battle_WaitMode0_11(args...) Func_020060ee(args)
#define Motion_SetVarCbObj_2(args...) Func_02006186(args)
#define Battle_WaitMode0_12(args...) ((void (*)())Func_020060fc_b)(args)
#define Object_LinkObjectAndSetCallback_9(args...) Func_02006294(args)
#define Object_LinkObjectAndSetCallback_10(args...) Func_0200629c(args)
#define Battle_WaitMode0_13(args...) Func_02006112_b(args)
#define BattleEv_RunWait_4(args...) Func_020061e2(args)
#define Motion_EnableActCb_5(a0, a1) Value2(Func_0200615a, a0, a1)
#define Motion_SetVarCbObj_3(args...) Func_020061ba(args)
#define Object_LinkObjectAndSetCallback_11(args...) Func_020062c2(args)
#define Battle_WaitMode0_14(args...) Func_02006138(args)
#define BattleEv_RunWait_5(args...) Func_02006208(args)
#define Motion_SetVarCbObj_4(args...) Func_020061d8(args)
#define Object_LinkObjectAndSetCallback_12(args...) Func_020062e0_a(args)
#define Battle_WaitMode0_15(args...) Func_02006156(args)
#define BattleEv_RunWait_6(args...) Func_02006226(args)
#define Motion_SetVarCbObj_5(args...) Func_020061f6(args)
#define Object_LinkObjectAndSetCallback_13(args...) Func_020062fe(args)
#define Battle_WaitMode0_16(args...) Func_02006174(args)
#define BattleEv_RunWait_7(args...) Func_02006244(args)
#define Motion_EnableActCb_6(args...) Func_020061bc(args)
#define Battle_WaitMode0_17(args...) Func_0200618a(args)
#define Battle_WaitMode0_18(args...) Func_020061a4(args)
#define Battle_WaitMode0_19(args...) Func_020061b0(args)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_020062a2, a0, a1, a2)
#define Battle_WaitMode0_20(args...) Func_020061ca(args)
#define Battle_WaitMode0_21(args...) Func_020061d6(args)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_020062c8, a0, a1, a2)
#define Battle_WaitMode0_22(args...) Func_020061f4(args)
#define Battle_WaitMode0_23(args...) Func_02006200(args)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_020062f4, a0, a1, a2)
#define Battle_WaitMode0_24(args...) Func_02006220(args)
#define Battle_WaitMode0_25(args...) Func_0200622c(args)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_02006320_a, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_14(args...) Func_020063d0_a(args)
#define Object_LinkObjectAndSetCallback_15(args...) Func_020063d8(args)
#define Battle_WaitMode0_26(args...) Func_0200624e(args)
#define Object_LinkObjectAndSetCallback_16(args...) Func_020063e6(args)
#define Object_LinkObjectAndSetCallback_17(args...) Func_020063ee(args)
#define Battle_WaitMode0_27(args...) Func_02006272(args)
#define Battle_WaitMode0_28(args...) Func_0200627e(args)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Call3(Func_02006372, a0, a1, a2)
#define Motion_SetVarCb_1(args...) Func_0200631a(args)
#define Motion_SetVarCb_2(args...) Func_02006322_a(args)
#define Motion_SetVarCbObj_6(args...) Func_02006332_a(args)
#define Object_LinkObjectAndSetCallback_18(args...) Func_0200643a(args)
#define Object_LinkObjectAndSetCallback_19(args...) Func_02006472(args)
#define Object_SetModeById_5(args...) Func_02006370(args)
#define Object_SetModeById_6(args...) Func_02006378(args)
#define Object_SetModeById_7(args...) Func_02006380(args)
#define Battle_WaitMode0_29(args...) Func_0200630e(args)
#define Motion_ResetPosMode2_2(a0, a1, a2) Call3(Func_0200636a, a0, a1, a2)
#define Battle_WaitMode0_30(args...) Func_02006320_b(args)
#define Motion_ResetPosMode2_3(a0, a1, a2) Call3(Func_0200637c_a, a0, a1, a2)
#define Battle_WaitMode0_31(args...) Func_02006332_b(args)
#define Motion_SetPosReset_6(a0, a1, a2) Call3(Func_02006396_a, a0, a1, a2)
#define Motion_ResetPosMode2_4(a0, a1, a2) Call3(Func_0200639a, a0, a1, a2)
#define Motion_CommitPos_1(args...) Func_020063b8(args)
#define Object_SetModeById_8(args...) Func_020063d0_b(args)
#define Object_LinkObjectAndSetCallback_20(args...) Func_020064f0(args)
#define Battle_WaitMode0_32(args...) Func_02006366(args)
#define Motion_SetPosReset_7(a0, a1, a2) Call3(Func_020063ca, a0, a1, a2)
#define Motion_ResetPosMode2_5(a0, a1, a2) Call3(Func_020063ce, a0, a1, a2)
#define Motion_SetSpeed_5(args...) Func_020064ee(args)
#define Battle_WaitMode0_33(args...) Func_0200638c(args)
#define Motion_SetHPosTerrain_6(a0, a1, a2) Call3(Func_0200640a, a0, a1, a2)
#define Motion_SetHPosTerrain_7(a0, a1, a2) Call3(Func_02006418, a0, a1, a2)
#define Motion_SetHPosTerrain_8(a0, a1, a2) Call3(Func_02006426, a0, a1, a2)
#define Battle_WaitMode0_34(args...) Func_0200650a(args)
#define Battle_WaitMode0_35(args...) Func_02006518(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02006486, a0)
#define BattleEv_RunWait_8(args...) Func_0200649e(args)
#define BattleEv_RunWait_9(args...) Func_020064a6(args)
#define BattleEv_RunWait_10(args...) Func_020064ae(args)
#define Battle_WaitMode0_36(args...) Func_020063ec(args)
#define Battle_SchedShoulder_1(args...) Func_02006400(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define PENDING_CALLBACK_FLAG (*(s32 *)0x0200db80)
#define Scene_RunClosingAuxiliarySequence Func_020002f8
#define Scene_RunFinalAuxiliarySequence Func_020004a4
#define Scene_RunEarlySequence Func_0200076c
#define Scene_RunSupplementalSequenceOne Func_02000ba4
#define Scene_RunMultiPhaseActorSequence Func_02001474

extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_00003333[];
extern u8 Data_00000091[];
extern u8 Data_00002073[];
extern u8 Data_02000434[];
extern u8 Data_0200d950[];

void Func_02004c06();
void Func_02004c6a();
void Func_02004c8a();
void Func_02004cae();
s32 Func_02004cc2();
void Func_02004cfe();
void Func_02004d00();
void Func_02004d26();
s32 Func_02004d36();
s32 Func_02004d54();
void Func_02004d70();
s32 Func_02004d78();
void Func_02004d96_a();
void Func_02004d96_b();
s32 Func_02004d98();
void Func_02004e0c();
void Func_02004e0e();
void Func_02004e2c();
void Func_02004e2e();
void Func_02004e46();
void Func_02004e90();
void Func_02004ea2();
s32 Func_02004ecc();
u8 *Func_02004ee4();
void Func_02004ef6();
s32 Func_02004f1c();
u8 *Func_02004f3c();
s32 Func_02000f3a();
s32 Func_02000f68();
void Func_02000fd0();
void Func_02005014();
void Func_02005036();
s32 Func_02005154();
s32 Func_0200519a();
s32 Func_020051a0();
s32 Func_020051a8();
void Func_020051c0();
void Func_020051d4();
void Func_020051d6();
void Func_020051e2();
void Func_020051e4();
s32 Func_02005208();
void Func_0200522a();
void Func_02005238();
void Func_0200523c();
void Func_02005246();
void Func_0200527a();
void Func_02005288();
void Func_02005298();
void Func_0200529e();
void Func_020052be();
void Func_020052c4();
void Func_020052d0();
void Func_020052f4();
void Func_0200530a();
void Func_02005310();
void Func_0200531c();
void Func_0200531e();
void Func_02005344();
void Func_0200534c();
void Func_020053a6();
void Func_020053b4();
void Func_020053da();
void Func_020053ee();
void Func_0200549c();
void Func_02005538();
void Func_0200553e();
void Func_02005550();
void Func_02005556();
void Func_02003262();
void Func_02004920();
void Func_02004942();
s32 Func_02004a48();
void Func_020055ce();
s32 Func_020055fa();
void Func_02005648();
void Func_02005664();
void Func_0200568a();
s32 Func_0200569a();
void Func_020056a6();
void Func_020056b4();
void Func_020056b6();
void Func_020056be();
void Func_020056c4();
void Func_020056d0();
void Func_020056da();
void Func_020056e2();
void Func_020056e4();
void Func_020056fc();
void Func_02005704();
void Func_02005714();
void Func_02005718();
void Func_02005724();
void Func_02005726();
void Func_0200572c();
void Func_02005744();
void Func_02005752();
void Func_02005754();
void Func_02005766();
void Func_02005770();
void Func_02005776();
void Func_0200577c();
void Func_0200578a();
void Func_0200578e();
void Func_02005796();
s32 Func_02005850();
s32 Func_0200586c();
void Func_0200587c();
void Func_02005d9e();
void Func_02005e7e();
void Func_02005e98();
void Func_02005ea2();
void Func_02005ea8();
s32 Func_02005ef0();
s32 Func_02005ef4();
void Func_02005f0e();
s32 Func_02005f14();
void Func_02005f22_a();
void Func_02005f22_b();
void Func_02005f2c();
void Func_02005f30();
void Func_02005f36();
void Func_02005f3a();
void Func_02005f3e();
void Func_02005f5c();
s32 Func_02005f5e();
s32 Func_02005f66();
s32 Func_02005f6e();
void Func_02005f8a();
void Func_02005f8c();
void Func_02005f9a();
void Func_02005fa8();
s32 Func_02005fba();
s32 Func_02005fc8();
void Func_02005fca();
void Func_02005fcc();
s32 Func_02005fda();
void Func_02005fdc();
void Func_02005fee();
s32 Func_02005ff0();
void Func_02005ff2();
void Func_02006002();
void Func_02006014();
s32 Func_0200601c();
void Func_02006028();
void Func_0200602a();
void Func_02006034_a();
void Func_02006034_b();
void Func_0200603c();
void Func_0200603e();
void Func_02006052();
void Func_02006056_a();
void Func_02006056_b();
void Func_0200606a();
void Func_0200607c();
void Func_0200607e();
void Func_0200608a();
void Func_0200608e();
void Func_02006092();
void Func_02006098();
void Func_020060a6();
void Func_020060b4();
void Func_020060bc();
void Func_020060be();
void Func_020060ce();
void Func_020060d0();
void Func_020060d2();
void Func_020060e2();
void Func_020060e4();
void Func_020060e6();
void Func_020060ee();
s32 Func_020060fc_a();
s32 Func_020060fc_b();
void Func_02006106();
void Func_02006112_a();
void Func_02006112_b();
void Func_02006114();
void Func_02006138();
void Func_0200613a();
void Func_02006140();
void Func_02006146();
void Func_02006156();
s32 Func_0200615a();
void Func_02006168();
void Func_02006174();
void Func_02006186();
void Func_0200618a();
void Func_020061a4();
void Func_020061b0();
void Func_020061b4();
void Func_020061ba();
void Func_020061bc();
void Func_020061c0();
void Func_020061ca();
void Func_020061d6();
void Func_020061d8();
void Func_020061e2();
void Func_020061f4();
void Func_020061f6();
void Func_02006200();
void Func_02006208();
void Func_02006214();
void Func_02006220();
void Func_02006226();
void Func_0200622c();
void Func_02006238();
void Func_02006240();
void Func_02006244();
void Func_0200624e();
void Func_02006254();
void Func_0200625c();
void Func_02006270();
void Func_02006272();
void Func_02006278();
void Func_0200627e();
void Func_02006294();
void Func_0200629c();
void Func_020062a2();
void Func_020062c2();
void Func_020062c8();
void Func_020062e0_a();
void Func_020062f4();
void Func_020062fe();
void Func_0200630e();
void Func_0200631a();
void Func_02006320_a();
void Func_02006320_b();
void Func_02006322_a();
void Func_02006322_b();
void Func_02006332_a();
void Func_02006332_b();
void Func_02006366();
void Func_0200636a();
void Func_02006370();
void Func_02006372();
void Func_02006378();
void Func_0200637c_a();
void Func_02006380();
void Func_0200638c();
void Func_02006396_a();
void Func_0200639a();
void Func_020063b8();
void Func_020063ca();
void Func_020063ce();
void Func_020063d0_a();
void Func_020063d0_b();
void Func_020063d8();
void Func_020063e6();
void Func_020063ec();
void Func_020063ee();
void Func_02006400();
void Func_0200640a();
void Func_02006418();
void Func_02006426();
void Func_0200643a();
void Func_02006472();
void Func_02006486();
void Func_0200649e();
void Func_020064a6();
void Func_020064ae();
void Func_020064e2();
void Func_020064ee();
void Func_020064f0();
void Func_0200650a();
void Func_02006518();
void Func_020093c1();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Slot at 0x0200db80 set just before installing one of the callbacks below. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void Scene_RunClosingAuxiliarySequence(void)
{
    u32 i;
    u8 *p9;
    s32 rec;
    s32 rec7;
    u8 *record;
    u8 *p6;

    u8 *base = Data_02000240;

    p6 = *(u8 **)(base + 500);
    rec = Value1(Func_02004cc2, 0x362);
    if (rec == 0) {
        record = Value1(Func_02004d36, 10);
        if ((s32)record != 0) {
            Func_02004d70((s32)p6, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02004d96_a((s32)p6);
        record = Value1(Func_02004d54, 11);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Func_02004c6a, (s32)record, *(s32 *)((s32)record + 8), 0x200000, *(s32 *)((s32)record + 16));
        record = Value1(Func_02004d78, 10);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Func_02004c8a, (s32)record, *(s32 *)((s32)record + 8), 0x40000, *(s32 *)((s32)record + 16));
        rec7 = Func_02004d98((s32)p6);
        p9 = rec7 + 85;
        *p9 = rec;
        *(s32 *)(rec7 + 52) = 0x6666;
        *(s32 *)(rec7 + 48) = 0xcccc;
        Call4(Func_02004cae, rec7, *(s32 *)(rec7 + 8), 0x40000, *(s32 *)(rec7 + 16));
        Func_02004cfe(rec7, 1);
        Func_02004e0c((s32)p6);
        Call6(Func_02004d00, 0, 24, 1, 1, 9, 12);
        Func_02004c06(2);
        Func_02004d26(rec7, 1);
        *p9 = 3;
        *(s32 *)(rec7 + 20) = *(s32 *)(rec7 + 12);
        Call1(Func_02004d96_b, 0x367);
    }
}

void Scene_RunFinalAuxiliarySequence(void)
{
    u8 *rec;
    u8 *b1;
    u8 *t;
    u8 *b2;
    u8 *b3;
    s32 two;
    s32 zero;
    s32 a;
    s32 b;

    rec = (u8 *)Value1(Func_02004ecc, 12);
    a = (*(s32 *)((s32)rec + 8) >> 20);
    if (a == 9) {
        b = (*(s32 *)((s32)rec + 16) >> 20);
        if (b == 12) {
            b1 = Func_02004ee4(12);
            Func_02004e2c((s32)b1, 0);
            t = b1 + 35;
            zero = 0;
            two = 2;
            *t = two;
            t += 50;
            *t = zero;
            *(s32 *)((s32)b1 + 52) = 0x6666;
            *(s32 *)((s32)b1 + 48) = 0xcccc;
            Call4(Func_02004e0e, (s32)b1, *(s32 *)((s32)b1 + 8), 0x40000, *(s32 *)((s32)b1 + 16));
            b2 = (u8 *)Value1(Func_02004f1c, 11);
            b2[35] = two;
            *(s32 *)((s32)b2 + 52) = 0x6666;
            *(s32 *)((s32)b2 + 48) = 0xcccc;
            Call4(Func_02004e2e, (s32)b2, *(s32 *)((s32)b2 + 8), 0x200000, *(s32 *)((s32)b2 + 16));
            b3 = Func_02004f3c(10);
            *(s32 *)((s32)b3 + 52) = 0x6666;
            *(s32 *)((s32)b3 + 48) = 0xcccc;
            Call4(Func_02004e46, (s32)b3, *(s32 *)((s32)b3 + 8), 0x40000, *(s32 *)((s32)b3 + 16));
            Call1(Func_02004ef6, 0x368);
            Call6(Func_02004e90, 15, 12, 1, 1, 13, b);
            Call6(Func_02004ea2, 1, 25, 1, 1, a, b);
        }
    }
}

void Scene_RunEarlySequence(void)
{
    s32 rec4;
    s32 rec2;
    s32 base5_2000434;
    s32 tile;
    s32 v3;
    s32 v7;
    s32 v9;
    s32 v5;
    s32 base5_3333;
    s32 slot16;
    s32 slot12;
    s32 record;
    s32 p5;
    s32 p6;
    s32 *q;
    u8 *base0;
    u8 *tbl;
    s32 off;
    s32 t;
    s32 a8;
    volatile s32 *fl;
    s32 slot20[3];

    base0 = Data_02000240;
    base5_2000434 = (s32)(base0 + 500);
    rec4 = Value1(Func_020051a0, *(volatile s32 *)base5_2000434);
    rec2 = Value1(Func_020051a8, 31);
    tbl = (u8 *)0x0200cc38;
    v9 = 0;
    off = (s32)(((u32)*(volatile u16 *)(rec4 + 6) >> 13) << 1);
    tile = *(volatile u16 *)(tbl + off);
    a8 = *(volatile s32 *)(rec4 + 8);
    p5 = *(volatile s32 *)base5_2000434;
    q = slot20;
    q[0] = ((a8 & -0x100000) + 0x80000);
    q[1] = *(volatile s32 *)(rec4 + 12);
    q[2] = ((*(volatile s32 *)(rec4 + 16) & -0x100000) + 0x80000);
    Call3(Func_02005036, 0x100000, tile, (s32)q);
    v7 = *(volatile s32 *)(rec2 + 8);
    v3 = q[0] - v7;
    p6 = *(volatile s32 *)(rec2 + 16);
    if (v3 < 0) {
        v3 = v7 - q[0];
        if (v3 > 0x80000) {
            goto L_020009ea;
        }
        t = q[2];
    } else {
        if (v3 > 0x80000) {
            goto L_020009ea;
        }
        t = q[2];
    }
    {
        if ((t - p6) >= 0) {
            if ((t - p6) > 0x200000) {
                goto L_020009ea;
            }
            fl = (volatile s32 *)0x03001ae8;
        } else {
            if ((p6 - t) > 0x200000) {
                goto L_020009ea;
            }
            fl = (volatile s32 *)0x03001ae8;
        }
        if ((*fl & 32) != 0) {
            slot16 = 2;
            slot12 = -8;
            L_0200081c:;
            v5 = (v7 + -0x100000);
            if (Value2(Func_02000f3a, v5, p6) != 0) {
                goto L_02000862;
            }
            v9 = (v9 + 1);
            v7 = v5;
            goto L_0200081c;
        }
        if ((*fl & 16) == 0) {
            goto L_020009f2;
        }
        slot16 = 3;
        slot12 = 8;
        L_02000848:;
        v5 = (v7 + 0x100000);
        if (Value2(Func_02000f68, v5, p6) == 0) {
            v9 = (v9 + 1);
            v7 = v5;
            goto L_02000848;
        }
        L_02000862:;
        if (v9 == 0) {
            goto L_020009f2;
        }
        Call6(Func_020051c0, 74, 8, 1, 4, (*(volatile s32 *)(rec2 + 8) >> 20), 9);
        Call6(Func_020051d4, 120, 60, 8, 5, 74, 60);
        Func_02005298();
        Func_02005310(p5, 8);
        Func_0200529e(6);
        *(volatile s32 *)(rec2 + 48) = 0x8000;
        base5_3333 = (s32)Data_00003333;
        *(volatile s32 *)(rec2 + 52) = base5_3333;
        Value2(Func_0200519a, rec2, slot16);
        Func_020051d6(rec2, v7, 0, p6);
        Func_020052c4(6);
        Func_02005344(p5, 2);
        record = Value2(Func_02005154, 27, 0xccc);
        Func_020051e2(*(volatile s32 *)((record + 0x1e0)), rec2);
        Call3(Func_0200531e, p5, 0x8000, base5_3333);
        Func_0200549c(239);
        Func_020051e4(rec4, 2);
        Func_0200522a(rec4, (((v9 *slot12) << 16) + *(volatile s32 *)(rec4 + 8)), 0, *(volatile s32 *)(rec4 + 16));
        Func_02005238(rec4);
        Value2(Func_02005208, rec4, 1);
        Func_02005246(rec2);
        if (v7 >= 0x5300000) {
            Call1(Func_020052f4, 0x369);
            Func_020053b4(31, 3);
            Func_020053a6(31, 18, 6);
            Func_0200534c(30);
            Func_0200523c(rec2, 8);
            Func_0200527a(rec2);
            *(u8 *)(rec2 + 35) = 2;
            v5 = 84;
            Call6(Func_020052be, 86, 10, 1, 2, v5, 10);
            Call6(Func_020052d0, 86, 9, 1, 1, v5, 12);
            Call1(Func_02005538, 0x120);
            Func_0200553e(240);
        } else {
            Func_02005288(rec2, 1);
            Call1(Func_02005550, 0x120);
            Func_02005556(213);
            v5 = (v7 >> 20);
            Call6(Func_0200530a, 85, 9, 1, 4, v5, 9);
            Call6(Func_0200531c, 85, 9, 1, 4, v5, 61);
        }
        Func_020053da(15);
        Func_020053ee();
        goto L_020009f2;
    }
    L_020009ea:;
    Func_02005014();
    Func_02000fd0();
    L_020009f2:;
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    u8 *base;
    s32 p10;
    s32 p8;
    s32 base7_2073;
    s32 threea0;
    s32 mode;

    p10 = *(volatile s32 *)Data_03001ebc;
    base = Data_02000240;
    p8 = *(volatile s32 *)(base + 500);
    mode = *(s16 *)(base + 450);
    if (mode == 2) {
        Func_020055ce();
        base7_2073 = (s32)Data_00002073;
        threea0 = (a0 << 1) + a0;
        Func_0200568a(threea0 + base7_2073);
        Value2(Func_0200569a, a0, 0);
        if (Value2(Func_020055fa, p8, 0) == 0) {
            s32 t1 = base7_2073 + 1;
            Func_020056a6(threea0 + t1);
            Func_020056be(a0, 0);
            *(volatile s32 *)((0x1c0 + p10)) = 0x200;
            *(volatile s32 *)((0x1c8 + p10)) = 15;
            Func_02005770();
            Func_0200577c();
            Func_02003262(a0);
            Func_02005776();
            Func_0200578a();
        } else {
            s32 t2 = base7_2073 + 2;
            Func_020056e4(threea0 + t2);
            Func_020056fc(a0, 0);
        }
        Func_02005648();
    }
}

/* Sets up actors 0-3 and a batch of movement/scale/rotation animation
 * records, then plays them through a long fixed sequence of moves,
 * rotations, and callback-driven steps. */
void Scene_RunMultiPhaseActorSequence(s32 a0)
{
    s32 record;
    s32 data_table_addr;
    s32 callback_target;

    ObjectTable_DestroyById_1(39);
    ObjectTable_DestroyById_2(40);
    Func_02005e7e(1);
    Audio_PlayCue_1(17);
    Battle_Reset_1();
    Motion_SetHPosTerrain_1(8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Object_SetModeById_1(8, 10);
    } else {
        Object_SetModeById_2(8, 8);
    }
    Motion_EnableActCb_1(8, 0x200d668);
    Motion_SetHPosTerrain_2(0, 0x5e00000, 0xc00000);
    record = Scene_GetRecord_1(0);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Motion_EnableActCb_2(0, 0x200d738);
    Object_SetModeById_3(0, 35);
    Motion_SetSpeed_1(1, 0x10000, 0x8000);
    Motion_SetSpeed_2(2, 0x10000, 0x8000);
    Motion_SetSpeed_3(3, 0x10000, 0x8000);
    Motion_SetHPosTerrain_3(1, 0x5b80000, 0xb80000);
    Motion_SetHPosTerrain_4(2, 0x5b80000, 0xc80000);
    Motion_SetHPosTerrain_5(3, 0x5a80000, 0xc00000);
    record = Scene_GetRecord_2(1);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_3(2);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_4(3);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Func_02005d9e(1);
    Motion_SetPosReset_1(0, 0);
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1(0x10001, 1);
    Battle_WaitMode0_2();
    Motion_SetSpeed_4();
    SceneWork_SetStepValue_1(0x20f1);
    Battle_WaitMode0_3(60);
    data_table_addr = (s32)Data_0200d950;
    Motion_EnableActCb_3(0, data_table_addr);
    record = Scene_GetRecord_5(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Scene_GetRecord_6(0);
    *(s32 *)(record + 28) = 0x10000;
    Motion_CallWaitAnim_1(0, 36);
    record = Scene_GetRecord_7(0);
    *(s32 *)(record + 8) += 0x30000;
    Battle_WaitMode0_4(10);
    record = Scene_GetRecord_8(0);
    Func_02005f36(record, 0);
    Battle_WaitMode0_5(20);
    Motion_EnableActCb_4(0, 0x200d808);
    BattleEv_RunWait_1(1, 0);
    Battle_WaitMode0_6(20);
    Motion_SetPosReset_2(1, 0x5e0, 176);
    Motion_ArmCb_1(1, 0x4000, 10);
    BattleFx_SpawnLinked_1(1, 0x100, 20);
    BattleEv_RunWait_2(2, 0);
    Object_LinkObjectAndSetCallback_1(1, 2);
    Battle_WaitMode0_7(30);
    Motion_SetPosReset_3(2, 0x5d0, 176);
    Motion_ResetPosMode2_1(1, 0x5f0, 184);
    Motion_SetPosReset_4(2, 0x5e0, 176);
    Object_SetModeById_4(1, 1);
    Motion_ArmCb_2(1, 0x6000, 0);
    Motion_ArmCb_3(2, 0x4000, 10);
    Motion_SetVarCbObj_1(2, 2);
    Battle_WaitMode0_8(10);
    Object_LinkObjectAndSetCallback_2(2, 1);
    Battle_WaitMode0_9(30);
    Motion_CallWaitAnim_2(1, 4);
    Battle_WaitMode0_10(30);
    BattleEv_RunWait_3(3, 0);
    Object_LinkObjectAndSetCallback_3(1, 3);
    Object_LinkObjectAndSetCallback_4(2, 3);
    Motion_SetPosReset_5(3, 0x5d0, 184);
    Object_LinkObjectAndSetCallback_5(2, 0);
    Object_LinkObjectAndSetCallback_6(1, 0);
    BattleFx_SpawnLinked_2(1, 0x102, 60);
    Object_LinkObjectAndSetCallback_7(2, 1);
    Object_LinkObjectAndSetCallback_8(1, 2);
    Battle_WaitMode0_11(40);
    Motion_SetVarCbObj_2(3, 2);
    Battle_WaitMode0_12(10);
    Object_LinkObjectAndSetCallback_9(2, 3);
    Object_LinkObjectAndSetCallback_10(1, 3);
    Battle_WaitMode0_13(20);
    BattleEv_RunWait_4(3, 0);
    Motion_EnableActCb_5(0, 0x200d8ac);
    Motion_SetVarCbObj_3(1, 2);
    Object_LinkObjectAndSetCallback_11(1, 0);
    Battle_WaitMode0_14(20);
    BattleEv_RunWait_5(1, 0);
    Motion_SetVarCbObj_4(2, 2);
    Object_LinkObjectAndSetCallback_12(2, 0);
    Battle_WaitMode0_15(20);
    BattleEv_RunWait_6(2, 0);
    Motion_SetVarCbObj_5(3, 2);
    Object_LinkObjectAndSetCallback_13(3, 0);
    Battle_WaitMode0_16(20);
    BattleEv_RunWait_7(3, 0);
    Motion_EnableActCb_6(0, data_table_addr);
    Battle_WaitMode0_17(60);
    callback_target = (s32)Func_020093c1;
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_02005fee, callback_target, 0xc80);
    Battle_WaitMode0_18(5);
    Func_02006002(callback_target);
    Battle_WaitMode0_19(55);
    BattleFx_SpawnLinked_3(1, 0x101, 60);
    Call2(Func_02006014, callback_target, 0xc80);
    Battle_WaitMode0_20(20);
    Func_02006028(callback_target);
    Battle_WaitMode0_21(40);
    BattleFx_SpawnLinked_4(2, 0x101, 60);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_0200603e, callback_target, 0xc80);
    Battle_WaitMode0_22(35);
    Func_02006052(callback_target);
    Battle_WaitMode0_23(25);
    BattleFx_SpawnLinked_5(3, 0x102, 60);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_0200606a, callback_target, 0xc80);
    Battle_WaitMode0_24(35);
    Func_0200607e(callback_target);
    Battle_WaitMode0_25(25);
    BattleFx_SpawnLinked_6(2, 0x102, 60);
    Object_LinkObjectAndSetCallback_14(3, 2);
    Object_LinkObjectAndSetCallback_15(2, 3);
    Battle_WaitMode0_26(60);
    Object_LinkObjectAndSetCallback_16(3, 0);
    Object_LinkObjectAndSetCallback_17(2, 0);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_020060bc, callback_target, 0xc80);
    Battle_WaitMode0_27(35);
    Func_020060d0(callback_target);
    Battle_WaitMode0_28(25);
    BattleFx_SpawnLinked_7(3, 0x108, 60);
    Motion_SetVarCb_1(1, 3);
    Motion_SetVarCb_2(2, 3);
    Motion_SetVarCbObj_6(3, 3);
    Object_LinkObjectAndSetCallback_18(3, 2);
    Object_LinkObjectAndSetCallback_19(1, 2);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_02006140, callback_target, 0xc80);
    Object_SetModeById_5(1, 3);
    Object_SetModeById_6(2, 3);
    Object_SetModeById_7(3, 3);
    Battle_WaitMode0_29(60);
    Motion_ResetPosMode2_2(3, 0x5b8, 200);
    Battle_WaitMode0_30(5);
    Motion_ResetPosMode2_3(2, 0x558, 184);
    Battle_WaitMode0_31(3);
    Motion_SetPosReset_6(1, 0x5e8, 184);
    Motion_ResetPosMode2_4(1, 0x558, 184);
    Motion_CommitPos_1(3);
    Object_SetModeById_8(3, 1);
    Object_LinkObjectAndSetCallback_20(3, 0);
    Battle_WaitMode0_32(60);
    Motion_SetPosReset_7(3, 0x598, 200);
    Motion_ResetPosMode2_5(3, 0x558, 184);
    Func_020064e2();
    Motion_SetSpeed_5();
    Battle_WaitMode0_33(30);
    Motion_SetHPosTerrain_6(1, 0x5e80000, 0xb00000);
    Motion_SetHPosTerrain_7(2, 0x5b80000, 0xc00000);
    Motion_SetHPosTerrain_8(3, 0x6180000, 0xc80000);
    Func_02006322_b();
    Battle_WaitMode0_34(0x10000, 2);
    Battle_WaitMode0_35(1);
    SceneWork_SetStepValue_2(0x214c);
    BattleEv_RunWait_8(1, 0);
    BattleEv_RunWait_9(2, 0);
    BattleEv_RunWait_10(3, 0);
    Battle_WaitMode0_36(60);
    Battle_SchedShoulder_1();
}
#undef ObjectTable_DestroyById_1
#undef ObjectTable_DestroyById_2
#undef Audio_PlayCue_1
#undef Battle_Reset_1
#undef Motion_SetHPosTerrain_1
#undef Object_SetModeById_1
#undef Object_SetModeById_2
#undef Motion_EnableActCb_1
#undef Motion_SetHPosTerrain_2
#undef Scene_GetRecord_1
#undef Motion_EnableActCb_2
#undef Object_SetModeById_3
#undef Motion_SetSpeed_1
#undef Motion_SetSpeed_2
#undef Motion_SetSpeed_3
#undef Motion_SetHPosTerrain_3
#undef Motion_SetHPosTerrain_4
#undef Motion_SetHPosTerrain_5
#undef Scene_GetRecord_2
#undef Scene_GetRecord_3
#undef Scene_GetRecord_4
#undef Motion_SetPosReset_1
#undef Battle_WaitMode0_1
#undef Battle_WaitMode0_2
#undef Motion_SetSpeed_4
#undef SceneWork_SetStepValue_1
#undef Battle_WaitMode0_3
#undef Motion_EnableActCb_3
#undef Scene_GetRecord_5
#undef Scene_GetRecord_6
#undef Motion_CallWaitAnim_1
#undef Scene_GetRecord_7
#undef Battle_WaitMode0_4
#undef Scene_GetRecord_8
#undef Battle_WaitMode0_5
#undef Motion_EnableActCb_4
#undef BattleEv_RunWait_1
#undef Battle_WaitMode0_6
#undef Motion_SetPosReset_2
#undef Motion_ArmCb_1
#undef BattleFx_SpawnLinked_1
#undef BattleEv_RunWait_2
#undef Object_LinkObjectAndSetCallback_1
#undef Battle_WaitMode0_7
#undef Motion_SetPosReset_3
#undef Motion_ResetPosMode2_1
#undef Motion_SetPosReset_4
#undef Object_SetModeById_4
#undef Motion_ArmCb_2
#undef Motion_ArmCb_3
#undef Motion_SetVarCbObj_1
#undef Battle_WaitMode0_8
#undef Object_LinkObjectAndSetCallback_2
#undef Battle_WaitMode0_9
#undef Motion_CallWaitAnim_2
#undef Battle_WaitMode0_10
#undef BattleEv_RunWait_3
#undef Object_LinkObjectAndSetCallback_3
#undef Object_LinkObjectAndSetCallback_4
#undef Motion_SetPosReset_5
#undef Object_LinkObjectAndSetCallback_5
#undef Object_LinkObjectAndSetCallback_6
#undef BattleFx_SpawnLinked_2
#undef Object_LinkObjectAndSetCallback_7
#undef Object_LinkObjectAndSetCallback_8
#undef Battle_WaitMode0_11
#undef Motion_SetVarCbObj_2
#undef Battle_WaitMode0_12
#undef Object_LinkObjectAndSetCallback_9
#undef Object_LinkObjectAndSetCallback_10
#undef Battle_WaitMode0_13
#undef BattleEv_RunWait_4
#undef Motion_EnableActCb_5
#undef Motion_SetVarCbObj_3
#undef Object_LinkObjectAndSetCallback_11
#undef Battle_WaitMode0_14
#undef BattleEv_RunWait_5
#undef Motion_SetVarCbObj_4
#undef Object_LinkObjectAndSetCallback_12
#undef Battle_WaitMode0_15
#undef BattleEv_RunWait_6
#undef Motion_SetVarCbObj_5
#undef Object_LinkObjectAndSetCallback_13
#undef Battle_WaitMode0_16
#undef BattleEv_RunWait_7
#undef Motion_EnableActCb_6
#undef Battle_WaitMode0_17
#undef Battle_WaitMode0_18
#undef Battle_WaitMode0_19
#undef BattleFx_SpawnLinked_3
#undef Battle_WaitMode0_20
#undef Battle_WaitMode0_21
#undef BattleFx_SpawnLinked_4
#undef Battle_WaitMode0_22
#undef Battle_WaitMode0_23
#undef BattleFx_SpawnLinked_5
#undef Battle_WaitMode0_24
#undef Battle_WaitMode0_25
#undef BattleFx_SpawnLinked_6
#undef Object_LinkObjectAndSetCallback_14
#undef Object_LinkObjectAndSetCallback_15
#undef Battle_WaitMode0_26
#undef Object_LinkObjectAndSetCallback_16
#undef Object_LinkObjectAndSetCallback_17
#undef Battle_WaitMode0_27
#undef Battle_WaitMode0_28
#undef BattleFx_SpawnLinked_7
#undef Motion_SetVarCb_1
#undef Motion_SetVarCb_2
#undef Motion_SetVarCbObj_6
#undef Object_LinkObjectAndSetCallback_18
#undef Object_LinkObjectAndSetCallback_19
#undef Object_SetModeById_5
#undef Object_SetModeById_6
#undef Object_SetModeById_7
#undef Battle_WaitMode0_29
#undef Motion_ResetPosMode2_2
#undef Battle_WaitMode0_30
#undef Motion_ResetPosMode2_3
#undef Battle_WaitMode0_31
#undef Motion_SetPosReset_6
#undef Motion_ResetPosMode2_4
#undef Motion_CommitPos_1
#undef Object_SetModeById_8
#undef Object_LinkObjectAndSetCallback_20
#undef Battle_WaitMode0_32
#undef Motion_SetPosReset_7
#undef Motion_ResetPosMode2_5
#undef Motion_SetSpeed_5
#undef Battle_WaitMode0_33
#undef Motion_SetHPosTerrain_6
#undef Motion_SetHPosTerrain_7
#undef Motion_SetHPosTerrain_8
#undef Battle_WaitMode0_34
#undef Battle_WaitMode0_35
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_8
#undef BattleEv_RunWait_9
#undef BattleEv_RunWait_10
#undef Battle_WaitMode0_36
#undef Battle_SchedShoulder_1
#undef SCENE_PHASE
#undef PENDING_CALLBACK_FLAG
#undef Scene_RunClosingAuxiliarySequence
#undef Scene_RunFinalAuxiliarySequence
#undef Scene_RunEarlySequence
#undef Scene_RunSupplementalSequenceOne
#undef Scene_RunMultiPhaseActorSequence

/* map/locations/colosso/log_rolling_stage/shared.c */
#define HexDigitCharacters Data_0200ce3c
#define WriteU32AsHex Func_02003c94
#define FindActorAtWholeTilePosition Func_020045e0

extern u8 Data_0200ce3c[];
extern u8 *Data_03001ebc;

void WriteU32AsHex(u8 *hex_text, u32 value)
{
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = HexDigitCharacters[value & 15];
        value >>= 4;
        hex_text--;
    }
}

s32 *FindActorAtWholeTilePosition(s32 *position)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 actor_index;

    for (actor_index = 8; actor_index <= 65; actor_index++) {
        s32 *actor = slots[actor_index];

        if ((position[0] >> 20) == (actor[2] >> 20)
            && (position[1] >> 20) == (actor[3] >> 20)
            && (position[2] >> 20) == (actor[4] >> 20)) {
            return actor;
        }
    }
    return 0;
}
#undef HexDigitCharacters
#undef WriteU32AsHex
#undef FindActorAtWholeTilePosition
