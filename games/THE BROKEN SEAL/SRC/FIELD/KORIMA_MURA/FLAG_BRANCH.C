/*
 * Restores the map cells flags 0x845 and 0x847 have changed.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "KORIMA_MURA.H"

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

void FieldScene_RunFlag845And847Branches(void)
{
    if (Engine_GameFlagIsSet(0x845) == 0) {
        Engine_ActorSetPosition(8, 0, 0);
        { s32 k5 = 9, k6 = 18; Engine_MapCopyCellAttributes(9, 17, 5, 1, k5, k6); }
        Engine_MapRedraw();
        Engine_TaskWait(1);
    } else {
        Engine_ActorSetPosition(9, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x847) != 0) {
        { s32 k5 = 5, k6 = 7; Engine_MapCopyCellsTo(91, 19, 72, 9, k5, k6); }
        { s32 k5 = 8, k6 = 11; Engine_MapCopyCellAttributes(23, 11, 5, 7, k5, k6); }
        Engine_MapRedraw();
        Engine_TaskWait(1);
    }
}
