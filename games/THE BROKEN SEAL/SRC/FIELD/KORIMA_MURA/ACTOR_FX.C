/*
 * Per-frame actor and effect callbacks, and the village's work variables.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "KORIMA_MURA.H"

/* The village's own work, past the overlay image. */
s32 KorimaMura_PendingPose __attribute__((section(".bss")));
s32 KorimaMura_LayoutFlag __attribute__((section(".bss")));
s32 KorimaMura_EffectActive __attribute__((section(".bss")));

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
    s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
    s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
    s32 f60;
    u16 f64;
};

struct Ent {
    u8 pad00[6];
    u16 f06;
    s32 f08;
    u8 pad0c[4];
    s32 f10;
    u8 pad14[0x46];
    u8 f5a;
    u8 pad5b[13];
    struct Ent *f68;
};

struct Rec { u16 f00, f02, f04, f06; };

struct Ent_02000800 {
    s32 f00, f04, f08, f0c;
    u8 pad10[0x45];
    u8 f55;
};

struct Obj_020025d8 {
    s32 f00, f04, f08, f0c, f10, f14;
    s32 f18;
    s32 f1c, f20, f24, f28, f2c, f30, f34;
    s32 f38, f3c, f40;
};

struct Sub {
    u8 pad00[9];
    u8 f09;
    u8 pad0a[28];
    u8 f26;
};

struct Obj_02002608 {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

extern u8 KorimaMura_Object26Script[];

u16 ArcTan2(s32, s32);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
void BattleEffect_CleanupSceneObjects(void);

s32 SceneState_FlushPendingWordB698(s32 arg0)
{
    if (KorimaMura_PendingPose != 0) {
        Object_SetAnimation(arg0, 2);
        KorimaMura_PendingPose = 0;
    }
    return 1;
}

s32 SceneEffect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
    s32 v = Engine_RandomNext();
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        Object_SetPalette(p, 7);
    } else {
        Object_SetPalette(p, 10);
    }
    if ((s16)p->f64 > 1200) {
        p->f64 = 0;
    }
    return 1;
}

void SceneData_InitRecordTable(u8 *o)
{
    u8 *p = o + 72;
    u32 i;
    s32 normal;
    s32 special;

    i = 0;
    normal = 105;
    special = 110;
    for (; i <= 8; i++) {
        *(u16 *)p = normal;
        if ((u32)(i - 6) <= 1) {
            *(u16 *)p = special;
        }
        p[22] = 2;
        *(s32 *)(p + 4) = 1;
        p += 24;
    }
}

s32 SceneEffect_UpdateFallingObject(struct Obj *p)
{
    p->f08 += p->f24;
    p->f10 += p->f2c;
    p->f2c -= 2621;
    p->f18 += 0x600;
    p->f1c += 0x600;
    {
        s32 t = p->f64 - 1;
        p->f64 = t;
        if ((u16)t == 0) {
            Engine_ObjectDispatchRelease(p);
        }
    }
    return 1;
}

s32 SceneActor_TurnTowardTarget(struct Ent *p)
{
    struct Ent *q;
    u16 h;
    s32 t;
    s32 v;
    u8 *b;

    q = p->f68;
    if (q != 0) {
        b = &p->f5a;
        v = 0xfe;
        v &= *b;
        *b = v;
        h = ArcTan2(q->f10 - p->f10, q->f08 - p->f08);
        t = h;
        t -= p->f06;
        t <<= 16;
        t >>= 16;
        if (t != 0) {
            if (t > 0x1000) {
                t = 0x1000;
            }
            if (t < -0x1000) {
                t = -0x1000;
            }
            p->f06 = p->f06 + t;
        }
    }
    return 1;
}

#include "OBJECT_RUNTIME.H"

void Object_RefreshSelectorById();
void FieldScene_RunPairedStepA();
void FieldScene_RunPairedStepB();
void Object_SetActionCallbackAndRefreshById();
void Audio_PlayCueFromEventWork();

extern u8 KorimaMura_ActionTable1[];
extern u8 KorimaMura_ActionTable2[];
extern u8 KorimaMura_ActionTable3[];
extern u8 KorimaMura_ActionTable4[];
extern u8 KorimaMura_ActionTable5[];
extern u8 KorimaMura_ActionTable6[];
extern u8 KorimaMura_ActionTable7[];
