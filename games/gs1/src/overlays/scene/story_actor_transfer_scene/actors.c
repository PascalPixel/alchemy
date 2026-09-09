#include "types.h"

#define StoryActor_AdvanceTimer Func_0200011c
#define StoryActor_ConfigureSpawnedObject Func_0200013c
#define StoryActor_Initialize Func_02000250
#define StoryActor_ApplyFlaggedMode Func_020002a4
#define StoryActor_ResetPosition Func_02000314
#define StoryActor_ClearActiveFlag Func_02000350

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} StorySpawnRecord;

extern u32 Data_03001e40;
extern u8 Data_0200811d[];
extern u8 Data_020081c5[];
extern u8 Value_00000847;
extern u8 Value_000002f1;

void Func_02004348();
u8 *Func_02004378();
void Func_02004618();
void Func_020043dc();
void Func_0200439c();
s32 Func_020044e4(s32);
s32 Func_020044f0(s32);
void Func_02000576(void);
s32 Func_0200453c(s32);
void Func_020044b8(u8 *, s32);
void Func_02004560(u8 *, s32);
void Func_02004580(u8 *, s32);
s32 Func_020045c0(s32);
void Func_020045d2(s32);

void StoryActor_AdvanceTimer(u8 *actor)
{
    u16 *timer = (u16 *)(actor + 0x64);

    /* Arm order decides the branch sense: the reference falls through to the
     * increment and branches away to the call, which is `bgt`. */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Func_02004348(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{
    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((Data_03001e40 & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((Data_03001e40 & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Func_02004378(0x11d, x, y, z);
    }
    Func_02004618(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Func_020043dc(spawned_actor, 0);
        Func_0200439c(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)Data_0200811d;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    u8 *actor_flags;
    s32 fixed_scale;

    if (Func_020044e4(0x30) != 0) {
        return 0;
    }
    if (Func_020044f0(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)Data_020081c5;
    actor_flags = actor + 0x55;
    *actor_flags = 0;
    actor_flags += 0xF;
    *(u16 *)actor_flags = 0;
    actor_flags += 2;
    *(u16 *)actor_flags = 0;
    fixed_scale = 0x8000;
    *(s32 *)(actor + 0x18) = fixed_scale;
    *(s32 *)(actor + 0x1C) = fixed_scale;
    return 0;
}

/*
 * Resource 371 owner at 0x02000350 (44 bytes, 0 calls).
 *
 * Complete owner: `push {lr}` at 0x02000350 and `pop {r1} ; bx r1` at
 * 0x02000372.  The popped register is r1, not r0, so r0 survives the return
 * and IS the result -- the constant 1 set at 0x02000370.  Bytes
 * 0x02000376-0x0200037b are alignment plus the single literal pool word
 * 0x03001e40; they are reached only by `ldr r3, [pc, #20]`.
 *
 * 0x03001e40 is an IWRAM word this overlay reads all over (see the sibling
 * owners at 0x02003f10/0x02003f4c/0x02003f88, which mask it with 1, 2 and
 * 0xf).  It is a live status/flag word, not overlay image data.
 *
 * The flag byte at object + 0x54 is cleared with `eors` against the same
 * bit that was just tested, which is a clear of bit 0 only.
 *
 * The range-proven `one` expression and the identical `flags` branches are
 * intentional source shape.  Since `value` is a byte, `value >> 8` is zero;
 * both branches perform the same clear and store.  This form makes the routed
 * GCC allocate the constant to r1, the loaded byte to r2, and the clear result
 * to r3 without changing the C behavior.
 */
s32 StoryActor_ApplyFlaggedMode(u8 *actor)
{
    Func_02000576();
    if (Func_0200453c((s32)&Value_00000847) != 0) {
        Func_020044b8(actor, 2);
    }
    return 1;
}

s32 StoryActor_ResetPosition(u8 *actor)
{
    s32 zero;
    Func_02004560(actor, 0);
    Func_02004580(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (Func_020045c0(0x8A0) != 0) {
        Func_020045d2((s32)&Value_000002f1);
        *(s32 *)(actor + 8) = zero;
        *(s32 *)(actor + 12) = zero;
    }
    return 0;
}

s32 StoryActor_ClearActiveFlag(u8 *actor)
{
    u8 *active_flags = actor + 0x54;
    u8 value = *active_flags;
    u32 active_bit = 1 + (value >> 8);

    if ((active_bit & value) != 0 && (Data_03001e40 & active_bit) != 0) {
        u8 cleared = 1;

        if (active_flags) {
            cleared ^= value;
            *active_flags = (u8)cleared;
        } else {
            cleared ^= value;
            *active_flags = (u8)cleared;
        }
    }
    return 1;
}
