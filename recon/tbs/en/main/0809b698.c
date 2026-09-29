/* 2026-09-29: the two callbacks are the build's
   BattleFx_UpdatePairedArcSpawner and BattleFx_UpdateEffect16State rather
   than literal Thumb addresses, 810 to 750. The VRAM source is now the
   labelled BattleFx_ArcSparkTiles; 16 scheduling halfwords remain (the
   opening resource store and the pulse-loop constants). */
/* 2026-09-29 alchemy permute: score 1180 to 810 on the permuter's scorer
   (0 is exact); remaining 2 register-only, 2 operand, 9 reordered, 1
   inserted, 1 deleted. Kept rewrites: 3x swap commutative operands, 2x
   reorder independent statements, 2x introduce a temporary, 1x reorder
   local declarations, 1x change loop form, 1x test truth or compare with
   zero. FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* 2026-09-28: the 20-pulse loop counts up from zero (the compiler reverses
 * it itself), which gives 364 bytes and 26 differing halfwords (was 32).
 * Remaining: the opening object/record loads and the pulse-loop constant
 * setup schedule differently. */
/* NONMATCHING: complete extent is 364 bytes (332-byte body and 32-byte
 * literal pool, previously split as 0809b7e4). Keep this 364-byte baseline.
 * The typed scene/object/child and goto-pulse experiment is preserved in
 * e12527b62: 368 bytes, 151 differing halfwords. It rematerialises loop
 * constants and does not fix resource sign extension or angle/load order.
 * Do not repeat that structural spelling without new evidence. Typed motion
 * record/angle reads and assignment inside the consuming resource call
 * also leave this baseline identical; the assignment was rejected. */
#include "TYPES.H"
#include "MOTION_OBJECT.H"

extern void WaitFrames(s32);
extern s32 Resource_ResetEntry(s32);
extern s32 VramBlock_LoadCached(s32, s32, const void *);
extern s16 Resource_FindFreeEntry(void);
extern s32 Scheduler_AddOrUpdateCallback(const void *, s32);
extern void Scheduler_RemoveCallback(const void *);
extern void ObjectDispatch_SetSingleChildField26Far(void *, s32);
extern void Animation_ApplyChildValuesFar(void *, s32);
extern void UiText_DrawMessage(s32, s32);
extern s32 GameFlag_TestFar(s32);
extern void Audio_PlayCue(s32);

extern const u8 BattleFx_ArcSparkTiles[];
extern u8 *gEffectWork;
void BattleFx_UpdateEffect16State(void);
void BattleFx_UpdatePairedArcSpawner(void);
extern u8 gGameState[];

void RunBattleEffect16(void)
{
    u8 *scene;
    u8 *object;
    u8 *group;
    u8 *entry;
    u32 saved;
    u16 value;
    s32 active;
    s32 index;
    s32 entry_mode;
    s32 count;

    scene = gEffectWork;
    object = *(u8 **)(scene + 16);
    group = ((struct MotionObject *)object)->records;
    entry = *(u8 **)(group + 40);
    saved = ((struct MotionObject *)object)->angle;
    value = Resource_FindFreeEntry();
    {
        s32 zero = 0;
        s16 *tmp;
        u8 *tmp2;
        tmp = (s16 *)(0x71a + scene);
        *tmp = value;
        VramBlock_LoadCached((s16)value, 0x100, BattleFx_ArcSparkTiles);
        index = 145;
        ((s32 *)gGameState)[index] = 0x09600000;
        index = 146;
        *(s8 *)&((s32 *)gGameState)[index] = GameFlag_TestFar(0x145);
        Animation_ApplyChildValuesFar(object, zero);
        *(void **)(object + 108) = BattleFx_UpdatePairedArcSpawner;
        *(s16 *)(object + 100) = zero;
        tmp2 = object + 102;
        *(s16 *)tmp2 = zero;
    }
    Audio_PlayCue(0x8c);
    WaitFrames(15);
    active = 1;
    *(s16 *)(object + 100) = active;
    WaitFrames(10);
    entry_mode = 7;
    count = 0;
    while (19 >= count) {
        *(s8 *)(entry + 5) = entry_mode;
        *(s8 *)(group + 37) = 1;
        WaitFrames(2);
        *(s8 *)(group + 37) = 1;
        *(s8 *)(entry + 5) = 0;
        count++;
        *(s8 *)(group + 38) = 1;
        WaitFrames(3);
    }
    *(void **)(object + 108) = 0;
    *(u16 *)(object + 6) = saved;
    Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
    WaitFrames(15);
    Audio_PlayCue(0xae);
    WaitFrames(55);
    Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
    index = 147;
    if (((s16 *)gGameState)[index * 2]) {
        ObjectDispatch_SetSingleChildField26Far(object, 2);
    } else {
        ObjectDispatch_SetSingleChildField26Far(object, 1);
    }
    Animation_ApplyChildValuesFar(object, 0);
    Resource_ResetEntry(*(s16 *)(0x71a + scene));
    UiText_DrawMessage(0x922, 1);
}
