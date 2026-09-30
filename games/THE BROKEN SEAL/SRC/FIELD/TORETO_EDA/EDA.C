#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_397.H"

extern struct EventWork *gEventWork;
extern u8 Data_03001ecc[];

/* The two tracked scene objects share this coordinate and terrain prefix. */
struct SceneObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 settled_y;
    u8 unknown_18[10];
    u8 layer;
};

/* The split threshold and both scroll values share one work-record cell. */
union SceneCell {
    s32 w;
    s16 h[2];
};

extern s32 gCell[];
extern u8 *gCam;
extern u8 *gWork;
extern u32 gFrameCount;
extern s32 gKeysHeld;
extern s32 gKeysRepeat;

/*
 * The scene's own work, after the overlay image: a transfer header and the
 * buffer it describes, and the scanline where the BG3 scroll switches between
 * the two values above and below it.
 */
static u16 sTransferHeader[16];
static u8 sTransferData[0x60];
static s32 sSplitLine;
static u16 sHofsAbove;
static u16 sHofsBelow;

/* The scene's tables, in the overlay's read-only data. */
extern u8 ToretoEda_SceneTable0[];
extern u8 ToretoEda_SceneTable1[];
extern u8 ToretoEda_SceneTable2[];
extern u8 ToretoEda_SceneTable3[];
void *Object_GetById(u32);
void BattleFx_SetPhaseRequest(s32, s32);
void ToretoEda_StartBg3Split(void);

void Effect_UpdateBg3HofsByVcount(void);
void Effect_SetBg3HofsSplit(void);
void Runtime_SetIrqHandler(s32 slot, s32 mode, void (*handler)(void));
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/*
 * Overlay resource_397: a field scene that shifts its two tracked objects by
 * whole blocks, blends the display for scene 9, and scrolls BG3 against the
 * vertical counter.
 */
void State_SetActorEightValue3d(void)
{
    BattleFx_SetPhaseRequest(8, 0x3D);
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetTable835c(void)
{
    return ToretoEda_SceneTable0;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable844c(void)
{
    return ToretoEda_SceneTable1;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable8474(void)
{
    return ToretoEda_SceneTable2;
}

void Actor_ShiftObjectsByBlock(s32 bx, s32 bz)
{
    u8 *work = *(u8 **)&gEventWork;
    struct SceneObject *obj;
    s32 dx = bx;
    s32 dz = bz;
    s32 h;

    /* Block coordinates become 16.16 fixed-point shifts of sixteen tiles. */
    obj = (struct SceneObject *)Object_GetById(gCell[125]);
    dx <<= 20;
    dz <<= 20;

    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Map_GetTerrainHeight((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }

    /* Apply the same shift to the workspace's independently optional object. */
    obj = *(struct SceneObject **)(work + 480);
    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Map_GetTerrainHeight((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }
}

void Scene_ApplyOffset0Pos5(void)
{
    Actor_ShiftObjectsByBlock(0, 5);
}

void Scene_ApplyOffset0Neg5(void)
{
    Actor_ShiftObjectsByBlock(0, -5);
}

void Scene_ApplyOffset0Pos5Second(void)
{
    Actor_ShiftObjectsByBlock(0, 5);
}

void Scene_ApplyOffset0Neg5Second(void)
{
    Actor_ShiftObjectsByBlock(0, -5);
}

void Scene_ApplyOffset0Pos6(void)
{
    Actor_ShiftObjectsByBlock(0, 6);
}

void Scene_ApplyOffset0Neg6(void)
{
    Actor_ShiftObjectsByBlock(0, -6);
}

void State_SetValue123ThenCounter16c(void)
{
    u8 *state = gWork;
    s16 *cnt;

    Audio_PlayCue(0x7B);
    cnt = (s16 *)(state + 0x16C);
    Event_SetValue170(*cnt);
}

void Effect_SetAlphaBlendForScene9(void)
{
    u8 *disp;

    /* Start the scene, then configure alpha blending for its display state. */
    DisplayTransition_InitializeBattleEffectState(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    disp = *(u8 **)Data_03001ecc;
    {
        u16 *slot = (u16 *)(disp + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x52a);
        int value = 10;
        *slot = value;
    }
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable84a4(void)
{
    return ToretoEda_SceneTable3;
}

void Scene_RunTwoCallSequence(void)
{
    Battle_Reset();
    BattleFx_FinishAction();
}

/*
 * Two scene hooks that do nothing. Each is the two-byte return alone; the
 * zero halfwords after them align the entry that follows.
 */
void State_RunEmptyHookFirst(void)
{
}

void State_RunEmptyHook(void)
{
}

void SceneData_InitHeader8590(void)
{
    u16 *hdr = sTransferHeader;

    hdr[0] = gKeysHeld;
    hdr[1] = gKeysRepeat;
    SerialRuntime_PollAndTransfer(hdr, sTransferData);
}

s32 State_SetRuntimeWord448To256(void)
{
    u8 **base = (u8 **)&gEventWork;
    u8 *work;
    u8 *disp;
    s32 off = 224;
    s32 *scene;

    /* Reset the scene word at workspace + 448 before entering scene 9. */
    off <<= 1;
    work = *base;
    scene = (s32 *)(work + off);
    off -= 192;
    *scene = off;

    DisplayTransition_InitializeBattleEffectState(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    disp = base[4];
    {
        u16 *slot = (u16 *)(disp + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x52a);
        int value = 10;
        *slot = value;
    }

    ToretoEda_StartBg3Split();
    return 0;
}

void Effect_UpdateBg3HofsByVcount(void)
{
    u16 *src;
    u32 value;

    if (*(u16 *)0x04000006 >= sSplitLine) {
        src = &sHofsAbove;
    } else {
        src = &sHofsBelow;
    }
    value = *src;
    *(u16 *)0x0400001c = value;
}

/*
 * Prepares the three values that Effect_UpdateBg3HofsByVcount consumes: a
 * VCOUNT threshold and the two BG3HOFS values selected above and below it.
 * 192 is the screen height, so the threshold is a scanline derived from a
 * coordinate in the scene work record.
 *
 * The record cell is read as a union, not as bare halfwords. The word store to
 * the threshold and the halfword reads are only ordered against each other
 * when they can alias, and the reference schedules the address load of the
 * value above ahead of the halfword read on exactly that dependence. Reading
 * the cell through a halfword-only pointer disambiguates the two accesses and
 * loses that order.
 */
void Effect_SetBg3HofsSplit(void)
{
    union SceneCell *work = (union SceneCell *)(gCam + 260);
    s32 hofs;

    sSplitLine = 192 - work[1].h[1];
    sHofsAbove = hofs = work[0].h[1];
    sHofsBelow = hofs - (gFrameCount >> 2);
}

/* Start the BG3 split: run the scroll update from the vertical-count
 * interrupt and schedule the task that prepares its values. */
void ToretoEda_StartBg3Split(void)
{
    Runtime_SetIrqHandler(1, 0, Effect_UpdateBg3HofsByVcount);
    Scheduler_AddOrUpdateCallback(Effect_SetBg3HofsSplit, 0xc80);
}
