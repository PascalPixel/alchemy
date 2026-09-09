#include "types.h"

#define FieldScene_RunPrimaryScriptChoreography Func_02000f90

/* Reviewed owner resource_399:0x02000f90, 1468 bytes.
 *
 * A long primary-script scene body for overlay resource_399. It arranges
 * actor records 0, 1, 2, 3, 8, 9 and 10 through position, animation, camera
 * and timing calls, waits twice on a per-record halfword at offset 100,
 * plays three sound cues, toggles bit 0 of a record flag byte at offset 90
 * for actors 0, 1 and 2, and stamps the shared scene work record at
 * 0x03001ebc (phase word 0x1c0, related word 0x1c8) at the start and again
 * at the end before setting one game flag.
 *
 * Loader-relocated overlay calls: every Func_02xxxxxx symbol below names the
 * pre-relocation call word the stored image holds at its site, not a function
 * body in this address range. The loader rewrites those BL pairs to the
 * overlay's own veneer island (0x0200a1xx-0x0200a3xx at runtime), which in
 * turn reaches the main image. Because the stored word is site-relative, one
 * runtime veneer is named by several such symbols and one symbol can name two
 * different veneers.
 *
 * NOTE FOR THE INTEGRATOR: eleven of the reference's site names are
 * genuinely ambiguous over this owner - ten cover two veneers each and
 * Func_0200365a covers three - so at 23 sites this draft spells the call
 * with another site's unambiguous name for the same veneer. That keeps the
 * emitted target correct and lets the owner score without a translation-unit
 * binding. On adoption, prefer this overlay's established convention
 * (Func_<site>_a / Func_<site>_b plus explicit absolute_symbols) as in
 * run_scene_399_three_actor_choreography.c. Three veneers have no
 * unambiguous site name at all (runtime 0x0200a218, 0x0200a370, 0x0200a378)
 * and are spelled here as Data_<runtime address> so they resolve by address.
 *
 * The complete substitution list, as reference site -> spelling used here:
 *   Func_02003252  0x02000fa0 -> Func_020032e6   0x02001000 -> Func_02003248
 *   Func_02003472  0x02001140 -> Func_0200351c   0x02001198 -> Func_0200338a
 *   Func_02003490  0x0200115e -> Func_0200351c   0x0200131e -> Func_0200312a
 *   Func_020035e2  0x020012d8 -> Func_020036b0   0x02001350 -> Func_020032a6
 *   Func_02003616  0x020013ac -> Func_0200361c   0x020013cc -> Func_02003270
 *   Func_0200365a  0x020012e8 -> Data_0200a370   0x020012f0 -> Func_0200367c
 *                  0x02001338 -> Func_02003342
 *   Func_02003670  0x020012f6 -> Data_0200a378   0x02001366 -> Func_020036b0
 *   Func_02003692  0x02001318 -> Data_0200a378   0x02001440 -> Func_02003248
 *   Func_0200370e  0x02001494 -> Func_020036fa   0x020014c4 -> Func_02003270
 *   Func_0200372c  0x0200140a -> Func_02003342   0x02001502 -> Func_020032c2
 *   Func_02003738  0x02001416 -> Func_02003342   0x0200151e -> Data_0200a218
 *
 * Veneer roles. Every row below is the role name the overlay's exact-C
 * sibling resource_399:0x020019e8 already gives the same main-image entry
 * through an unambiguous site symbol, matched by main address:
 *   0x0200a218  GameFlag_Set                              (0x080770c8)
 *   0x0200a228  BattleRuntime_WaitIfModeZero              (0x0808a010)
 *   0x0200a230  BattleRuntime_Reset                       (0x0808a018)
 *   0x0200a238  BattleRuntime_ScheduleShoulderButtonModeUpdate
 *   0x0200a248  Scene_GetRecord                           (0x0808a080)
 *   0x0200a250  ObjectMotion_SetSpeedParameters           (0x0808a090)
 *   0x0200a288  ObjectMotion_ResetAndSetPositionInMode2   (0x0808a0c8)
 *   0x0200a290  ObjectMotion_SetPositionAndReset          (0x0808a0d0)
 *   0x0200a2b0  ObjectMotion_SetHorizontalPositionWithTerrain
 *   0x0200a2b8  Object_SetModeById                        (0x0808a100)
 *   0x0200a2c0  ObjectMotion_CallThenWaitForAnimationChange
 *   0x0200a2d8  ObjectMotion_SetVariantCallbackAndInvokeObject
 *   0x0200a300  SceneWork_SetStepValue                    (0x0808a170)
 *   0x0200a310  BattleRuntime_RunThenWaitIfModeZero       (0x0808a188)
 *   0x0200a320  ObjectMotion_ArmCallback                  (0x0808a1b8)
 *   0x0200a330  BattleEffect_SpawnLinkedResourceObject    (0x0808a1e8)
 *   0x0200a338  BattleRuntime_WaitIfModeZero              (0x0808a1f0)
 *   0x0200a340  ObjectMotion_SetSpeedLimitAndAcceleration (0x0808a208)
 *   0x0200a348  ObjectMotion_PlaceWithinCameraBounds      (0x0808a210)
 *   0x0200a380  BattleRuntime_WaitIfModeZero              (0x0808a360)
 * One further role is evidenced outside that sibling: 0x0200a3e8 reaches
 * main 0x080f9010, which battle/event_runtime/dispatch_queued_events.c calls
 * with a pending sound id, so its three uses here are sound cues.
 * The remaining veneers (0x0200a170, 0x0200a1e0, 0x0200a258, 0x0200a260,
 * 0x0200a268, 0x0200a270, 0x0200a278, 0x0200a2f8, 0x0200a308, 0x0200a350,
 * 0x0200a368, 0x0200a370, 0x0200a378) have no repository name yet and keep
 * their raw spellings.
 *
 * Func_02002800 is the one intra-overlay direct call; it reaches
 * SceneState_StoreTable96adToWork at 0x020016c8.
 *
 * STATUS: drafted, not adopted. 1468 candidate bytes against 1468 reference
 * bytes, 36 differing halfwords, 4 wrong instructions, topology equal,
 * residual class allocation-uncovered. All 142 reference call sites, both
 * `record != 0` guards, both wait loops, every literal constant and every
 * store are represented; the extent is exact and no code was dropped.
 *
 * Residual, now one coupled defect. The literal table address held in r5
 * across each of the three table groups (0x0200a74c, 0x0200a5ec, 0x0200a760)
 * is scheduled one to two calls earlier here than in the reference, which
 * issues each `ldr r5, [pc, ...]` at the group's first use. Because the third
 * hoisted load makes r5 live immediately after the last `|= 1` flag update,
 * that update also computes into r3 here where the reference clobbers the
 * now-dead constant register r5. Fixing the schedule would fix both; nothing
 * structural is missing.
 *
 * Measured source hypotheses, so the next reader does not repeat them:
 *  - Binding the wait flag's address before materialising the zero
 *    (`done = (u16 *)(... + 100); val = 0; *done = val;`) keeps the zero's
 *    live range off the call and restores the reference's scratch register.
 *    This removed the two `strh` mismatches: 40 -> 36 differing halfwords,
 *    12 -> 4 wrong instructions.
 *  - Storing the zero directly (`*(u16 *)(... + 100) = 0;`) makes GCC 2.96
 *    materialise the HImode constant from the literal pool, adding two pool
 *    words: 1476 bytes, 416 differing halfwords. The `val` temporary is
 *    load-bearing, not decoration.
 *  - Block-scoping the three table locals: neutral.
 *  - Splitting the shared table local into three: neutral.
 *  - Inlining the table constants at their call sites: regressed to 255.
 *  - Spelling the first table call directly rather than through Call3:
 *    regressed to 637; the Call/Value wrappers are load-bearing.
 * The triage router reports "do not probe register roles by respelling
 * source", so the remaining schedule residual is reported, not chased. */

extern u8 Data_03001ebc[];

/* Veneers with no unambiguous reference site name over this owner. */
void Data_0200a218();
void Data_0200a370();
void Data_0200a378();

void Func_02002800();
void Func_0200312a();
void Func_020031c4();
void Func_02003248();
void Func_0200325c();
s32 Func_02003270();
s32 Func_02003284();
void Func_020032a6();
void Func_020032c2();
void Func_020032d2();
void Func_020032dc();
void Func_020032e6();
void Func_020032ec();
void Func_020032ee();
void Func_020032fa();
void Func_020032fc();
void Func_02003302();
void Func_0200331c();
void Func_0200331e();
s32 Func_02003322();
void Func_02003332();
void Func_02003342();
void Func_02003352();
void Func_0200336a();
void Func_0200336e();
void Func_02003374();
void Func_0200337e();
void Func_0200338a();
s32 Func_0200338c();
void Func_02003392();
void Func_020033a0();
void Func_020033b2();
void Func_020033b8();
void Func_020033c4();
void Func_020033c8();
void Func_020033d2();
void Func_020033da();
void Func_020033dc();
void Func_020033e0();
void Func_020033e4();
void Func_020033ee();
void Func_020033f8();
void Func_02003400();
void Func_0200340a();
void Func_0200340c();
void Func_02003420();
void Func_02003436();
void Func_0200343c();
void Func_02003440();
void Func_02003442();
void Func_02003448();
void Func_0200344e();
void Func_02003458();
void Func_0200345c();
void Func_02003460();
void Func_02003466();
void Func_0200346e();
void Func_0200347a();
void Func_02003480();
s32 Func_02003482();
void Func_02003484();
void Func_02003498();
void Func_020034a0();
void Func_020034a2();
void Func_020034b8();
void Func_020034d2();
s32 Func_020034d6();
void Func_020034e0();
void Func_020034f0();
void Func_020034f2();
void Func_020034fa();
void Func_02003510();
void Func_02003518();
void Func_0200351a();
void Func_0200351c();
void Func_02003554();
void Func_02003558();
void Func_02003580();
void Func_02003598();
void Func_020035a4();
void Func_020035b2();
void Func_020035d4();
void Func_020035ea();
void Func_020035f2();
void Func_02003618();
void Func_0200361c();
void Func_02003622();
void Func_02003630();
s32 Func_02003632();
void Func_02003634();
void Func_02003638();
void Func_02003662();
void Func_0200367c();
void Func_02003676();
void Func_02003684();
void Func_0200368a();
void Func_020036a4();
s32 Func_020036a8();
void Func_020036b0();
s32 Func_020036b8();
void Func_020036bc();
void Func_020036c8();
void Func_020036ca();
void Func_020036d0();
void Func_020036ec();
s32 Func_020036f0();
void Func_020036fa();
void Func_020036fc();
s32 Func_02003696();
s32 Func_02003700();
void Func_02003704();
void Func_02003720();
void Func_02003728();
void Func_02003732();
void Func_0200373c();
void Func_0200374e();
void Func_0200375c();
void Func_0200376e();
void Func_02003830();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Byte offset of the flag byte on an actor record; bit 0 is cleared for
 * actors 0, 1 and 2 and set again after they are repositioned. */
#define ACTOR_FLAGS_OFFSET 90

/* Halfword at offset 100 of an actor record: the scene waits until it
 * becomes non-zero. */
#define ACTOR_DONE_OFFSET 100

void FieldScene_RunPrimaryScriptChoreography(void)
{
    s32 record;
    s32 tbl;
    s32 val;
    u16 *done;
    u8 *work;

    Func_020031c4();
    Call3(Func_020032e6, 3, 0xb60000, 0x960000);
    Call4(Func_020032fc, 0x8d0000, -1, 0xdd0000, 0);
    Func_0200312a(1);
    Call2(Func_02003302, 0x4ccc, 0x999);
    Call4(Func_0200331c, 0x8c0000, -1, 0xa40000, 1);
    /* Enter the scene phase. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(work + 0x1c0) = 0x100;
    *(volatile s32 *)(work + 0x1c8) = 40;
    Func_0200336e();
    Call3(Func_02003248, 0, 0x6666, 0x3333);
    Call3(Func_02003248, 1, 0x6666, 0x3333);
    Call3(Func_0200325c, 2, 0x6666, 0x3333);
    Func_020032a6(0, 142, 221);
    Call3(Func_02003342, 0, 0xd000, 0);
    record = Value1(Func_02003270, 0);
    if (record != 0) {
        Func_020032e6(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02003284, 0);
    if (record != 0) {
        Func_020032fa(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_020032dc(1, 150, 234);
    Func_020032ee(2, 134, 234);
    Func_0200331e(1, 1);
    tbl = 0x200a74c;
    Call3(Func_0200336a, 0, 0x10003, tbl);
    Call3(Func_02003374, 1, 0x10003, tbl);
    Call3(Func_0200337e, 2, 0x10003, tbl);
    Func_020033da();
    tbl = 0x200a5ec;
    Func_020032ec(9, tbl);
    Func_020032c2(40);
    Call2(Func_020033dc, 3, 0x102);
    Func_020032d2(40);
    Func_0200338a(3, 1);
    Call1(Func_020033b8, 0x155c);
    Func_020033d2(3, 0, 20);
    Value2(Func_02003322, 9, tbl);
    Func_020033e4(9, 0, 20);
    Call3(Func_02003400, 3, 0x8000, 20);
    Func_0200340a(8, 0, 10);
    Func_020033b2(8, 4);
    Func_0200340c(8, 0, 40);
    Func_020033c4(3, 3);
    Func_02003332(10);
    Call3(Func_02003436, 3, 0x4000, 0);
    Call3(Func_02003442, 8, 0x3000, 20);
    Func_0200343c(3, 0, 10);
    Value2(Func_0200338c, 9, tbl);
    /* SceneState_StoreTable96adToWork at 0x020016c8. */
    Func_02002800();
    Call3(Func_0200351c, 3, 0x101, 60);
    Func_0200345c(3, 0, 40);
    Func_02003466(9, 0, 20);
    Call3(Func_0200351c, 8, 0x105, 60);
    Func_02003420(9, 7);
    Call3(Func_02003352, 0x200b1c0, 10, 69);
    Func_020033a0(10);
    Func_02003458(3, 2);
    Func_02003448(3, 4);
    Func_020034a2(3, 0, 20);
    Func_0200338a(9, 1);
    Func_020033c8(40);
    Func_02003460(9, 8);
    Call3(Func_02003392, 0x200b1d6, 10, 69);
    Func_020033e0(40);
    Func_02003480(3, 3);
    Func_020033ee(20);
    Func_020034f0(8, 0, 20);
    Func_02003498(8, 3);
    Func_020034f2(8, 0, 10);
    Call3(Func_0200351c, 3, 0x101, 30);
    Call3(Func_02003518, 3, 0x8000, 10);
    Func_020034b8(3, 4);
    Func_0200351a(3, 0, 10);
    Func_020034d2(8, 3);
    Func_02003440(20);
    Func_020034e0(3, 3);
    Func_0200344e(40);
    Call3(Func_02003484, 3, 0x10000, 0x8000);
    done = (u16 *)(Func_02003482(3) + ACTOR_DONE_OFFSET);
    val = 0;
    *done = val;
    Call2(Func_020034a0, 3, 0x200a670);
    while (*(s16 *)(Func_020034d6(3) + ACTOR_DONE_OFFSET) == 0) {
        Func_020033f8(1);
    }
    Call4(Func_020035f2, 0x8c0000, -1, 0xc60000, 1);
    Func_02003510(3);
    Call3(Func_020035ea, 3, 0x101, 80);
    Func_020035d4(3, 0, 40);
    Func_020035a4(3, 1);
    Func_020034fa(10);
    Func_020036b0(3, 0);
    Func_020036c8(131);
    Call2(Data_0200a370, 0x10000, 0);
    Call2(Func_0200367c, 0x207e9f, 0);
    Data_0200a378(10);
    Func_0200346e(1);
    Func_020036ec(220);
    Func_0200347a(40);
    Call2(Func_0200367c, 0x10000, 0);
    Data_0200a378(60);
    Func_0200312a(60);
    Call2(Func_02003662, 3, 0x102);
    Func_02003558(20);
    Func_02003342(3, 0, 10);
    Call3(Func_02003598, 3, 0x20000, 0x10000);
    Func_020032a6(3, 202, 198);
    Func_02003580(40);
    Func_02003638(3, 2);
    Func_020036b0(3, 0);
    Func_02003630(3, 4);
    Func_0200368a(3, 0, 20);
    Call2(Func_020036bc, 3, 0x102);
    Func_020035b2(40);
    Func_020036a4(3, 0, 40);
    Call3(Func_020036d0, 3, 0x100, 40);
    Func_020036b0(3, 0);
    Call1(Func_0200361c, 0);
    Func_0200361c(1);
    Func_02003622(2);
    Call3(Func_02003618, 3, 0x30000, 0x18000);
    done = (u16 *)(Func_02003270(3) + ACTOR_DONE_OFFSET);
    val = 0;
    *done = val;
    Call2(Func_02003634, 3, 0x200a6e0);
    while (*(s16 *)(Func_02003632(3) + ACTOR_DONE_OFFSET) == 0) {
        Func_02003554(1);
    }
    Call3(Func_02003720, 0, 0x4000, 0);
    Call3(Func_02003342, 1, 0x4000, 0);
    Call3(Func_02003342, 2, 0x4000, 10);
    Call3(Func_02003676, 0, 0x40000, 0x20000);
    Call3(Func_02003684, 1, 0x40000, 0x20000);
    Call3(Func_02003248, 2, 0x40000, 0x20000);
    Func_02003830(152);
    *(u8 *)(Func_02003696(0) + ACTOR_FLAGS_OFFSET) &= 254;
    *(u8 *)(Func_020036a8(1) + ACTOR_FLAGS_OFFSET) &= 254;
    *(u8 *)(Func_020036b8(2) + ACTOR_FLAGS_OFFSET) &= 254;
    Func_020036fa(0, 132, 206);
    Func_02003704(1, 136, 221);
    Call3(Func_020036fa, 2, 122, 238);
    Func_020036fc(3);
    Func_020036ca(80);
    *(u8 *)(Func_020036f0(0) + ACTOR_FLAGS_OFFSET) |= 1;
    *(u8 *)(Func_02003700(1) + ACTOR_FLAGS_OFFSET) |= 1;
    *(u8 *)(Func_02003270(2) + ACTOR_FLAGS_OFFSET) |= 1;
    Call3(Func_02003728, 0, 0xcccc, 0x6666);
    Call3(Func_02003732, 1, 0xcccc, 0x6666);
    Call3(Func_0200373c, 2, 0xcccc, 0x6666);
    tbl = 0x200a760;
    Func_0200374e(1, tbl);
    Func_0200376e(2, tbl);
    Func_020032c2(20);
    /* Advance the scene phase word to its next value. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(work + 0x1c0) = 0x209;
    *(volatile s32 *)(work + 0x1c8) = 24;
    Call1(Data_0200a218, 0x82b);
    Func_0200375c();
}
