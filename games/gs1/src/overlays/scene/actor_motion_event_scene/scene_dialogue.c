#include "types.h"

#define SceneDialogue_RunActor8FlagScene Func_020004d8
#define SceneDialogue_RunActor11Line Func_0200050c
#define SceneDialogue_RunActor12TwoFlagScene Func_0200052c
#define SceneDialogue_ShowLine1CB0ForActor13 Func_0200056c
#define SceneDialogue_RunActor16CountedDialogue Func_020006b4
#define SceneDialogue_RunActorEightTimedDialogue Func_02000710
#define SceneDialogue_RunActor11AcceptanceDialogue Func_02000754
#define SceneDialogue_RunActor12TimedTwoFlagScene Func_02000784
/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

extern struct SceneWork_020006b4 *Data_03001ebc;
/*
 * Every call is named at its decoded bl site: the two Scene_GetRecord reads go
 * through two DIFFERENT per-site import veneers, and the dialogue and
 * Func_020004b4 calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */
extern void Func_020018c2_a(s32 dialogue);                 /* Func_0808a170 */
extern struct Actor_02000754 *Func_02001860(s32 actor);  /* Scene_GetRecord */
extern void Func_02000c20(s32 actor);                    /* Func_020004b4 */
extern struct Actor_02000754 *Func_02001872(s32 actor);  /* Scene_GetRecord */

/* Complete actor-8 flag-setting dialogue scene through its two-word pool. */
extern void Func_020015c4(void);
void Func_02001636(s32 actor, s32 mode, s32 value);
void Func_020015b4(s32 flag);
void Func_0200165a(s32 dialogue);
void Func_02001672(s32 actor, s32 mode);
void Func_020015ee(void);
void Func_02001674(s32 actor, s32 mode, s32 value);
void Func_020009d6(s32 actor);
void Func_0200168a(s32 actor, s32 mode, s32 value);
void Func_02001608(s32 flag);
void Func_0200160e(s32 flag);
void Func_020016b4(s32 dialogue);
void Func_020016cc(s32 actor, s32 mode);
void Func_02001648(void);
void Func_020016d4(s32 actor, s32 mode, s32 value);
void Func_02000a36(s32 actor);
void Func_020017a0(void);
void Func_02001826(s32 dialogue);
void Func_02001820(s32 actor, s32 mode, s32 value);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern s32 Func_02001840(s32 actor, s32 mode);
s32 Func_020017d0(s32 actor, s32 mode);
void Func_0200186c(s32 actor, s32 mode);
void Func_020017c4(s32 flag);
void Func_020017f0(void);
/* Complete actor-8 timed flag/dialogue scene through its two-word pool. */
extern void Func_020017fc(void);
void Func_02001864(s32 actor, s32 mode);
void Func_02001802(s32 frames);
void Func_0200187c(s32 actor, s32 mode, s32 frames);
void Func_020017fa(s32 flag);
void Func_020018a0(s32 dialogue);
void Func_020018c2(s32 actor, s32 mode, s32 frames);
void Func_02001836(void);
/* Complete actor-12 timed two-flag dialogue scene through its pool. */
extern void Func_02001870(void);
void Func_020018d8(s32 actor, s32 mode);
void Func_02001876(s32 frames);
void Func_020018f0(s32 actor, s32 mode, s32 frames);
void Func_0200186e(s32 flag);
void Func_02001874(s32 flag);
void Func_0200191a(s32 dialogue);
void Func_0200193c(s32 actor, s32 mode, s32 frames);
void Func_020018b0(void);

void SceneDialogue_RunActor8FlagScene(void)
{
    Func_020015c4();
    Func_02001636(8, 0, 2);
    Func_020015b4(0x305);
    Func_0200165a(0x1cab);
    Func_02001672(8, 0);
    Func_020015ee();
}

void SceneDialogue_RunActor11Line(void)
{
    /*
 * Complete actor-11 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 753-765, still unadopted at the
 * time of writing): `bl sub_0200167a`, `bl sub_02001674`, then
 * `bl .L_020009d6` -- the third call targets a local branch label at
 * 0x020009d6, not the guessed 0x020004b4.
 */
void Func_0200167a(s32 dialogue);

    Func_0200167a(0x1cae);
    Func_02001674(11, 0, 2);
    Func_020009d6(11);
}

void SceneDialogue_RunActor12TwoFlagScene(void)
{
    /*
 * Complete actor-12 two-flag dialogue scene through its three-word pool.
 *
 * Call symbols are per-site, not per-import: the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 766-783, still unadopted) spells
 * every call as `bl sub_02000xxx`, and the two calls that both ultimately
 * reach GameFlag_Set go through TWO DIFFERENT overlay-local veneers,
 * sub_02001608 and sub_0200160e -- not the same symbol twice.
 */
void Func_02001618(void);

    Func_02001618();
    Func_0200168a(12, 0, 2);
    Func_02001608(0x306);
    Func_0200160e(0x868);
    Func_020016b4(0x1caf);
    Func_020016cc(12, 0);
    Func_02001648();
}

void SceneDialogue_ShowLine1CB0ForActor13(void)
{
    /*
 * Complete actor-13 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 757-769, still unadopted at the
 * time of writing): `bl sub_020016da`, `bl sub_020016d4`, `bl sub_02000a36`.
 */
void Func_020016da(s32 dialogue);

    Func_020016da(0x1cb0);
    Func_020016d4(13, 0, 2);
    Func_02000a36(13);
}

void SceneDialogue_RunActor16CountedDialogue(void)
{
    Func_020017a0();
    Func_02001826(0x1cb5);
    Func_02001820(16, 0, 2);
    Func_02001840(16, 0);
    if (Func_020017d0(0, 0) != 0) {
        Data_03001ebc->branch_counter += 1;
    }
    Func_0200186c(16, 0);
    Func_020017c4(0x308);
    Func_020017f0();
}

void SceneDialogue_RunActorEightTimedDialogue(void)
{
    Func_020017fc();
    Func_02001864(8, 1);
    Func_02001802(20);
    Func_0200187c(8, 0, 20);
    Func_020017fa(0x305);
    Func_020018a0(0x1cab);
    Func_020018c2(8, 0, 20);
    Func_02001836();
}

void SceneDialogue_RunActor11AcceptanceDialogue(void)
{
    Func_020018c2_a(0x1cbd);
    Func_02001860(11)->accepted = 1;
    Func_02000c20(11);
    Func_02001872(11)->accepted = 0;
}

void SceneDialogue_RunActor12TimedTwoFlagScene(void)
{
    Func_02001870();
    Func_020018d8(12, 1);
    Func_02001876(20);
    Func_020018f0(12, 0, 20);
    Func_0200186e(0x306);
    Func_02001874(0x868);
    Func_0200191a(0x1caf);
    Func_0200193c(12, 0, 20);
    Func_020018b0();
}
