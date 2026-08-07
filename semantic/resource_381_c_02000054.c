#include "types.h"

struct Resource381SceneState {
    u8 unknown_000[0x40C];
    s32 scriptPhase;
};

struct MainSceneState {
    u8 unknown_000[0x1C8];
    s32 activeActorLimit;
};

struct SceneActor {
    u8 unknown_00[0x0A];
    s16 positionX;
    u8 unknown_0C[6];
    s16 positionZ;
    u8 unknown_14[0x41];
    u8 behavior;
    u8 unknown_56[4];
    u8 flags;
};

/*
 * Resource 381 encounter/setup script.
 *
 * This function is a long, linear orchestration routine: it initializes the
 * overlay state, creates and configures actors and effects, schedules their
 * movements, and finally hands control to the completed scene. Every call is
 * named by its resolved overlay-local or resident service target.
 *
 * Repeated call-shape families:
 * - (slot, x, z): actor/effect placement, with slot IDs 5, 9, A, B, D, E.
 * - (slot, value): per-actor mode, duration, or animation selection.
 * - (slot, x, z, mode): actor creation with an explicit behavior variant.
 * - (duration): scene waits and transition gates, commonly 0x14/0x1e/0x28.
 *
 * Coordinates are predominantly 16.16 fixed point. Small unshifted values
 * passed to the same helper families are therefore IDs, modes, or durations,
 * not alternate coordinate encodings.
 *
 * ABI assumptions:
 * - 0x03001EC4 is a pointer slot for the resource-local scene state.
 * - the pointer slot eight bytes earlier owns activeActorLimit at +0x1C8.
 * - Func_0808a080 returns the actor layout, of which only positionX,
 *   positionZ, behavior, and flags are used.
 * - helpers without a typed declaration retain an old-style declaration
 *   deliberately: the calls preserve observed register arguments, while
 *   stronger shared ABI claims require evidence from their own bodies.
 */
/* Resolved whole-owner call targets. */
extern void Func_02003530();
extern void Func_0200368e();
extern void Func_0200350e();
extern void Func_020035bc();
extern void Func_020035ca();
extern void Func_0200363e();
extern void Func_0200364a();
extern void Func_020035ee();
extern void Func_020035fa();
extern void Func_02003606();
extern void Func_02003612();
extern void Func_0200361e();
extern void Func_0200362a();
extern void Func_020036a2();
extern void Func_020036d6();
extern void Func_0200357a();
extern void Func_02003722();
extern void Func_0200372e();
extern void Func_0200371e();
extern void Func_0200372c();
extern void Func_0200360a();
extern void Func_02003734();
extern void Func_02003742();
extern void Func_02003620();
extern void Func_020010bc();
extern void Func_0200374c();
extern void Func_0200375a();
extern void Func_02003638();
extern void Func_02003796();
extern void Func_02003768();
extern void Func_02003776();
extern void Func_02003654();
extern void Func_0200377c();
extern void Func_0200378a();
extern void Func_02003668();
extern void Func_020037c6();
extern void Func_02003798();
extern void Func_020037a6();
extern void Func_02003684();
extern void Func_0200370e();
extern void Func_02003718();
extern void Func_02002f1e();
extern void Func_02002f28();
extern void Func_02003762();
extern void Func_02003784();
extern void Func_02002f4a();
extern void Func_020037a0();
extern void Func_02002f5e();
extern void Func_020037b4();
extern void Func_020037be();
extern void Func_02003706();
extern void Func_0200371c();
extern void Func_020037f8();
extern void Func_02003804();
extern void Func_02002fba();
extern void Func_020037ce();
extern void Func_020037de();
extern void Func_02003754();
extern void Func_02002fda();
extern void * Func_02003784_b();
extern void * Func_0200379c();
extern void Func_020038e6();
extern void Func_02003764();
extern void Func_02003770();
extern void Func_02003878();
extern void Func_0200388a();
extern void Func_02003864();
extern void Func_020038a8();
extern void Func_020038b4();
extern void Func_02003858();
extern void Func_02003870();
extern void Func_0200387c();
extern void Func_02003888();
extern void Func_02003894();
extern void Func_02003924();
extern void Func_0200393c();
extern void Func_02003948();
extern void Func_020039a6();
extern void Func_02003976();
extern void Func_02003984();
extern void Func_0200377a();
extern void Func_0200398c();
extern void Func_0200399a();
extern void Func_02003938();
extern s32 Func_020038a0();
extern void Func_02003942();
extern void Func_02003998();
extern void Func_020039ba();
extern void Func_020039a8();
extern void Func_020039ca();
extern void Func_02003188();
extern void Func_02003a72();
extern void Func_020038f2();
extern void Func_02003930();
extern void Func_02003a92();
extern void Func_02003910();
extern void Func_0200391c();
extern void Func_020031d2();
extern void Func_02003a28();
extern void Func_02003a3c();
extern void Func_020031f2();
extern void Func_020031fc();
extern void Func_02003a52();
extern void Func_02003a66();
extern void Func_0200321c();
extern void Func_02003226();
extern void Func_02003a7c();
extern void Func_02001570();
extern void Func_020038e2();
extern void Func_020038ec();
extern void Func_020039d2();
extern void Func_02003aa4();
extern void Func_02003a50();
extern void Func_02003a5c();
extern void Func_02003a68();
extern void Func_02003a74();
extern void Func_02003a80();
extern void Func_02003a8c();
extern void Func_02003b00();
extern void Func_02003b0c();
extern void Func_02003b18();
extern void Func_02003b24();
extern void Func_02003b30();
extern void Func_02003b3c();
extern void * Func_02003a92_b();
extern void Func_02003b82();
extern void Func_02003b8e();
extern void Func_02003a94();
extern void Func_02003b2c();
extern void Func_02003aa2();
extern void Func_02003b6a();
extern void Func_02003b42();
extern void Func_02003b84();
extern void Func_02003b34();
extern void Func_02003b96();
extern void Func_02003b70();
extern void Func_02003b50();
extern void Func_02003bb2();
extern void Func_02003bd6();
extern void Func_02003bd2();
extern void Func_02003bd4();
extern void Func_02003be8();
extern void Func_02003bf4();
extern void Func_02003bc6();
extern void Func_02003c18();
extern void Func_02003c14();
extern void Func_02003c16();
extern void Func_02003c2a();
extern void Func_02003bf2();
extern void Func_02003c34();
extern void Func_02003be4();
extern void Func_02003c46();
extern void Func_02003c5a();
extern void Func_02003c0a();
extern void Func_02003c64();
extern void Func_02003c78();
extern void Func_02003c84();
extern void Func_02003c90();
extern void Func_02003c9c();
extern void Func_02003ca8();
extern void Func_02003cb4();
extern void Func_02003cbe();
extern void Func_02003ce0();
extern void Func_02003c3e();
extern void Func_02003c72();
extern void Func_02003caa();
extern void Func_02003cf4();
extern void Func_02003d08();
extern void Func_02003cb0();
extern void Func_02003d12();
extern void Func_02003cda();
extern void Func_02003d24();
extern void Func_02003d36();
extern void Func_02003d4a();
extern void Func_02003d4c();
extern void Func_02003d5e();
extern void Func_02003d26();
extern void Func_02003d1c();
extern void Func_02003d86();
extern void Func_02003cc4();
extern void Func_02003d96();
extern void Func_02003daa();
extern void Func_02003dc0();
extern void Func_02003dd8();
extern void Func_02003d46();
extern void Func_02003dbe();
extern void Func_02003dce();
extern void Func_02003e32();
extern void Func_02003e3e();
extern void Func_02003d74();
extern void Func_02003e0c();
extern void Func_02003d82();
extern void Func_02003e5e();
extern void Func_02003d94();
extern void Func_02003e80();
extern void Func_02003da6();
extern void Func_02003e82();
extern void Func_02003db8();
extern void Func_02003e38();
extern void Func_02003e92();
extern void Func_02003ea6();
extern void Func_02003eb2();
extern void Func_02003ebe();
extern void Func_02003eda();
extern void Func_02003ecc();
extern void Func_02003eee();
extern void Func_02003e8e();
extern void Func_02003f00();
extern void Func_02003f0a();
extern void Func_02003ec2();
extern void Func_02003f04();
extern void Func_02003ebc();
extern void Func_02003e4a();
extern void Func_02003f36();
extern void Func_02003f42();
extern void Func_02003ee4();
extern void Func_02003ef4();
extern void Func_02003ec6();
extern void Func_02003ef2();
extern void Func_02003efe();
extern void Func_02003f14();
extern void Func_02003f1a();
extern void Func_02003f32();
extern void Func_02003f3a();
extern void Func_02003fa6();
extern void Func_02003fb2();
extern void Func_02003fbe();
extern void Func_02003fd8();
extern void Func_02003f90();
extern void Func_02003fc8();
extern void Func_02003f4c();
extern void Func_02003f80();
extern void * Func_02003f4e();
extern void Func_02003fa0();
extern void Func_02003fe0();
extern void Func_02003fda();
extern void Func_02003fa4();
extern void Func_0200405e();
extern void Func_0200400e();
extern void Func_02004058();
extern void Func_02004028();
extern void Func_02003f9e();
extern void Func_02003fe4();
extern void Func_02004018();
extern void Func_0200409c();
extern void Func_02004094();
extern void Func_020040b0();
extern void Func_02004070();
extern void Func_020040ba();
extern void Func_020040de();
extern void Func_020040da();
extern void Func_020040e6();
extern void Func_02004096();
extern void Func_0200409e();
extern void Func_02004102();
extern void Func_020040b2();
extern void Func_02004048();
extern void Func_020040a6();
extern void Func_02004146();
extern void Func_02004152();
extern void Func_0200411a();
extern void Func_02004166();
extern void Func_02004168();
extern void Func_02004138();
extern void Func_020040ae();
extern void Func_0200418a();
extern void Func_020040f8();
extern void Func_0200412c();
extern void Func_020041b0();
extern void Func_0200411e();
extern void Func_0200413a();
extern void Func_02004148();
extern void Func_0200417c();
extern void Func_02004200();
extern void Func_02004194();
extern void Func_02004218();
extern void Func_020041ac();
extern void Func_02004230();
extern void Func_0200423c();
extern void Func_02004248();
extern void Func_02004254();
extern void Func_02004260();
extern void Func_0200428c();
extern void Func_020042a6();
extern void Func_020042b2();
extern void Func_02004206();
extern void Func_02004204();
extern void Func_0200424a();
extern void Func_02004258();
extern void Func_02004266();
extern void Func_020042b6();
extern void Func_02001ba8();
extern void Func_020042dc();
extern void Func_02001bb6();
extern void Func_020042d2();
extern void Func_02001bc4();
extern void Func_0200433c();
extern void Func_020042fc();
extern void Func_02004346();
extern void Func_0200435a();
extern void Func_02004366();
extern void Func_02004372();
extern void Func_02001c0c();
extern void Func_02001c12();
extern void Func_0200438a();
extern void Func_0200438c();
extern void Func_02004344();
extern void Func_020043a4();
extern void Func_02001c46();
extern void Func_0200222a();

                            

                            

                            

                            

                            

                            

                            

                            

                            

                            

                            

                            

                            

                            

/* Overlay entry 0x02000054: set up and execute the scripted scene. */
void Func_02000054(void) {
    struct SceneActor *actor;
    struct SceneActor *configuredActor;
    struct Resource381SceneState **sceneSlot;
    struct Resource381SceneState *scene;
    s32 *firstPhase;
    s32 *secondPhase;

    sceneSlot = (struct Resource381SceneState **)0x03001EC4;
    scene = *sceneSlot;
    firstPhase = &scene->scriptPhase;

    /* Phase 0: reset the encounter and establish the initial actor layout. */
    Func_02003530();
    Func_0200368e(0x8D);
    Func_0200350e(0x10000, 0x10000, 0x10000);
    *firstPhase = 0;
    Func_020035bc(0, 0xE80000, 0x9C0000);
    Func_020035ca(1, 0xDA0000, 0xAC0000);
    Func_0200363e(0, 0xB000, 0);
    Func_0200364a(1, 0xD000, 0);
    Func_020035ee(5, 0x01DB0000, 0x014C0000);
    Func_020035fa(9, 0x01EB0000, 0x014C0000);
    Func_02003606(0xB, 0x01CB0000, 0x015C0000);
    Func_02003612(0xA, 0x01FB0000, 0x015C0000);
    Func_0200361e(0xD, 0x01D70000, 0x01320000);
    Func_0200362a(0xE, 0x01DF0000, 0x016A0000);
    Func_020036a2(0xF, 1);
    Func_020036d6(0xE80000, -1, 0x9C0000, 0);
    Func_0200357a();
    Func_02003722();
    Func_0200372e();
    Func_0200371e(0x7FFF, 0);
    (*(struct MainSceneState **)((u8 *)sceneSlot - 8))->activeActorLimit = 8;
    Func_0200372c(4);
    Func_0200360a(4);
    Func_02003734(0x10000, 0);
    Func_02003742(4);
    Func_02003620(4);
    Func_020010bc();
    Func_0200374c(0x7FFF, 0);
    Func_0200375a(4);
    Func_02003638(0x10);
    Func_02003796(0x90);
    Func_02003768(0x10000, 0);
    Func_02003776(4);
    Func_02003654(4);
    Func_0200377c(0x7FFF, 0);
    Func_0200378a(4);
    Func_02003668(4);
    Func_020037c6(0x90);
    Func_02003798(0x10000, 0);
    Func_020037a6(0x30);
    Func_02003684(0x30);
    Func_0200370e(0, 6, 0);
    Func_02003718(1, 6, 0x14);
    Func_02002f1e(1, 0x14, 0x14);
    Func_02002f28(0, 0x14, 0x28);
    Func_02003762(0x10CD);
    Func_02003784(0xB, 0, 0x14);
    Func_02003784(0xA, 0);
    Func_02002f4a(1, 0x14, 0);
    Func_020037a0(5, 0, 0x14);
    Func_02002f5e(0, 0x14, 0);
    Func_020037b4(0xE, 0, 0x14);
    Func_020037be(9, 0, 0x14);
    Func_02003798(0, 1, 0);
    Func_02003706(0x28);
    Func_02003796(0, 2);
    Func_020037a6(1, 2);
    Func_0200371c(0x14);
    Func_020037f8(0, 0xB000, 0);
    Func_02003804(1, 0xD000, 0x1E);
    Func_02002fba(1, 0x14, 0);
    Func_020037ce(0, 2);
    Func_020037de(1, 2);
    Func_02003754(0x14);
    Func_02002fda(0, 0x14, 0x14);
    Func_02003722(Func_02003784_b(0xF), 0);
    Func_020037de(0xF, 0x01450000, 0x012E0000);
    ((struct SceneActor *)Func_0200379c(0xF))->behavior = 5;
    /*
     * Phase 1: run the first staged exchange. Calls in the 020037xx-020039xx
     * family repeatedly address the same six actor slots with fixed-point
     * positions and short transition durations.
     */
    *firstPhase = 1;
    Func_020038e6(0x121);
    Func_02003764(-1, -1, 0xE666);
    Func_02003770();
    Func_020037a6(0x96);
    Func_02003878(0xB, 0, 0x14);
    Func_02003878(5, 0);
    Func_0200388a(0xA, 0, 0xA);
    Func_02003864(0, 1, 0x14);
    Func_020038a8(0, 0x3000, 0);
    Func_020038b4(1, 0x3000, 0xA);
    Func_02003858(5, 0x01DB0000, 0x014C0000);
    Func_02003864(9, 0x01EB0000, 0x014C0000);
    Func_02003870(0xB, 0x01CB0000, 0x015C0000);
    Func_0200387c(0xA, 0x01FB0000, 0x015C0000);
    Func_02003888(0xD, 0x01D70000, 0x01320000);
    Func_02003894(0xE, 0x01DF0000, 0x016A0000);
    Func_02003924(0x66666, 0xCCCC);
    Func_0200393c(0x01480000, -1, 0x012B0000, 1);
    Func_02003948();
    Func_020039a6(0xA7);
    Func_02003976(0x205294, 2);
    Func_02003984(0x14);
    Func_0200377a(0x14);
    Func_0200398c(0x10000, 2);
    Func_0200399a(0x14);
    Func_02003878(0xC8);
    Func_02003938(0x1001, 0);
    if (Func_020038a0(0, 0) != 0) {
        goto block_2;
    }
    Func_02003942(0x10D6);
    goto block_3;
block_2:
    Func_02003998(0x10D7);
block_3:
    Func_020039ba(0x1001, 0, 0x50);
    Func_020039a8(0x10D8);
    Func_020039ca(9, 0, 0x14);
    Func_02003188(1, 0x14, 0);
    /* Phase 2 setup: clear the phase latch and rebuild the formation. */
    secondPhase = &scene->scriptPhase;
    *secondPhase = 0;
    Func_02003a72(0x8D);
    Func_020038f2(0x10000, 0x20000, 0x10000);
    Func_02003930(0x50);
    /*
     * Phase 2 execution: the long 02003axx-020043xx sequence composes actor
     * motion, flags, animation selections, and waits before final handoff.
     */
    *secondPhase = 1;
    Func_02003a92(0x121);
    Func_02003910(-1, -1, 0xE666);
    Func_0200391c();
    Func_020031d2(0, 0x14, 0x3C);
    Func_02003a28(0xE, 0, 0x1E);
    Func_02003a3c(0xF, 0xA000, 0x28);
    Func_020031f2(1, 0x14, 0x14);
    Func_020031fc(0, 0x14, 0x14);
    Func_02003a52(0x1001, 0, 0x1E);
    Func_02003a66(0xF, 0x1000, 0x28);
    Func_0200321c(1, 0x14, 0x14);
    Func_02003226(0, 0x14, 0x14);
    Func_02003a7c(5, 0, 0x1E);
    Func_02001570();
    Func_020038e2(0x020090C5, 0xC80);
    Func_020038ec(0x0200935D, 0xC80);
    Func_020039d2(0xF0);
    Func_02003aa4(0xA, 0, 0x1E);
    Func_02003a50(5, 0x01DB0000, 0x014C0000);
    Func_02003a5c(9, 0x01EB0000, 0x014C0000);
    Func_02003a68(0xB, 0x01CB0000, 0x015C0000);
    Func_02003a74(0xA, 0x01FB0000, 0x015C0000);
    Func_02003a80(0xD, 0x01D70000, 0x01320000);
    Func_02003a8c(0xE, 0x01DF0000, 0x016A0000);
    Func_02003b00(5, 0x8000, 0);
    Func_02003b0c(9, 0x8000, 0);
    Func_02003b18(0xB, 0x8000, 0);
    Func_02003b24(0xA, 0x8000, 0);
    Func_02003b30(0xD, 0x8000, 0);
    Func_02003b3c(0xE, 0x8000, 0);
    actor = Func_02003a92_b(5);
    Func_02003b82(actor->positionX * 0x10000, -1,
                  actor->positionZ * 0x10000, 1);
    Func_02003b8e();
    Func_02003a94(0x28);
    Func_02003b2c(0xD, 2);
    Func_02003aa2(0x14);
    Func_02003b6a(0xD, 0);
    Func_02003b42(0xB, 2);
    Func_02003b84(0xB, 0, 0xA);
    Func_02003b34(0xE, 4);
    Func_02003b96(0xE, 0, 0x14);
    Func_02003b70(0xA, 0xB, 0x28);
    Func_02003b50(0xA, 4);
    Func_02003bb2(0xA, 0, 0xA);
    Func_02003bd6(5, 0x100, 0);
    Func_02003bd2(5, 0x3000, 0x28);
    Func_02003bd4(0x4005, 0, 0xA);
    Func_02003be8(0xA, 0xB000, 0);
    Func_02003bf4(0xB, 0xD000, 0x28);
    Func_02003bc6(0xA, 0xB, 0x28);
    Func_02003c18(0xA, 0x105, 0x28);
    Func_02003c14(0xA, 0xB000, 0xA);
    Func_02003c16(0xA, 0, 0xA);
    Func_02003c2a(0xA, 0x8000, 0xA);
    Func_02003bf2(0xB, 2);
    Func_02003c34(0xB, 0, 0xA);
    Func_02003be4(0xA, 3);
    Func_02003c46(0xA, 0, 0x28);
    Func_02003c5a(0xE, 0xB000, 0x3C);
    Func_02003c0a(0xE, 3);
    Func_02003c64(0xE, 0, 0xA);
    Func_02003c78(0xD, 0x3000, 0);
    Func_02003c84(5, 0x3000, 0);
    Func_02003c90(9, 0x5000, 0);
    Func_02003c9c(0xB, 0x3000, 0);
    Func_02003ca8(0xA, 0x5000, 0x14);
    Func_02003cb4(0xE, 0xD000, 0x14);
    Func_02003c64(0xE, 4);
    Func_02003cbe(0xE, 0, 0xA);
    Func_02003ce0(0xD, 0x103, 0);
    Func_02003c3e(0xD, 0x10000, 0x8000);
    Func_02003c72(0xD, 0x1D7, 0x13A);
    Func_02003caa(0xD, 3);
    Func_02003cf4(0xD, 0, 0xA);
    Func_02003d08(0xE, 0xB000, 0xA);
    Func_02003cb0(0xE, 4);
    Func_02003d12(0xE, 0, 0xA);
    Func_02003cda(0xD, 3);
    Func_02003d24(0xD, 0, 0xA);
    Func_02003cf4(9, 2);
    Func_02003d36(0x4009, 0, 0xA);
    Func_02003d4a(0xA, 0xB000, 0x14);
    Func_02003d4c(0xA, 0, 0x1E);
    Func_02003d5e(0xB, 0, 0x14);
    Func_02003d26(0xB, 2);
    Func_02003c9c(0x1E);
    Func_02003d1c(0xB, 3);
    Func_02003caa(0x1E);
    Func_02003d86(0xB, 0xD000, 0x1E);
    Func_02003d36(0xB, 4);
    Func_02003cc4(0xA);
    Func_02003d96(0xB, 0, 0xA);
    Func_02003daa(0xD, 0x5000, 0x14);
    Func_02003dc0(0xD, 2);
    Func_02003d36(0x14);
    Func_02003dd8(0xA, 0xB, 0);
    Func_02003d46(0x1E);
    Func_02003dbe(0xA, 3);
    Func_02003dce(0xB, 3);
    Func_02003e32(0xB, 0xD000, 0);
    Func_02003e3e(0xA, 0xB000, 0);
    Func_02003d74(0x28);
    Func_02003e0c(0xD, 1);
    Func_02003d82(0x1E);
    Func_02003e5e(0xD, 0x8000, 0);
    Func_02003d94(0x28);
    Func_02003e80(0xD, 0x102, 0);
    Func_02003da6(0x3C);
    Func_02003e82(0xE, 0xB000, 0);
    Func_02003db8(0x1E);
    Func_02003e38(0xE, 4);
    Func_02003e92(0x200E, 0, 0x1E);
    Func_02003ea6(9, 0xB000, 0);
    Func_02003eb2(5, 0xB000, 0);
    Func_02003ebe(0xD, 0x3000, 0x14);
    Func_02003eda(5, 0x102, 0x28);
    Func_02003ecc(0x2005, 0, 0x28);
    Func_02003eee(0xD, 0x105, 0x3C);
    Func_02003e8e(0xD, 3);
    Func_02003f00(0xA, 0x101, 0);
    Func_02003f0a(0xB, 0x101, 0x3C);
    Func_02003ec2(0xE, 2);
    Func_02003f04(0xE, 0, 0x3C);
    Func_02003ebc(0xD, 3);
    Func_02003e4a(0x28);
    Func_02003f36(5, 0x102, 0);
    Func_02003f42(9, 0x102, 0x28);
    Func_02003f04(0xA, 0xB, 0x14);
    Func_02003ee4(0xA, 3);
    Func_02003ef4(0xB, 3);
    Func_02003e82(0x14);
    Func_02003ebc(0xA, 0x9999, 0x4CCC);
    Func_02003ec6(0xB, 0x9999, 0x4CCC);
    Func_02003ef2(0xB, 0x1DB, 0x15C);
    Func_02003efe(0xA, 0x1EB, 0x15C);
    Func_02003f14(0xB);
    Func_02003f1a(0xA);
    Func_02003f32(0xB, 1);
    Func_02003f3a(0xA, 1);
    Func_02003fa6(0xB, 0xD000, 0);
    Func_02003fb2(0xA, 0xB000, 0);
    Func_02003fbe(0xD, 0x3000, 0xA);
    Func_02003fd8(0xB, 0x103, 0);
    Func_02003f90(0xB, 2);
    Func_02003fc8(0x200B, 0);
    Func_02003f42(0xB, 0x19999, 0xCCCC);
    Func_02003f4c(5, 0x13333, 0x9999);
    Func_02003f80(0xB, 0x1DB, 0x152);
    configuredActor = Func_02003f4e(0xB);
    configuredActor->flags &= 0xFE;
    Func_02003fa0(0xB, 0x1DB, 0x15C);
    Func_02003fe0(5, 1);
    Func_02003fda(5, 4, 0);
    Func_02003fbe(5, 0x1CB, 0x13C);
    Func_02003fa4(5, 0x8000, 0x4000);
    Func_0200405e(0xD, 0x103, 0);
    Func_0200400e(0xD, 3);
    Func_02004058(0xD, 0, 0x1E);
    Func_02004028(0xB, 1);
    Func_02003f9e(0x14);
    configuredActor->flags |= 1;
    Func_02003fe4(0xB, 0x10000, 0x8000);
    Func_02004018(0xB, 0x1DB, 0x14C);
    Func_0200409c(0xB, 0xB000, 0x14);
    Func_02004094(0x200B, 0);
    Func_020040b0(0xA, 0xB000, 0);
    Func_02004070(0xA, 2);
    Func_020040ba(0xA, 0, 0x14);
    Func_020040de(0xB, 0x102, 0x14);
    Func_020040da(5, 0x3000, 0);
    Func_020040e6(0xB, 0x3000, 0x14);
    Func_02004096(0xA, 4);
    Func_0200409e(0xB, 3);
    Func_02004102(0xB, 0xB000, 0x14);
    Func_020040b2(0xB, 3);
    Func_020040ba(0xD, 3);
    Func_02004048(0x14);
    Func_020040a6(5, 0x1B0, 0x13C);
    Func_020040ba(0xD, 0x1A6, 0x137);
    Func_020040da(5, 1);
    Func_02004146(5, 0x3000, 0);
    Func_02004152(0xD, 0x3000, 0x14);
    Func_0200411a(0xA, 2);
    Func_02004166(0xA, 0xD000, 0x14);
    Func_02004168(0x100A, 0, 0x28);
    Func_02004138(9, 2);
    Func_020040ae(0x14);
    Func_0200418a(9, 0xB000, 0x1E);
    Func_020040f8(9, 0x8000, 0x4000);
    Func_0200412c(9, 0x1EB, 0x128);
    Func_020041b0(9, 0x5000, 0x28);
    Func_0200411e(0xA, 0x10000, 0x8000);
    Func_0200412c(0xB, 0x10000, 0x8000);
    Func_0200413a(0xD, 0x10000, 0x8000);
    Func_02004148(0xE, 0x10000, 0x8000);
    Func_0200417c(0xA, 0x1D7, 0x134);
    Func_02004200(0xA, 0x5000, 0);
    Func_02004194(0xB, 0x1C7, 0x134);
    Func_02004218(0xB, 0x3000, 0);
    Func_020041ac(0xE, 0x1E7, 0x134);
    Func_02004230(0xA, 0xC000, 0);
    Func_0200423c(0xB, 0xD000, 0);
    Func_02004248(0xE, 0xB000, 0);
    Func_02004254(5, 0xD000, 0);
    Func_02004260(0xD, 0xD000, 0);
    Func_0200428c(0x8000, 0x1000);
    Func_020042a6(0x01D80000, -1, 0x012C0000, 1);
    /* Final convergence and scene teardown. */
    Func_020042b2();
    Func_02004206(0x28);
    Func_02004204(0x246);
    Func_0200424a(0xA, 0x8000, 0x4000);
    Func_02004258(0xB, 0x8000, 0x4000);
    Func_02004266(0xE, 0x8000, 0x4000);
    Func_020042b6(0xA, 3);
    Func_02001ba8(0xA);
    Func_020042dc(9, 2);
    Func_02001bb6(9);
    Func_020042d2(0xB, 3);
    Func_02001bc4(0xB);
    Func_0200433c(5, 0x9000, 0x28);
    Func_020042fc(5, 2);
    Func_02004346(0x2005, 0, 0x28);
    Func_0200435a(0xD, 0x3000, 0);
    Func_02004366(5, 0xB000, 0x1E);
    Func_02004372(0xD, 0xE000, 0);
    Func_02001c0c(5);
    Func_02001c12(0xD);
    Func_0200438a(0xE, 0x7000, 0x28);
    Func_0200438c(0xE, 0, 0x1E);
    Func_02004344(0xE, 4);
    Func_020042d2(0x14);
    Func_020043a4(0xE, 0, 0x1E);
    Func_02001c46(0xE);
    Func_0200222a();
    Func_020042fc(5);
    return;
}
