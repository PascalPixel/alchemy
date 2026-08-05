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
extern void Func_02000e30();
extern void Func_02000f64();
extern void Func_02001084();
extern void Func_02001410();
extern void Func_02002d48();
extern void Func_080000c0();
extern void Func_080000d0();
extern void Func_08009128();
extern void Func_080091e0();
extern void Func_080091f0();
extern void Func_080091f8();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a058();
extern s32 Func_0808a070();
extern void * Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e8();
extern void Func_0808a0f0();
extern void Func_0808a100();
extern void Func_0808a110();
extern void Func_0808a128();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a150();
extern void Func_0808a170();
extern void Func_0808a178();
extern void Func_0808a180();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a1e8();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_0808a330();
extern void Func_0808a348();
extern void Func_0808a360();
extern void Func_0808a370();
extern void Func_080f9010();

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
    Func_0808a018();
    Func_080f9010(0x8D);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    *firstPhase = 0;
    Func_0808a0f0(0, 0xE80000, 0x9C0000);
    Func_0808a0f0(1, 0xDA0000, 0xAC0000);
    Func_0808a1b8(0, 0xB000, 0);
    Func_0808a1b8(1, 0xD000, 0);
    Func_0808a0f0(5, 0x01DB0000, 0x014C0000);
    Func_0808a0f0(9, 0x01EB0000, 0x014C0000);
    Func_0808a0f0(0xB, 0x01CB0000, 0x015C0000);
    Func_0808a0f0(0xA, 0x01FB0000, 0x015C0000);
    Func_0808a0f0(0xD, 0x01D70000, 0x01320000);
    Func_0808a0f0(0xE, 0x01DF0000, 0x016A0000);
    Func_0808a1e0(0xF, 1);
    Func_0808a210(0xE80000, -1, 0x9C0000, 0);
    Func_08009128();
    Func_0808a360();
    Func_0808a370();
    Func_0808a330(0x7FFF, 0);
    (*(struct MainSceneState **)((u8 *)sceneSlot - 8))->activeActorLimit = 8;
    Func_0808a348(4);
    Func_0808a010(4);
    Func_0808a330(0x10000, 0);
    Func_0808a348(4);
    Func_0808a010(4);
    Func_02000f64();
    Func_0808a330(0x7FFF, 0);
    Func_0808a348(4);
    Func_0808a010(0x10);
    Func_080f9010(0x90);
    Func_0808a330(0x10000, 0);
    Func_0808a348(4);
    Func_0808a010(4);
    Func_0808a330(0x7FFF, 0);
    Func_0808a348(4);
    Func_0808a010(4);
    Func_080f9010(0x90);
    Func_0808a330(0x10000, 0);
    Func_0808a348(0x30);
    Func_0808a010(0x30);
    Func_0808a128(0, 6, 0);
    Func_0808a128(1, 6, 0x14);
    Func_02002d48(1, 0x14, 0x14);
    Func_02002d48(0, 0x14, 0x28);
    Func_0808a170(0x10CD);
    Func_0808a188(0xB, 0, 0x14);
    Func_0808a180(0xA, 0);
    Func_02002d48(1, 0x14, 0);
    Func_0808a188(5, 0, 0x14);
    Func_02002d48(0, 0x14, 0);
    Func_0808a188(0xE, 0, 0x14);
    Func_0808a188(9, 0, 0x14);
    Func_0808a150(0, 1, 0);
    Func_0808a010(0x28);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(0x14);
    Func_0808a1b8(0, 0xB000, 0);
    Func_0808a1b8(1, 0xD000, 0x1E);
    Func_02002d48(1, 0x14, 0);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(0x14);
    Func_02002d48(0, 0x14, 0x14);
    Func_080091e0(Func_0808a080(0xF), 0);
    Func_0808a0f0(0xF, 0x01450000, 0x012E0000);
    ((struct SceneActor *)Func_0808a080(0xF))->behavior = 5;
    /*
     * Phase 1: run the first staged exchange. Calls in the 020037xx-020039xx
     * family repeatedly address the same six actor slots with fixed-point
     * positions and short transition durations.
     */
    *firstPhase = 1;
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xE666);
    Func_080091f8();
    Func_0808a010(0x96);
    Func_0808a188(0xB, 0, 0x14);
    Func_0808a180(5, 0);
    Func_0808a188(0xA, 0, 0xA);
    Func_0808a150(0, 1, 0x14);
    Func_0808a1b8(0, 0x3000, 0);
    Func_0808a1b8(1, 0x3000, 0xA);
    Func_0808a0f0(5, 0x01DB0000, 0x014C0000);
    Func_0808a0f0(9, 0x01EB0000, 0x014C0000);
    Func_0808a0f0(0xB, 0x01CB0000, 0x015C0000);
    Func_0808a0f0(0xA, 0x01FB0000, 0x015C0000);
    Func_0808a0f0(0xD, 0x01D70000, 0x01320000);
    Func_0808a0f0(0xE, 0x01DF0000, 0x016A0000);
    Func_0808a208(0x66666, 0xCCCC);
    Func_0808a210(0x01480000, -1, 0x012B0000, 1);
    Func_0808a218();
    Func_080f9010(0xA7);
    Func_0808a330(0x205294, 2);
    Func_0808a348(0x14);
    Func_080000c0(0x14);
    Func_0808a330(0x10000, 2);
    Func_0808a348(0x14);
    Func_0808a010(0xC8);
    Func_0808a178(0x1001, 0);
    if (Func_0808a070(0, 0) != 0) {
        goto block_2;
    }
    Func_0808a170(0x10D6);
    goto block_3;
block_2:
    Func_0808a170(0x10D7);
block_3:
    Func_0808a188(0x1001, 0, 0x50);
    Func_0808a170(0x10D8);
    Func_0808a188(9, 0, 0x14);
    Func_02002d48(1, 0x14, 0);
    /* Phase 2 setup: clear the phase latch and rebuild the formation. */
    secondPhase = &scene->scriptPhase;
    *secondPhase = 0;
    Func_080f9010(0x8D);
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(0x50);
    /*
     * Phase 2 execution: the long 02003axx-020043xx sequence composes actor
     * motion, flags, animation selections, and waits before final handoff.
     */
    *secondPhase = 1;
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xE666);
    Func_080091f8();
    Func_02002d48(0, 0x14, 0x3C);
    Func_0808a188(0xE, 0, 0x1E);
    Func_0808a1b8(0xF, 0xA000, 0x28);
    Func_02002d48(1, 0x14, 0x14);
    Func_02002d48(0, 0x14, 0x14);
    Func_0808a188(0x1001, 0, 0x1E);
    Func_0808a1b8(0xF, 0x1000, 0x28);
    Func_02002d48(1, 0x14, 0x14);
    Func_02002d48(0, 0x14, 0x14);
    Func_0808a188(5, 0, 0x1E);
    Func_02001084();
    Func_080000d0(0x020090C5, 0xC80);
    Func_080000d0(0x0200935D, 0xC80);
    Func_0808a010(0xF0);
    Func_0808a188(0xA, 0, 0x1E);
    Func_0808a0f0(5, 0x01DB0000, 0x014C0000);
    Func_0808a0f0(9, 0x01EB0000, 0x014C0000);
    Func_0808a0f0(0xB, 0x01CB0000, 0x015C0000);
    Func_0808a0f0(0xA, 0x01FB0000, 0x015C0000);
    Func_0808a0f0(0xD, 0x01D70000, 0x01320000);
    Func_0808a0f0(0xE, 0x01DF0000, 0x016A0000);
    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a1b8(9, 0x8000, 0);
    Func_0808a1b8(0xB, 0x8000, 0);
    Func_0808a1b8(0xA, 0x8000, 0);
    Func_0808a1b8(0xD, 0x8000, 0);
    Func_0808a1b8(0xE, 0x8000, 0);
    actor = Func_0808a080(5);
    Func_0808a210(actor->positionX * 0x10000, -1,
                  actor->positionZ * 0x10000, 1);
    Func_0808a218();
    Func_0808a010(0x28);
    Func_0808a138(0xD, 2);
    Func_0808a010(0x14);
    Func_0808a180(0xD, 0);
    Func_0808a138(0xB, 2);
    Func_0808a188(0xB, 0, 0xA);
    Func_0808a100(0xE, 4);
    Func_0808a188(0xE, 0, 0x14);
    Func_0808a150(0xA, 0xB, 0x28);
    Func_0808a100(0xA, 4);
    Func_0808a188(0xA, 0, 0xA);
    Func_0808a1e8(5, 0x100, 0);
    Func_0808a1b8(5, 0x3000, 0x28);
    Func_0808a188(0x4005, 0, 0xA);
    Func_0808a1b8(0xA, 0xB000, 0);
    Func_0808a1b8(0xB, 0xD000, 0x28);
    Func_0808a150(0xA, 0xB, 0x28);
    Func_0808a1e8(0xA, 0x105, 0x28);
    Func_0808a1b8(0xA, 0xB000, 0xA);
    Func_0808a188(0xA, 0, 0xA);
    Func_0808a1b8(0xA, 0x8000, 0xA);
    Func_0808a138(0xB, 2);
    Func_0808a188(0xB, 0, 0xA);
    Func_0808a100(0xA, 3);
    Func_0808a188(0xA, 0, 0x28);
    Func_0808a1b8(0xE, 0xB000, 0x3C);
    Func_0808a110(0xE, 3);
    Func_0808a188(0xE, 0, 0xA);
    Func_0808a1b8(0xD, 0x3000, 0);
    Func_0808a1b8(5, 0x3000, 0);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a1b8(0xB, 0x3000, 0);
    Func_0808a1b8(0xA, 0x5000, 0x14);
    Func_0808a1b8(0xE, 0xD000, 0x14);
    Func_0808a110(0xE, 4);
    Func_0808a188(0xE, 0, 0xA);
    Func_0808a1e8(0xD, 0x103, 0);
    Func_0808a090(0xD, 0x10000, 0x8000);
    Func_0808a0d0(0xD, 0x1D7, 0x13A);
    Func_0808a130(0xD, 3);
    Func_0808a188(0xD, 0, 0xA);
    Func_0808a1b8(0xE, 0xB000, 0xA);
    Func_0808a100(0xE, 4);
    Func_0808a188(0xE, 0, 0xA);
    Func_0808a130(0xD, 3);
    Func_0808a188(0xD, 0, 0xA);
    Func_0808a138(9, 2);
    Func_0808a188(0x4009, 0, 0xA);
    Func_0808a1b8(0xA, 0xB000, 0x14);
    Func_0808a188(0xA, 0, 0x1E);
    Func_0808a1b8(0xB, 0, 0x14);
    Func_0808a138(0xB, 2);
    Func_0808a010(0x1E);
    Func_0808a110(0xB, 3);
    Func_0808a010(0x1E);
    Func_0808a1b8(0xB, 0xD000, 0x1E);
    Func_0808a110(0xB, 4);
    Func_0808a010(0xA);
    Func_0808a188(0xB, 0, 0xA);
    Func_0808a1b8(0xD, 0x5000, 0x14);
    Func_0808a138(0xD, 2);
    Func_0808a010(0x14);
    Func_0808a150(0xA, 0xB, 0);
    Func_0808a010(0x1E);
    Func_0808a100(0xA, 3);
    Func_0808a110(0xB, 3);
    Func_0808a1b8(0xB, 0xD000, 0);
    Func_0808a1b8(0xA, 0xB000, 0);
    Func_0808a010(0x28);
    Func_0808a138(0xD, 1);
    Func_0808a010(0x1E);
    Func_0808a1b8(0xD, 0x8000, 0);
    Func_0808a010(0x28);
    Func_0808a1e8(0xD, 0x102, 0);
    Func_0808a010(0x3C);
    Func_0808a1b8(0xE, 0xB000, 0);
    Func_0808a010(0x1E);
    Func_0808a110(0xE, 4);
    Func_0808a188(0x200E, 0, 0x1E);
    Func_0808a1b8(9, 0xB000, 0);
    Func_0808a1b8(5, 0xB000, 0);
    Func_0808a1b8(0xD, 0x3000, 0x14);
    Func_0808a1e8(5, 0x102, 0x28);
    Func_0808a188(0x2005, 0, 0x28);
    Func_0808a1e8(0xD, 0x105, 0x3C);
    Func_0808a110(0xD, 3);
    Func_0808a1e8(0xA, 0x101, 0);
    Func_0808a1e8(0xB, 0x101, 0x3C);
    Func_0808a138(0xE, 2);
    Func_0808a188(0xE, 0, 0x3C);
    Func_0808a110(0xD, 3);
    Func_0808a010(0x28);
    Func_0808a1e8(5, 0x102, 0);
    Func_0808a1e8(9, 0x102, 0x28);
    Func_0808a150(0xA, 0xB, 0x14);
    Func_0808a100(0xA, 3);
    Func_0808a110(0xB, 3);
    Func_0808a010(0x14);
    Func_0808a090(0xA, 0x9999, 0x4CCC);
    Func_0808a090(0xB, 0x9999, 0x4CCC);
    Func_0808a0c8(0xB, 0x1DB, 0x15C);
    Func_0808a0c8(0xA, 0x1EB, 0x15C);
    Func_0808a0e8(0xB);
    Func_0808a0e8(0xA);
    Func_0808a100(0xB, 1);
    Func_0808a100(0xA, 1);
    Func_0808a1b8(0xB, 0xD000, 0);
    Func_0808a1b8(0xA, 0xB000, 0);
    Func_0808a1b8(0xD, 0x3000, 0xA);
    Func_0808a1e8(0xB, 0x103, 0);
    Func_0808a138(0xB, 2);
    Func_0808a180(0x200B, 0);
    Func_0808a090(0xB, 0x19999, 0xCCCC);
    Func_0808a090(5, 0x13333, 0x9999);
    Func_0808a0d0(0xB, 0x1DB, 0x152);
    configuredActor = Func_0808a080(0xB);
    configuredActor->flags &= 0xFE;
    Func_0808a0d0(0xB, 0x1DB, 0x15C);
    Func_0808a138(5, 1);
    Func_0808a128(5, 4, 0);
    Func_0808a0d0(5, 0x1CB, 0x13C);
    Func_0808a090(5, 0x8000, 0x4000);
    Func_0808a1e8(0xD, 0x103, 0);
    Func_0808a130(0xD, 3);
    Func_0808a188(0xD, 0, 0x1E);
    Func_0808a138(0xB, 1);
    Func_0808a010(0x14);
    configuredActor->flags |= 1;
    Func_0808a090(0xB, 0x10000, 0x8000);
    Func_0808a0d0(0xB, 0x1DB, 0x14C);
    Func_0808a1b8(0xB, 0xB000, 0x14);
    Func_0808a180(0x200B, 0);
    Func_0808a1b8(0xA, 0xB000, 0);
    Func_0808a130(0xA, 2);
    Func_0808a188(0xA, 0, 0x14);
    Func_0808a1e8(0xB, 0x102, 0x14);
    Func_0808a1b8(5, 0x3000, 0);
    Func_0808a1b8(0xB, 0x3000, 0x14);
    Func_0808a110(0xA, 4);
    Func_0808a110(0xB, 3);
    Func_0808a1b8(0xB, 0xB000, 0x14);
    Func_0808a110(0xB, 3);
    Func_0808a110(0xD, 3);
    Func_0808a010(0x14);
    Func_0808a0c8(5, 0x1B0, 0x13C);
    Func_0808a0d0(0xD, 0x1A6, 0x137);
    Func_0808a100(5, 1);
    Func_0808a1b8(5, 0x3000, 0);
    Func_0808a1b8(0xD, 0x3000, 0x14);
    Func_0808a138(0xA, 2);
    Func_0808a1b8(0xA, 0xD000, 0x14);
    Func_0808a188(0x100A, 0, 0x28);
    Func_0808a138(9, 2);
    Func_0808a010(0x14);
    Func_0808a1b8(9, 0xB000, 0x1E);
    Func_0808a090(9, 0x8000, 0x4000);
    Func_0808a0d0(9, 0x1EB, 0x128);
    Func_0808a1b8(9, 0x5000, 0x28);
    Func_0808a090(0xA, 0x10000, 0x8000);
    Func_0808a090(0xB, 0x10000, 0x8000);
    Func_0808a090(0xD, 0x10000, 0x8000);
    Func_0808a090(0xE, 0x10000, 0x8000);
    Func_0808a0d0(0xA, 0x1D7, 0x134);
    Func_0808a1b8(0xA, 0x5000, 0);
    Func_0808a0d0(0xB, 0x1C7, 0x134);
    Func_0808a1b8(0xB, 0x3000, 0);
    Func_0808a0d0(0xE, 0x1E7, 0x134);
    Func_0808a1b8(0xA, 0xC000, 0);
    Func_0808a1b8(0xB, 0xD000, 0);
    Func_0808a1b8(0xE, 0xB000, 0);
    Func_0808a1b8(5, 0xD000, 0);
    Func_0808a1b8(0xD, 0xD000, 0);
    Func_0808a208(0x8000, 0x1000);
    Func_0808a210(0x01D80000, -1, 0x012C0000, 1);
    /* Final convergence and scene teardown. */
    Func_0808a218();
    Func_0808a010(0x28);
    Func_080770c8(0x246);
    Func_0808a090(0xA, 0x8000, 0x4000);
    Func_0808a090(0xB, 0x8000, 0x4000);
    Func_0808a090(0xE, 0x8000, 0x4000);
    Func_0808a110(0xA, 3);
    Func_02000e30(0xA);
    Func_0808a138(9, 2);
    Func_02000e30(9);
    Func_0808a110(0xB, 3);
    Func_02000e30(0xB);
    Func_0808a1b8(5, 0x9000, 0x28);
    Func_0808a130(5, 2);
    Func_0808a188(0x2005, 0, 0x28);
    Func_0808a1b8(0xD, 0x3000, 0);
    Func_0808a1b8(5, 0xB000, 0x1E);
    Func_0808a1b8(0xD, 0xE000, 0);
    Func_02000e30(5);
    Func_02000e30(0xD);
    Func_0808a1b8(0xE, 0x7000, 0x28);
    Func_0808a188(0xE, 0, 0x1E);
    Func_0808a110(0xE, 4);
    Func_0808a010(0x14);
    Func_0808a188(0xE, 0, 0x1E);
    Func_02000e30(0xE);
    Func_02001410();
    Func_0808a058(5);
    return;
}
