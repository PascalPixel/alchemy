#include "types.h"

#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * Resource 381's Sol Sanctum collapse scene-direction sheet.
 *
 * The tracked message archive identifies this sequence: Isaac and Garet
 * escape after Jenna and Kraden disappear, the Wise One warns that Mt. Aleph
 * will erupt, and the Elemental Star effect is returned to its bag. This
 * routine creates those actors and effects, then advances their placements,
 * animations, waits, and transitions. Coordinates use 16.16 fixed point. Calls use their
 * resolved service or overlay-local identities; old-style declarations remain
 * where differing call shapes do not justify a stronger shared ABI claim.
 *
 * The assembly has one owning entry at 0x02001410 and one return at 0x020027f4.
 * Labels reached by calls inside that span are local script continuations,
 * not separately reconstructed C functions.
 */

/* Resolved whole-owner call targets. */
extern void Func_02002820();
extern void Func_0200284c();
extern void Func_020028dc();
extern void Func_020029a4();
extern void Func_02002c1c();
extern void Func_02002c9c();
extern void Func_02002d48();
extern void Func_02002d94();
extern void Func_02002ff0();
extern void Func_02003000();
extern void Func_080000c0();
extern void Func_080000d0();
extern void Func_080000d8();
extern void * Func_08000140();
extern void Func_08000150();
extern s32 Func_080001c8();
extern void * Func_080090c8();
extern void Func_080090d0();
extern void Func_08009128();
extern void Func_080091e0();
extern void Func_080091f0();
extern void Func_080091f8();
extern void Func_08015040();
extern void Func_08015250();
extern void Func_080770c8();
extern void Func_0808a010();
extern s32 Func_0808a070();
extern void * Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a0a8();
extern void Func_0808a0b8();
extern void Func_0808a0c0();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e8();
extern void Func_0808a0f0();
extern void Func_0808a100();
extern void Func_0808a110();
extern void Func_0808a128();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a158();
extern void Func_0808a170();
extern void Func_0808a178();
extern void Func_0808a180();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a1e8();
extern void Func_0808a1f0();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void * Func_0808a228();
extern void Func_0808a248();
extern void Func_0808a330();
extern void Func_0808a348();
extern void Func_0808a390();
extern void Func_080f9010();

/*
 * Behavioral vocabulary established by this owner and the exact/semantic
 * scene siblings that call the same engine entries.  The raw addresses remain
 * above as the ABI boundary; these aliases make the long direction sheet
 * readable without guessing character or location identities.
 */
#define AdvanceTaskFrames       Func_080000c0
#define InstallFrameTask        Func_080000d0
#define RemoveFrameTask         Func_080000d8
#define SetActorRecordPresentation Func_080091e0
#define SetCameraZoom           Func_080091f0
#define WaitSceneFrames          Func_0808a010
#define QueryActorCondition      Func_0808a070
#define GetActorRecord           Func_0808a080
#define SetActorScale            Func_0808a090
#define PlaceActor               Func_0808a0f0
#define SetActorSlotPresentation Func_0808a100
#define RequestActorPresentation Func_0808a110
#define ConfigureActorAnimation Func_0808a128
#define SetActorMode             Func_0808a138
#define ShowSceneDialogue        Func_0808a170
#define SelectDialogueVariant    Func_0808a178
#define WaitForActorAction       Func_0808a180
#define WaitForActorActionFrames Func_0808a188
#define MoveActor                Func_0808a1b8
#define SetActorPresentation     Func_0808a1e0
#define PlayActorAnimation       Func_0808a1e8
#define SetActorPose             Func_0808a1f0
#define SetCameraRates           Func_0808a208
#define SetCameraTarget          Func_0808a210
#define CommitCameraMove         Func_0808a218
#define GetSceneCameraRecord     Func_0808a228
#define InitializeScenePairActor Func_02002820
#define RenderFirstWipeStep      Func_0200284c
#define RenderSecondWipeStep     Func_020028dc
#define ArrangeElementalStars    Func_020029a4
#define BuildSceneTileGrid       Func_02002c9c
#define ConfigureSceneTransition Func_02002d48
#define UpdateActorEffectFrame   Func_02002d94
#define SetStoryFlag             Func_080770c8
#define PlaySoundCue             Func_080f9010

typedef struct {
    u8 unknown000[0x40C];
    s32 cameraTransitionEnabled;
} SceneWorkspace;

/* ABI entry for RunResource381SceneDirection. */
void Func_02001410(void) {
    s32 effectResource;
    SceneWorkspace *sceneWorkspace = *(SceneWorkspace **)0x03001EC4;
    void *wiseOne = GetActorRecord(0xF);
    s32 actorApproachStep;
    s32 firstWipeDelay;
    s32 secondWipeDelay;
    s32 thirdWipeDelay;
    u32 finalEffectFrame;
    u32 openingFrame;
    u32 firstEffectWarmupFrame;
    u32 pulseFrame;
    u32 secondEffectWarmupFrame;
    u32 thirdEffectWarmupFrame;
    u8 extraActorIndex;
    void *cameraRecord;
    void *isaacHiddenRecordB;
    void *garetHiddenRecordB;
    void *isaacVisibleRecordB;
    void *garetVisibleRecordB;
    void *isaacFlagsRecord;
    void *garetFlagsRecord;
    void *isaacHiddenRecordA;
    void *garetHiddenRecordA;
    void *isaacVisibleRecordA;
    void *garetVisibleRecordA;
    void *isaacRecordForStar;
    void *elementalStarEffect;
    void *garet;
    void *elementalStarSprite;
    void *garetSprite;
    void *isaac;
    void *isaacSprite;

    /* Establish the shared scene workspace and the long-lived Wise One record. */
    RemoveFrameTask(0x0200935D);
    *(s32 *)0x0200BB68 = 3;
    WaitSceneFrames(0x50);
    PlaySoundCue(0x11);
    Func_0808a330(0x7FFF, 0);
    Func_0808a348(0x28);
    WaitSceneFrames(0x28);
    extraActorIndex = 0;
    RemoveFrameTask(0x020090C5);
    do {
        PlaceActor(extraActorIndex + 0x10, 0, 0);
        extraActorIndex += 1;
    } while ((u32) extraActorIndex <= 0xFU);
    AdvanceTaskFrames(1);
    PlaceActor(0xF, 0, 0);
    InitializeScenePairActor(0);
    InitializeScenePairActor(1);
    sceneWorkspace->cameraTransitionEnabled = 0;
    SetCameraZoom(0x10000, 0x10000, 0x10000);
    WaitSceneFrames(0x50);
    BuildSceneTileGrid();
    openingFrame = 0;
    cameraRecord = GetSceneCameraRecord();
    FIELD(cameraRecord, s8 *, 0x55) = 0;
    FIELD(cameraRecord, s32 *, 8) = 0xE70000;
    FIELD(cameraRecord, s32 *, 0x10) = 0x900000;
    FIELD(cameraRecord, s32 *, 0x38) = 0x80000000;
    FIELD(cameraRecord, s32 *, 0x3C) = 0x80000000;
    FIELD(cameraRecord, s32 *, 0x40) = 0x80000000;
    FIELD(cameraRecord, s32 *, 0xC) = 0;
    FIELD(cameraRecord, s32 *, 0x24) = 0;
    FIELD(cameraRecord, s32 *, 0x2C) = 0;
    AdvanceTaskFrames(4);
    Func_08009128();
    AdvanceTaskFrames(4);
    SetActorPresentation(0, 3);
    SetActorPresentation(1, 3);
    Func_0808a330(0x10000, 0);
    Func_0808a348(0x28);
    WaitSceneFrames(0x28);
    isaac = GetActorRecord(0);
    garet = GetActorRecord(1);
    isaacSprite = FIELD(isaac, void **, 0x50);
    garetSprite = FIELD(garet, void **, 0x50);
    actorApproachStep = 0x6000;
    do {
        FIELD(isaacSprite, u16 *, 0x1E) = (u16) (FIELD(isaacSprite, u16 *, 0x1E) + 0x100);
        FIELD(garetSprite, u16 *, 0x1E) = (u16) (FIELD(garetSprite, u16 *, 0x1E) + 0xFFFFFF00);
        FIELD(isaac, s32 *, 8) = (s32) (FIELD(isaac, s32 *, 8) + actorApproachStep);
        FIELD(garet, s32 *, 8) = (s32) (FIELD(garet, s32 *, 8) - actorApproachStep);
        AdvanceTaskFrames(1);
        openingFrame += 1;
    } while (openingFrame <= 0x13U);
    WaitSceneFrames(0x28);
    SetActorScale(0, 0x20000, 0x10000);
    SetActorScale(1, 0x20000, 0x10000);
    FIELD(FIELD(GetActorRecord(0), void **, 0x50), s16 *, 0x1E) = 0;
    FIELD(FIELD(GetActorRecord(1), void **, 0x50), s16 *, 0x1E) = 0;
    ConfigureActorAnimation(0, 6, 0);
    ConfigureActorAnimation(1, 6, 0);
    Func_0808a0b8(0, 0xF6, 0x96);
    Func_0808a0c0(1, 0xDC, 0x96);
    SetActorPresentation(0, 2);
    SetActorPresentation(1, 2);
    isaacFlagsRecord = GetActorRecord(0);
    FIELD(isaacFlagsRecord, u8 *, 0x23) = (u8) (FIELD(isaacFlagsRecord, u8 *, 0x23) | 1);
    garetFlagsRecord = GetActorRecord(1);
    FIELD(garetFlagsRecord, u8 *, 0x23) = (u8) (1 | FIELD(garetFlagsRecord, u8 *, 0x23));
    Func_0808a0a8(0);
    Func_0808a0a8(1);
    MoveActor(0, 0x2000, 0);
    WaitSceneFrames(0x14);
    MoveActor(1, 0xE000, 0);
    WaitSceneFrames(0x28);
    MoveActor(0, 0x9000, 0);
    WaitSceneFrames(0x28);
    MoveActor(1, 0x5000, 0);
    WaitSceneFrames(0x50);
    MoveActor(0, 0x8000, 0);
    WaitSceneFrames(0xA);
    MoveActor(1, 0x1000, 0);
    WaitSceneFrames(0x3C);
    SetActorMode(1, 2);
    /* Play the paired-actor movement and dialogue exchange. */
    ShowSceneDialogue(0x10F8);
    SelectDialogueVariant(1, 0);
    if (QueryActorCondition(0, 0) == 0) {
        RequestActorPresentation(1, 3);
        ShowSceneDialogue(0x10F9);
    } else {
        SetActorMode(1, 1);
        ShowSceneDialogue(0x10FA);
    }
    WaitForActorActionFrames(1, 0, 0x3C);
    MoveActor(1, 0x3000, 0);
    WaitSceneFrames(0x28);
    ConfigureActorAnimation(1, 2, 0);
    PlayActorAnimation(1, 0x102, 0);
    WaitSceneFrames(0x28);
    ShowSceneDialogue(0x10FB);
    WaitForActorActionFrames(1, 0, 0xA);
    ConfigureActorAnimation(0, 2, 0);
    WaitSceneFrames(0xA);
    MoveActor(0, 0x2000, 0);
    WaitSceneFrames(0x3C);
    PlayActorAnimation(0, 0x102, 0);
    WaitSceneFrames(0x50);
    MoveActor(1, 0x1000, 0);
    WaitSceneFrames(0xA);
    SetActorMode(1, 2);
    SelectDialogueVariant(1, 0);
    if (QueryActorCondition(0, 0) == 0) {
        WaitSceneFrames(0xA);
        SetActorMode(1, 2);
        WaitSceneFrames(0xA);
        MoveActor(1, 0x3000, 0);
        WaitSceneFrames(0x3C);
        MoveActor(1, 0x1000, 0);
        WaitSceneFrames(0xA);
        SetActorSlotPresentation(1, 4);
        ShowSceneDialogue(0x10FD);
    } else {
        WaitSceneFrames(0xA);
        SetActorMode(1, 2);
        WaitSceneFrames(0xA);
        MoveActor(1, 0x3000, 0);
        WaitSceneFrames(0x3C);
        MoveActor(1, 0x1000, 0);
        WaitSceneFrames(0xA);
        SetActorSlotPresentation(1, 4);
        ShowSceneDialogue(0x10FE);
    }
    WaitForActorActionFrames(1, 0, 0x28);
    SetActorSlotPresentation(0, 3);
    RequestActorPresentation(1, 3);
    WaitSceneFrames(0x28);
    MoveActor(0, 0x8000, 0);
    MoveActor(1, 0x7000, 0);
    SetCameraRates(0x20000, 0x4000);
    SetCameraTarget(0x011C0000, -1, 0x5C0000, 1);
    CommitCameraMove();
    WaitSceneFrames(0x14);
    MoveActor(0, 0x6000, 0);
    MoveActor(1, 0x5000, 0);
    SetCameraRates(0x18000, 0x3000);
    SetCameraTarget(0x7F0000, -1, 0xA20000, 1);
    CommitCameraMove();
    WaitSceneFrames(0x28);
    MoveActor(0, 0x2000, 0);
    MoveActor(1, 0x3000, 0);
    SetCameraRates(0x40000, 0x8000);
    SetCameraTarget(0x01300000, -1, 0x01260000, 1);
    CommitCameraMove();
    WaitSceneFrames(0x14);
    MoveActor(0, 0, 0);
    MoveActor(1, 0x1000, 0);
    SetCameraRates(0x10000, 0x2000);
    SetCameraTarget(0x01900000, -1, 0xD70000, 1);
    CommitCameraMove();
    WaitSceneFrames(0x3C);
    SetCameraRates(0x40000, 0x8000);
    SetCameraTarget(0x01110000, -1, 0x910000, 1);
    CommitCameraMove();
    WaitSceneFrames(0x14);
    SetActorMode(1, 2);
    ShowSceneDialogue(0x10FF);
    WaitForActorActionFrames(1, 0, 0x14);
    MoveActor(1, 0x3000, 0);
    WaitSceneFrames(0x28);
    MoveActor(1, 0xD000, 0);
    WaitSceneFrames(0x14);
    MoveActor(1, 0xF000, 0);
    WaitSceneFrames(0x3C);
    MoveActor(1, 0x3000, 0);
    WaitSceneFrames(0x28);
    MoveActor(1, 0xF000, 0);
    WaitSceneFrames(0xA);
    WaitForActorActionFrames(1, 0, 0xA);
    RequestActorPresentation(0, 3);
    WaitSceneFrames(0xA);
    PlaySoundCue(0x17);
    /* Change presentation scale and introduce the Wise One into the scene. */
    ConfigureSceneTransition(1, 4, 0);
    sceneWorkspace->cameraTransitionEnabled = 0;
    SetCameraZoom(0x50000, 0x50000, 0x10000);
    WaitSceneFrames(0xA);
    ConfigureSceneTransition(0, 0x28, 0);
    SetActorScale(0, 0x20000, 0x10000);
    SetActorScale(1, 0x20000, 0x10000);
    ConfigureActorAnimation(0, 6, 0);
    ConfigureActorAnimation(1, 6, 0);
    Func_0808a0b8(0, 0xF3, 0x90);
    Func_0808a0b8(1, 0xCA, 0x90);
    Func_0808a0e8(0);
    WaitSceneFrames(0x14);
    sceneWorkspace->cameraTransitionEnabled = 1;
    SetCameraZoom(0x10000, 0x10000, 0x10000);
    WaitSceneFrames(0x3C);
    PlayActorAnimation(0, 0x102, 0);
    PlayActorAnimation(1, 0x102, 0);
    WaitSceneFrames(0x50);
    SetActorScale(0, 0x6666, 0x3333);
    SetActorScale(1, 0x6666, 0x3333);
    Func_0808a0c8(1, 0xDC, 0x96);
    Func_0808a0d0(0, 0xF6, 0x96);
    SetActorSlotPresentation(1, 1);
    MoveActor(0, 0x8000, 0);
    MoveActor(1, 0x7000, 0);
    WaitSceneFrames(0x3C);
    SetActorMode(1, 2);
    WaitSceneFrames(0xA);
    MoveActor(1, 0x3000, 0);
    WaitSceneFrames(0x28);
    SelectDialogueVariant(1, 0);
    if (QueryActorCondition(0, 0) == 0) {
        MoveActor(1, 0x1000, 0);
        WaitSceneFrames(0xA);
        RequestActorPresentation(1, 3);
    } else {
        MoveActor(1, 0x1000, 0);
        WaitSceneFrames(0xA);
        SetActorSlotPresentation(1, 4);
        ShowSceneDialogue(0x1103);
    }
    WaitSceneFrames(0x14);
    WaitForActorAction(1, 0);
    PlaceActor(0xF, 0x6E0000, 0x980000);
    AdvanceTaskFrames(1);
    SetActorScale(0xF, 0x13333, 0x9999);
    Func_0808a0b8(0xF, 0xAB, 0x98);
    MoveActor(0xF, 0, 0);
    SetActorRecordPresentation(GetActorRecord(0xF), 1);
    SetActorScale(1, 0x10000, 0x8000);
    Func_0808a0c8(1, 0xD9, 0xB6);
    MoveActor(0, 0x7000, 0);
    WaitSceneFrames(0xA);
    SetCameraRates(0x20000, 0x4000);
    SetCameraTarget(0xD90000, -1, 0xB00000, 1);
    CommitCameraMove();
    WaitSceneFrames(0x14);
    SetActorPose(0, 0x102);
    WaitSceneFrames(0x14);
    ConfigureActorAnimation(0, 2, 0);
    WaitSceneFrames(0xA);
    ConfigureActorAnimation(0, 4, 0);
    WaitSceneFrames(0x1E);
    PlayActorAnimation(1, 0x100, 0);
    SetActorSlotPresentation(1, 1);
    WaitSceneFrames(0x28);
    MoveActor(1, 0x2000, 0);
    WaitSceneFrames(4);
    Func_0808a0d0(1, 0xE7, 0xAF);
    MoveActor(1, 0xE000, 0);
    ShowSceneDialogue(0x1104);
    WaitForActorAction(1, 0);
    WaitSceneFrames(0x28);
    PlayActorAnimation(1, 0x101, 0);
    WaitSceneFrames(0x28);
    MoveActor(1, 0xA000, 0);
    WaitSceneFrames(0x3C);
    Func_0808a130(1, 1);
    SetActorPose(1, 0x102);
    WaitSceneFrames(0x28);
    ConfigureSceneTransition(1, 0x14, 0);
    sceneWorkspace->cameraTransitionEnabled = 0;
    SetCameraZoom(0x10000, 0x10000, 0x10000);
    WaitSceneFrames(0x28);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    PlaySoundCue(0x121);
    Func_0808a330(0x10000, 1);
    Func_0808a348(0x28);
    sceneWorkspace->cameraTransitionEnabled = 1;
    SetCameraZoom(-1, -1, 0xE666);
    WaitSceneFrames(0x78);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x14);
    PlayActorAnimation(0, 0x102, 0);
    PlayActorAnimation(1, 0x102, 0);
    WaitSceneFrames(0x64);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x14);
    ConfigureSceneTransition(1, 0xA, 0);
    sceneWorkspace->cameraTransitionEnabled = 0;
    SetCameraZoom(0x20000, 0x20000, 0x10000);
    WaitSceneFrames(0x14);
    SetActorScale(0, 0x20000, 0x10000);
    SetActorScale(1, 0x20000, 0x10000);
    isaacHiddenRecordA = GetActorRecord(0);
    FIELD(isaacHiddenRecordA, u8 *, 0x5A) = (u8) (0xFE & FIELD(isaacHiddenRecordA, u8 *, 0x5A));
    garetHiddenRecordA = GetActorRecord(1);
    FIELD(garetHiddenRecordA, u8 *, 0x5A) = (u8) (0xFE & FIELD(garetHiddenRecordA, u8 *, 0x5A));
    ConfigureActorAnimation(0, 4, 0);
    ConfigureActorAnimation(1, 4, 0);
    Func_0808a0b8(0, 0x100, 0x96);
    Func_0808a0b8(1, 0xE7, 0xB4);
    Func_0808a0e8(1);
    ConfigureSceneTransition(0, 0x28, 0);
    WaitSceneFrames(0x14);
    isaacVisibleRecordA = GetActorRecord(0);
    FIELD(isaacVisibleRecordA, u8 *, 0x5A) = (u8) (FIELD(isaacVisibleRecordA, u8 *, 0x5A) | 1);
    garetVisibleRecordA = GetActorRecord(1);
    FIELD(garetVisibleRecordA, u8 *, 0x5A) = (u8) (1 | FIELD(garetVisibleRecordA, u8 *, 0x5A));
    Func_0808a130(0, 2);
    SetActorMode(1, 2);
    WaitSceneFrames(0x28);
    PlayActorAnimation(1, 0x103, 0);
    WaitSceneFrames(0x28);
    WaitForActorAction(1, 0);
    WaitSceneFrames(0x14);
    PlayActorAnimation(0xF, 0x101, 0);
    WaitSceneFrames(0x3C);
    MoveActor(0, 0x5000, 0);
    MoveActor(1, 0xD000, 0);
    WaitSceneFrames(0x14);
    MoveActor(0, 0x8000, 0);
    MoveActor(1, 0xB000, 0);
    WaitSceneFrames(0xA);
    PlaySoundCue(0x6B);
    InstallFrameTask(0x0200A93D, 0xC80);
    WaitSceneFrames(0xA);
    PlayActorAnimation(0xF, 0x100, 0);
    WaitSceneFrames(0x28);
    SetCameraRates(0x10000, 0x2000);
    SetCameraTarget(0xBA0000, -1, 0xA60000, 1);
    Func_0808a0b8(0xF, 0x82, 0x71);
    Func_0808a0e8(0xF);
    MoveActor(0xF, 0x3000, 0);
    WaitSceneFrames(0x14);
    sceneWorkspace->cameraTransitionEnabled = 0;
    SetCameraZoom(0x20000, 0x20000, 0x10000);
    Func_0808a330(0x20119E, 1);
    Func_0808a348(0x14);
    WaitSceneFrames(0x14);
    Func_02002ff0();
    RequestActorPresentation(0xF, 2);
    firstEffectWarmupFrame = 0;
    do {
        UpdateActorEffectFrame(wiseOne);
        firstEffectWarmupFrame += 1;
        AdvanceTaskFrames(1);
    } while (firstEffectWarmupFrame <= 0x27U);
    InstallFrameTask(0x0200B00D, 0xC80);
    Func_0808a330(0x10000, 1);
    Func_0808a348(0x3C);
    WaitSceneFrames(0x1E);
    PlaySoundCue(0x121);
    RemoveFrameTask(0x0200A93D);
    SetCameraZoom(0x10000, 0x10000, 0x10000);
    /* Close the Wise One's first effect through the 16-step tile wipe. */
    firstWipeDelay = 0xF;
    do {
        RenderFirstWipeStep(0);
        AdvanceTaskFrames(firstWipeDelay);
        RenderFirstWipeStep(1);
        AdvanceTaskFrames(firstWipeDelay);
        firstWipeDelay -= 1;
    } while (firstWipeDelay != -1);
    RenderFirstWipeStep(0);
    sceneWorkspace->cameraTransitionEnabled = 1;
    SetCameraZoom(firstWipeDelay, firstWipeDelay, 0xE666);
    Func_080091f8();
    RequestActorPresentation(0xF, 3);
    RemoveFrameTask(0x0200B00D);
    AdvanceTaskFrames(1);
    Func_0808a158(0xF, 0);
    WaitSceneFrames(0x3C);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x28);
    PlayActorAnimation(0, 0x102, 0);
    PlayActorAnimation(1, 0x102, 0);
    WaitSceneFrames(0x3C);
    SetCameraRates(0x10000, 0x2000);
    SetCameraTarget(0xDA0000, firstWipeDelay, 0xB50000, 1);
    SetActorScale(0xF, 0x10000, 0x8000);
    Func_0808a0b8(0xF, 0xA9, 0x97);
    Func_0808a0e8(0xF);
    WaitSceneFrames(0xA);
    MoveActor(0xF, 0x1000, 0);
    WaitSceneFrames(0x28);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x14);
    MoveActor(0, 0x5000, 0);
    MoveActor(1, 0xD000, 0);
    WaitSceneFrames(0x28);
    MoveActor(0xF, 0xF000, 0);
    WaitSceneFrames(0xA);
    MoveActor(0, 0x7000, 0);
    MoveActor(1, 0xB000, 0);
    SetCameraTarget(0xE00000, firstWipeDelay, 0x9E0000, 1);
    CommitCameraMove();
    /* Run the four Elemental Star arrangements around the pulsing star. */
    ArrangeElementalStars(0);
    SetActorMode(0xF, 2);
    WaitSceneFrames(0x14);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x28);
    SelectDialogueVariant(0xF, 0);
    if (QueryActorCondition(0, 0) == 0) {
        WaitSceneFrames(0x28);
    } else {
        Func_08015040(0x110C, 1);
        WaitSceneFrames(0x28);
    }
    isaacRecordForStar = GetActorRecord(0);
    elementalStarEffect = Func_080090c8(0x16, FIELD(isaacRecordForStar, s32 *, 8), FIELD(isaacRecordForStar, s32 *, 0xC) + 0x240000, FIELD(isaacRecordForStar, s32 *, 0x10));
    if (elementalStarEffect != NULL) {
        effectResource = Func_08000140(0x11, 0x608);
        elementalStarSprite = FIELD(elementalStarEffect, void **, 0x50);
        FIELD(elementalStarSprite, s8 *, 0x26) = 0;
        FIELD((elementalStarSprite + 0x26), s8 *, 1) = 0;
        FIELD(elementalStarSprite, u8 *, 5) = (u8) (-0x21 & FIELD(elementalStarSprite, u8 *, 5));
        FIELD(elementalStarSprite, u8 *, 9) = (u8) ((0xF & FIELD(elementalStarSprite, u8 *, 9) & ~0xC) | 4);
        Func_08015250(0xDE);
        Func_080001c8(FIELD(elementalStarSprite, u8 *, 0x1C), 0x80, effectResource + 0x400);
        Func_08000150(0x11);
        SetActorSlotPresentation(0, 0x1C);
        Func_0808a390(elementalStarEffect, 3);
        SetActorSlotPresentation(0, 0x1C);
    }
    ConfigureSceneTransition(1, 0x14, 0);
    pulseFrame = 0;
    do {
        UpdateActorEffectFrame(wiseOne);
        AdvanceTaskFrames(1);
        UpdateActorEffectFrame(wiseOne);
        AdvanceTaskFrames(1);
        FIELD(elementalStarEffect, s32 *, 0x18) = 0x6666;
        FIELD(elementalStarEffect, s32 *, 0x1C) = 0x6666;
        UpdateActorEffectFrame(wiseOne);
        AdvanceTaskFrames(1);
        UpdateActorEffectFrame(wiseOne);
        pulseFrame += 1;
        AdvanceTaskFrames(1);
        FIELD(elementalStarEffect, s32 *, 0x18) = 0x10000;
        FIELD(elementalStarEffect, s32 *, 0x1C) = 0x10000;
    } while (pulseFrame <= 0x17U);
    Func_0808a158(0xF, 0);
    ConfigureSceneTransition(0, 0x14, 0);
    ShowSceneDialogue(0x110D);
    WaitForActorActionFrames(0xF, 0, 0x14);
    if (elementalStarEffect != NULL) {
        Func_080090d0(elementalStarEffect);
    }
    SetActorSlotPresentation(0, 1);
    WaitSceneFrames(0x14);
    MoveActor(0, 0x8000, 0);
    MoveActor(1, 0xA000, 0x3C);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x28);
    MoveActor(0xF, 0x4000, 0);
    SetCameraRates(0x40000, 0x8000);
    Func_02002c1c(0xE8, 0x1D0);
    ArrangeElementalStars(1);
    WaitForActorAction(0xF, 0);
    Func_02002c1c(0x2C7, 0x90);
    ArrangeElementalStars(2);
    WaitForActorAction(0xF, 0);
    Func_02002c1c(0x2C7, 0x1D0);
    ArrangeElementalStars(3);
    WaitForActorAction(0xF, 0);
    MoveActor(0xF, 0x1000, 0);
    PlaceActor(1, 0x02460000, 0x01590000);
    WaitSceneFrames(0x14);
    WaitForActorAction(1, 0);
    PlaceActor(1, 0xE70000, 0xB40000);
    MoveActor(1, 0xB000, 0);
    AdvanceTaskFrames(0x14);
    Func_02002c1c(0xDB, 0xAB);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0xA);
    Func_0808a130(0, 2);
    SetActorMode(1, 2);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x14);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x28);
    sceneWorkspace->cameraTransitionEnabled = 0;
    SetCameraZoom(0x40000, 0x40000, 0x10000);
    Func_0808a330(0x20119E, 1);
    Func_0808a348(0x14);
    WaitSceneFrames(0x14);
    PlaySoundCue(0x6B);
    InstallFrameTask(0x0200A971, 0xC80);
    WaitSceneFrames(0x14);
    SetCameraRates(0x10000, 0x2000);
    SetCameraTarget(0xB80000, -1, 0x840000, 1);
    ConfigureActorAnimation(0, 6, 0);
    ConfigureActorAnimation(1, 6, 0);
    isaacHiddenRecordB = GetActorRecord(0);
    FIELD(isaacHiddenRecordB, u8 *, 0x5A) = (u8) (0xFE & FIELD(isaacHiddenRecordB, u8 *, 0x5A));
    garetHiddenRecordB = GetActorRecord(1);
    FIELD(garetHiddenRecordB, u8 *, 0x5A) = (u8) (0xFE & FIELD(garetHiddenRecordB, u8 *, 0x5A));
    Func_0808a0b8(0, 0xF5, 0x91);
    Func_0808a0b8(1, 0xD7, 0xA8);
    Func_0808a0e8(1);
    WaitSceneFrames(0x50);
    isaacVisibleRecordB = GetActorRecord(0);
    FIELD(isaacVisibleRecordB, u8 *, 0x5A) = (u8) (FIELD(isaacVisibleRecordB, u8 *, 0x5A) | 1);
    garetVisibleRecordB = GetActorRecord(1);
    FIELD(garetVisibleRecordB, u8 *, 0x5A) = (u8) (1 | FIELD(garetVisibleRecordB, u8 *, 0x5A));
    Func_0808a0b8(0xF, 0xB8, 0x57);
    Func_0808a0e8(0xF);
    MoveActor(0xF, 0x4000, 0);
    WaitSceneFrames(0x14);
    RequestActorPresentation(0xF, 2);
    secondEffectWarmupFrame = 0;
    do {
        UpdateActorEffectFrame(wiseOne);
        secondEffectWarmupFrame += 1;
        AdvanceTaskFrames(1);
    } while (secondEffectWarmupFrame <= 0x27U);
    InstallFrameTask(0x0200B00D, 0xC80);
    Func_0808a330(0x10000, 1);
    Func_0808a348(0x3C);
    WaitSceneFrames(0x1E);
    PlaySoundCue(0x121);
    RemoveFrameTask(0x0200A971);
    SetCameraZoom(0x20000, 0x20000, 0x10000);
    /* Alternate the second tile family over an eight-step wipe. */
    secondWipeDelay = 7;
    do {
        RenderSecondWipeStep(0);
        AdvanceTaskFrames(secondWipeDelay);
        RenderSecondWipeStep(1);
        AdvanceTaskFrames(secondWipeDelay);
        secondWipeDelay -= 1;
    } while (secondWipeDelay != -1);
    RenderSecondWipeStep(0);
    SetCameraZoom(0x10000, 0x10000, 0x10000);
    RequestActorPresentation(0xF, 3);
    RemoveFrameTask(0x0200B00D);
    AdvanceTaskFrames(1);
    Func_0808a158(0xF, 0);
    WaitSceneFrames(0x3C);
    SetCameraZoom(0x40000, 0x40000, 0x10000);
    Func_0808a330(0x20119E, 1);
    Func_0808a348(0x14);
    WaitSceneFrames(0x14);
    PlaySoundCue(0x6B);
    InstallFrameTask(0x0200A93D, 0xC80);
    WaitSceneFrames(0x28);
    Func_0808a0c0(0xF, 0x7F, 0x6E);
    MoveActor(0xF, 0x4000, 0);
    WaitSceneFrames(0x14);
    RequestActorPresentation(0xF, 2);
    thirdEffectWarmupFrame = 0;
    do {
        UpdateActorEffectFrame(wiseOne);
        thirdEffectWarmupFrame += 1;
        AdvanceTaskFrames(1);
    } while (thirdEffectWarmupFrame <= 0x27U);
    InstallFrameTask(0x0200B00D, 0xC80);
    Func_0808a330(0x10000, 1);
    Func_0808a348(0x3C);
    PlaySoundCue(0x121);
    WaitSceneFrames(0x1E);
    RemoveFrameTask(0x0200A93D);
    SetCameraZoom(0x20000, 0x20000, 0x10000);
    /* Restore the first tile family with the matching eight-step wipe. */
    thirdWipeDelay = 7;
    do {
        RenderFirstWipeStep(0);
        AdvanceTaskFrames(thirdWipeDelay);
        RenderFirstWipeStep(1);
        AdvanceTaskFrames(thirdWipeDelay);
        thirdWipeDelay -= 1;
    } while (thirdWipeDelay != -1);
    RenderFirstWipeStep(0);
    SetCameraZoom(0x10000, 0x10000, 0x10000);
    PlaySoundCue(0x6B);
    PlaySoundCue(0x3F);
    SetCameraZoom(0x40000, 0x40000, 0x10000);
    Func_0808a330(0x20119E, 1);
    Func_0808a348(0x14);
    WaitSceneFrames(0x14);
    PlaySoundCue(0x6B);
    InstallFrameTask(0x0200A971, 0xC80);
    RequestActorPresentation(0xF, 3);
    RemoveFrameTask(0x0200B00D);
    AdvanceTaskFrames(1);
    Func_0808a158(0xF, 0);
    WaitSceneFrames(0x3C);
    Func_0808a0c0(0xF, 0xB8, 0x57);
    MoveActor(0xF, 0x4000, 0);
    WaitSceneFrames(0xA);
    RequestActorPresentation(0xF, 3);
    RemoveFrameTask(0x0200B00D);
    AdvanceTaskFrames(1);
    Func_0808a158(0xF, 0);
    PlaySoundCue(0x8D);
    WaitSceneFrames(0x64);
    PlayActorAnimation(0, 0x102, 0);
    PlayActorAnimation(1, 0x102, 0);
    WaitSceneFrames(0x3C);
    WaitForActorAction(0xF, 0);
    SetActorMode(1, 3);
    WaitForActorAction(1, 0);
    WaitSceneFrames(0x14);
    PlaySoundCue(0x121);
    MoveActor(0xF, 0x3000, 0);
    WaitSceneFrames(0x14);
    WaitForActorAction(0xF, 0);
    WaitSceneFrames(0x14);
    finalEffectFrame = 0;
    do {
        UpdateActorEffectFrame(wiseOne);
        finalEffectFrame += 1;
        AdvanceTaskFrames(1);
    } while (finalEffectFrame <= 0x27U);
    InstallFrameTask(0x0200B00D, 0xC80);
    WaitSceneFrames(0x14);
    Func_0808a330(0x7FFF, 2);
    Func_0808a348(0x3C);
    AdvanceTaskFrames(0x64);
    Func_0808a330(0x7FFF, 1);
    Func_0808a348(0x3C);
    AdvanceTaskFrames(0x3C);
    RemoveFrameTask(0x0200B00D);
    sceneWorkspace->cameraTransitionEnabled = 1;
    SetCameraZoom(-1, -1, 0xE666);
    Func_080091f8();
    Func_02003000();
    /* Commit the three story flags only after every scene task is removed. */
    SetStoryFlag(0x814);
    SetStoryFlag(0x83F);
    Func_0808a248(5);
    SetStoryFlag(0x100);
}
