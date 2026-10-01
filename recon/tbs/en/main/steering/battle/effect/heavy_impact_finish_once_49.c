/* NONMATCHING: 2026-10-01 brief Wave2 one-device attempt.
 * BattleFx_FinishHeavyImpact: removing the do-once at source line 49 changes
 * first changed instruction: mov r0, r6 => mov r1, #1; 131/131 assembly lines.
 * The production source retains and tags this scheduling boundary.
 * Other functions in this unit are unchanged from the current source.
 */
#include "TYPES.H"
#include "FIXED_MATH.H"

extern u8 gEffectWork[];

void BattleFx_AdvanceSpinAngle(void);
void BattleFx_RunAngledApproachPhases(void);

extern s32 gGameState[];
int BattleFx_ClearActiveSlotsAndScheduleUpdates();
int ObjectMotion_ArmCallback();
int ObjectMotion_Launch();

void BattleFx_FinishHeavyImpact(s32 arg)
{
    s8 *head;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;
    s32 value;
    s8 *zptr;
    s32 base;
    s32 id;
    s32 count;
    void *ctx;
    void *work;

    id = arg;
    ctx = Object_GetById();
    /* The single-pass block preserves the first call's argument schedule. */
    do
    {
        if (ctx == ((void *)0))
        {
            return;
        }
        BattleFx_InitializeSlots();
        base = (*((s32 *)gEffectWork));
        Unnamed_080b0840Far(0x20118C);
        Audio_PlayCue(0xAD);
        Motion_SetVarCbAndRefresh(id, 1);
    }
    while (0);
    Audio_PlayCue(0xAE);
    Motion_SetVarCbAndRefresh(id, 1);
    /* A second boundary gives the third repeated call its observed order. */
    
    Audio_PlayCue(0xAF);
    Motion_SetVarCbAndRefresh(id, 1);
    WaitFrames(0x14);
    Audio_PlayCue(0x8C);
    (*((s32 *)(((s8 *)ctx) + 0x6C))) = (s32)BattleFx_AdvanceSpinAngle;
    WaitFrames(0x28);
    Audio_PlayCue(0x99);
    ObjectMotion_Launch(id, 0xC, 0x16);
    pos.x = ((s32)(*((s32 *)(((s8 *)ctx) + 8))));
    pos.y = ((s32)(*((s32 *)(((s8 *)ctx) + 0xC))));
    pos.z = ((s32)(*((s32 *)(zptr = (((s8 *)ctx) + 0x10)))));
    Camera_WorldToScreen(&pos);
    Object_Destroy(ctx);
    Audio_PlayCue(0xA4);
    work = (base + 0x58);
    count = 0x17;
    do
    {
        EffectSlot_Initialize(work, 0x11C, pos.x, pos.z);
        EffectSlot_SetCallback(work, (s32)BattleFx_RunAngledApproachPhases);
        /* This boundary keeps the work pointer ahead of the constant. */
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
do
        {
        EffectSlot_SetObjectMode(work, 7);
        }
        while (0);
        ObjectGroup_SetChildValueUnlessFifteenFar(*((s32 *)(head = (((s8 *)work) + 0))), ((u32)(Random16() * 7)) >> 0x10);
        value = ((u32)Random16() / 3 + 0x10000);
        (*((s32 *)(((s8 *)work) + 0x2C))) = value;
        (*((s32 *)(((s8 *)work) + 0x28))) = value;
        count = (count - 1);
        WaitFrames(1);
        work += 0x48;
    }
    while (count >= 0);

    WaitFrames(0x3C);
    ObjectMotion_ArmCallback(gGameState[125], 0x4000, 0);
    WaitFrames(0x14);
    Object_SetMode(Object_GetById(gGameState[125]), 0x1C);
    WaitFrames(0x28);
    Audio_PlayCue(0xA4);
    WaitFrames(0x64);
    Shop_InitEffectFar();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
