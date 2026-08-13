#include "types.h"

/*
 * resource_39e owner at 0x02000518, 320 bytes: a scripted beat that poses
 * actor slot 9, spawns three objects around it at three different
 * headings, waits, awards something through Func_080091c0 and sets story
 * flag 0x892.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8/r9/sl spill at
 * 0x02000518-0x02000522 through `add sp, #56 / pop {r3, r5, r6} / ... /
 * pop {r0} / bx r0` at 0x0200063a-0x02000648, then the three-word
 * literal pool 0x0200064c-0x02000657. The next owner's prologue is
 * exactly at 0x02000658. **320 bytes**, measured to the epilogue.
 *
 * Published population (sweep B): no `bl` anywhere in the image targets
 * it. All 23 `bl` sites resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 518 658`; the tool
 * reports sites=23 and 23 bl lines were transcribed, so the site
 * arithmetic agrees.
 *
 * THE THREE SPAWNS ARE THE STRUCTURE. Func_0200013c -- the byte-identical
 * multi-copy spawn+configure routine already drafted in this overlay
 * (resource_39e_c_0200013c.c) and in resource_39c/resource_3c9 -- is
 * called three times with the SAME first three arguments and only the
 * fourth changing: 0x8000, 0, 0xffff8000. Those are +90, 0 and -90
 * degrees in the engine's 16-bit angle units, so the three objects are
 * placed a quarter-turn apart. Every other argument is shared:
 * a1/a2/a3 are record[8], record[12] and record[16] + 0x40000 read from
 * the slot-9 record fetched once at the top; a5/a6 are 0; flags is
 * 0x10000; extra is a two-word stack block at sp+16 whose second word is
 * 7. Because flags lacks bit 0x100000, Func_0200013c never dereferences
 * `extra` -- it is passed and ignored on this path.
 *
 * The 56-byte stack frame exists only to hold those four stack arguments
 * plus that two-word block; there is no local array.
 *
 * The 0x40000 added to record[16] and the 0x10000 written to record[72]
 * are carried in r9 and sl across the whole body -- the same two
 * constants are reused as a spawn argument and as a record field, which
 * is why they are held in callee-saved registers rather than rebuilt.
 * Both are built by shift chains (128 << 11 and 128 << 9), never pooled.
 *
 * SHARED IDIOMS, cited not re-derived: the Func_0808a018/Func_0808a020
 * scripted-scene bracket (resource_39e_c_02001494.c); dialogue-by-id
 * Func_0808a170 (here id 0x17b4, a pool word); wait-n-frames
 * Func_0808a010(30); story-flag set Func_080770c8(0x892).
 *
 * Uncertainty: Func_080091c0's six arguments (10, 24, 1, 1, 10, 22) are
 * transcribed, not interpreted. The meaning of the constant 7 in the
 * stack block is unknown -- Func_0200013c does not read it on this
 * path, so some later consumer must.
 */

extern void Func_02000718();
extern void Func_02000732();
extern void Func_0200074c();
extern void Func_020049ca();
extern void Func_020049f8();
extern void Func_020049ea();
extern void Func_0200490c(void);
extern void Func_02004a1c();
extern u8 *Func_0200492e();
extern u8 *Func_02004968();
extern u8 *Func_02004974();
extern u8 *Func_020049a2();
extern void Func_0200499a();
extern void Func_020049b6();
extern void Func_02004980();
extern void Func_020049dc();
extern void Func_020049d2();
extern void Func_020049f4();
extern void Func_02004a74();
extern void Func_02004b86(void);
extern void Func_02004ada();
extern void Func_02004b32();

/*
 * These names describe only the behavior witnessed in this owner.  The raw
 * relocations above remain the ABI boundary; the aliases keep the executable
 * body readable without assigning undocumented game-specific identities.
 */
#define GetSceneRecord                 Func_0200492e
#define OpenSceneBracket               Func_0200490c
#define PlaySceneDialogue              Func_020049d2
#define ConfigureSceneTiming           Func_020049f4
#define ConfigureScenePosition         Func_02004980
#define ConfigureSceneHeight           Func_02004a1c
#define SetSceneCue                    Func_02004ada
#define GetActorField40Target          Func_02004968
#define GetActorOffsetTarget           Func_02004974
#define ConfigureActorState            Func_0200499a
#define SetActorAnimation              Func_020049b6
#define FinishActorSetup               Func_020049dc
#define GetActorFlagTarget             Func_020049a2
#define SetActorPresentation           Func_02004a74
#define FinishSceneCue                 Func_02004b32
#define SpawnSceneObjectPositiveTurn  Func_02000718
#define SpawnSceneObjectNeutral       Func_02000732
#define SpawnSceneObjectNegativeTurn  Func_0200074c
#define WaitSceneFrames                Func_020049ea
#define RunSceneAward                  Func_02004b86
#define AwardSceneEvent                Func_020049ca
#define SetStoryFlag                   Func_020049f8
#define CloseSceneBracket              Func_02004a1c

void Func_02000518(void)
{
    s32 spawnArgs[10];
    u8 *record;
    u8 *actor;
    s32 spawnOffset;               /* 0x40000, added to record[16] */
    s32 spawnFlags;                /* 0x10000, also written to record[72] */

    record = GetSceneRecord(9);
    OpenSceneBracket();
    PlaySceneDialogue(0x17b4);
    ConfigureSceneTiming(9, 0, 20);
    ConfigureScenePosition(0, 168, 196 << 1);
    ConfigureSceneHeight(0, 192 << 8, 20);
    SetSceneCue(132);

    actor = GetActorField40Target(9);
    *(s32 *)(actor + 40) = 160 << 13;

    actor = GetActorOffsetTarget(9);
    spawnOffset = 128 << 11;
    *(s32 *)(actor + 72) = spawnOffset;

    ConfigureActorState(9, 192 << 10, 192 << 9);
    SetActorAnimation(9, 152, 196 << 1);
    FinishActorSetup(9);

    actor = GetActorFlagTarget(9);
    spawnFlags = 128 << 9;
    *(s32 *)(actor + 72) = spawnFlags;

    SetActorPresentation(9, 0, 0);
    FinishSceneCue(132);

    spawnArgs[1] = 7;

    SpawnSceneObjectPositiveTurn(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + spawnOffset,
                  128 << 8, 0, 0, spawnFlags, (u8 *)spawnArgs);

    SpawnSceneObjectNeutral(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + spawnOffset,
                  0, 0, 0, spawnFlags, (u8 *)spawnArgs);

    SpawnSceneObjectNegativeTurn(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + spawnOffset,
                  (s32)0xffff8000, 0, 0, spawnFlags, (u8 *)spawnArgs);

    WaitSceneFrames(30);
    RunSceneAward();
    {
        s32 arg0 = 10;
        s32 arg1 = 22;
        AwardSceneEvent(10, 24, 1, 1, arg0, arg1);
    }
    SetStoryFlag(0x892);
    CloseSceneBracket();
}
