#include "layout_guard.h"
#include "types.h"

/*
 * Player walk step: pick a heading from the direction pad, sweep it for map
 * collision, push or refuse against other live objects, then commit the new
 * velocity and drive the walk animation.
 *
 * The owner is one function spread over four manifest rows, because two
 * literal pools sit inside its body and a third boundary falls in the middle
 * of a straight-line instruction sequence:
 *
 *   0x0800ebec .. 0x0800edf0   code   (head; pool 0x0800edf0..0x0800ee14)
 *   0x0800ee14 .. 0x0800f186   code   (candidate scan and object pass)
 *                                     (pool 0x0800f186..0x0800f1a4)
 *   0x0800f1a4 .. 0x0800f1bc   code   (commit, first interworking call)
 *   0x0800f1bc .. 0x0800f1fa   code   (continuation of the same statement)
 *   0x0800f1fa .. 0x0800f2f6   code   (tail; 0x0800f2ae..0x0800f2c0 and
 *                                      0x0800f2d4..0x0800f2d8 are pools)
 *
 * The 0x0800f1bc boundary is a disassembly artefact, not a control-flow one.
 * At 0x0800f1b8 the original does `mov ip, pc ; bx r4` with r4 holding
 * 0x03000118, the IWRAM-relocated ARM multiply.  `mov ip, pc` supplies the
 * return address of the instruction that follows `bx r4`, so control resumes
 * at 0x0800f1bc with the frame and every live register unchanged: it is an
 * ordinary call through a fixed function pointer.  The same idiom repeats at
 * 0x0800f1c4 (returning to 0x0800f1c8), preceded by the `movs r0, r0`
 * alignment filler at 0x0800f1c2.  Only the *encoding* is unusual - GCC 2.96
 * would spell the identical call `bl __call_via_r4` - so both are written
 * here as plain indirect calls.  Cross-checked against 0x08097a10 and
 * 0x0800cacc, which use the same sequence with the same 0x03000118 operand
 * and fall through to the next Thumb instruction.
 *
 * Layout note: the owner's object type is byte-for-byte the 0x70-byte record
 * already reconstructed as Object_0800c150.  The names below follow this
 * function's use of the fields, which disagrees with two of that file's
 * guesses: offset 0x20 is used here as a collision radius rather than a frame
 * count, and 0x64/0x66 as an animation timer and phase rather than cell
 * coordinates.
 */

typedef struct Object_0800ebec {
    const void *program_00;
    u16 step_04;
    u16 heading_06;
    s32 position_08[3];      /* x, y, z */
    s32 ground_14;
    u8 unknown_18[8];
    u16 radius_20;
    u16 unknown_22;
    s32 velocity_24[3];      /* x, y, z; 0x24 and 0x2c are rebuilt below */
    s32 speed_30;
    s32 acceleration_34;
    s32 pushback_38[3];
    u8 unknown_44[0x14];
    u32 class_58;            /* byte 0x59 carries the live/solid bits */
    u8 unknown_5c[8];
    s16 animation_timer_64;
    s16 animation_phase_66;
    u8 unknown_68[8];
} Object_0800ebec;

LAYOUT_SIZE_GUARD(Object0800ebec_Size, Object_0800ebec, 0x70);
LAYOUT_OFFSET_GUARD(Object0800ebec_Radius, Object_0800ebec, radius_20, 0x20);
LAYOUT_OFFSET_GUARD(Object0800ebec_Velocity, Object_0800ebec, velocity_24, 0x24);
LAYOUT_OFFSET_GUARD(Object0800ebec_Class, Object_0800ebec, class_58, 0x58);
LAYOUT_OFFSET_GUARD(
    Object0800ebec_Timer, Object_0800ebec, animation_timer_64, 0x64);

/* Sprite record hung off the spawned dust effect (Object_0800c150 +0x50). */
typedef struct Sprite_0800ebec {
    u8 unknown_00[9];
    u8 flags_09;
    u8 unknown_0a[0x14];
    u16 angle_1e;
    u8 unknown_20[6];
    u8 palette_26;
} Sprite_0800ebec;

/* Spawned effect object; only the fields this owner writes are named. */
typedef struct Effect_0800ebec {
    u8 unknown_00[6];
    u16 angle_06;
    u8 unknown_08[0x0c];
    s32 ground_14;
    u8 unknown_18[0x0b];
    u8 kind_23;
    u8 unknown_24[0x2c];
    struct Sprite_0800ebec *sprite_50;
    u8 unknown_54;
    u8 flags_55;
} Effect_0800ebec;

LAYOUT_OFFSET_GUARD(Effect0800ebec_Kind, Effect_0800ebec, kind_23, 0x23);
LAYOUT_OFFSET_GUARD(Effect0800ebec_Sprite, Effect_0800ebec, sprite_50, 0x50);
LAYOUT_OFFSET_GUARD(Effect0800ebec_Flags, Effect_0800ebec, flags_55, 0x55);

/* Per-frame statistics block; +0x19c counts consecutive blocked steps. */
typedef struct Stats_0800ebec {
    u8 unknown_000[0x19c];
    u16 blocked_steps_19c;
} Stats_0800ebec;

/* 0x03000118 is the ARM multiply relocated into IWRAM.  Reached inline with
   `mov ip, pc ; bx r4`; see the header comment. */
typedef s32 (*Multiply_0800ebec)(s32 a, s32 b);
#define MULTIPLY_0800EBEC ((Multiply_0800ebec)0x03000118)

/* Input mirror; bits 4..7 select the direction-pad heading. */
extern u32 Data_03001ae8;
/* Non-zero while the walk system is allowed to move the player. */
extern u8 Data_03001f54;
/* 512-byte slot map scanned for free (0xff) entries. */
extern u8 Data_03001810[512];
/* Live object table: 64 records of 0x70 bytes. */
extern Object_0800ebec *Data_03001e64;
extern Stats_0800ebec *Data_03001ebc;
/* Global progress block; +0x17 gates the dust effect. */
extern u8 *Data_03001e70;
/* Heading per direction-pad combination; 0xffff means "no direction". */
extern const s16 Data_08013254[16];
/* Owner-state block; +0x1f4 holds the active party leader handle and
   +0x21c the mask of buttons that count as "run". */
extern u8 Data_02000240[];

void Func_0800447c(s32 distance, s32 angle, s32 position[3]);
s32 Func_080044d0(s32 dz, s32 dx);
s32 Func_080045d4(s32 squared_sum);
void Func_0800ba30(struct Sprite_0800ebec *sprite, s32 mode);
Effect_0800ebec *Func_0800c150(s32 packed_resource, s32 x, s32 y, s32 z);
void Func_0800c2d8(Effect_0800ebec *effect, const void *program);
void Func_0800c300(Object_0800ebec *object, s32 command);
void Func_0800d14c(Object_0800ebec *object, s32 x, s32 y, s32 z);
s32 Func_0800d924(const Object_0800ebec *object, const s32 position[3]);
s32 Func_0800eaf8(void);
s32 Func_0800eba0(const s32 other_position[3],
                  s32 self_radius,
                  const s32 self_position[3],
                  s32 other_radius);
/* Map probe.  The second argument is read as a position triple: only the
   high halfwords of [0] and [2] (the cell coordinates) are used. */
s32 Func_080120dc(const Object_0800ebec *object, const s32 position[3]);
u8 *Func_08077008(s32 handle);
s32 Func_080770c0(s32 query);
void Func_080f9010(s32 sound);

#define POSITION_MASK_0800EBEC     0xff000200u
#define POSITION_SOLID_0800EBEC    0x00000200u
#define OBJECT_LIVE_0800EBEC       0x00000100u  /* byte 0x59, bit 0 */

#define WALK_DISTANCE_0800EBEC     0x00080000
#define LOOK_DISTANCE_0800EBEC     0x00040000
#define PUSH_DISTANCE_0800EBEC     0x00004000
#define PUSH_PROBE_0800EBEC        0x000a0000

#define BLOCKED_NO_ROOM_0800EBEC   1
#define BLOCKED_OBJECT_0800EBEC    2
#define BLOCKED_NO_INPUT_0800EBEC  4

/* Countdowns the original spends inline before reading the input mirror
   again.  Kept as observable delays rather than being optimised away. */
static void Delay_0800ebec(s32 count)
{
    volatile s32 remaining = count;

    while (remaining != 0)
        remaining = remaining - 1;
}

static void DelaySigned_0800ebec(s32 count)
{
    volatile s32 remaining = count;

    while (remaining >= 0)
        remaining = remaining - 1;
}

static void CopyPosition_0800ebec(s32 destination[3], const s32 source[3])
{
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}

/*
 * Four flanking probes around `angle`, each a full walk step from the
 * object's own position.  Non-zero means the map refused one of them.
 */
static s32 ProbeRing_0800ebec(const Object_0800ebec *self,
                              s32 angle,
                              s32 probe[3])
{
    static const s32 offsets[4] = { 0x1000, -0x1000, 0x2000, -0x2000 };
    s32 index;

    for (index = 0; index < 4; index++) {
        CopyPosition_0800ebec(probe, self->position_08);
        Func_0800447c(WALK_DISTANCE_0800EBEC, angle + offsets[index], probe);
        if (Func_080120dc(self, probe) != 0)
            return 1;
    }
    return 0;
}

s32 Func_0800ebec(Object_0800ebec *self)
{
    s32 primary[3];        /* sp+0x5c: the accepted step position */
    s32 probe[3];          /* sp+0x50: scratch for the flanking probes */
    s32 forward[3];        /* sp+0x44: look-ahead used against other objects */
    s16 candidates[6];     /* sp+0x18 */
    s32 blocked_flags = 0; /* sp+0x14 */
    s32 pushed_flags = 0;  /* sp+0x10 */
    s32 walk_command;      /* sp+0x08 */
    s32 heading_word;      /* sp+0x04: requested heading, in the high half */
    s32 chosen_angle = 0;  /* sp+0x0c */
    s32 travel_word = 0;   /* sp+0x00: accepted heading, in the high half */
    s32 free_slots;
    s32 index;
    s32 angle;
    Object_0800ebec *other;
    Stats_0800ebec *stats;

    if (Data_03001f54 != 0 && Func_080770c0(0x15e) != 0) {
        free_slots = 0;
        for (index = 0; index < 512; index++) {
            if (Data_03001810[index] == 255)
                free_slots++;
        }
        if (free_slots < 136)
            Func_080f9010(135);
    }

    if (Data_03001f54 != 0 && (Data_03001ae8 & 0x200) != 0) {
        Delay_0800ebec(512);
        DelaySigned_0800ebec(95);
        DelaySigned_0800ebec(63);
        DelaySigned_0800ebec(63);
    }

    if ((Data_03001ae8 & *(u16 *)(Data_02000240 + 0x21c)) != 0) {
        self->speed_30 = 0x18000;
        self->acceleration_34 = 0x4000;
        walk_command = 5;
    } else {
        self->speed_30 = 0x10000;
        self->acceleration_34 = 0x4000;
        walk_command = 2;
    }

    if (Func_080770c0(0x17f) != 0 && (Data_03001ae8 & 2) != 0) {
        self->speed_30 = 0x40000;
        self->acceleration_34 = 0x10000;
        walk_command = 5;
    }

    heading_word = (s32)Data_08013254[(Data_03001ae8 >> 4) & 15] << 16;
    if (((u32)heading_word >> 16) == 0xffff) {
        /* No direction held: skip straight to the bookkeeping tail. */
        blocked_flags |= BLOCKED_NO_INPUT_0800EBEC;
        goto resolve;
    }

    blocked_flags = 0;
    angle = (heading_word >> 16) & 0xffff;
    CopyPosition_0800ebec(primary, self->position_08);
    Func_0800447c(WALK_DISTANCE_0800EBEC, angle, primary);

    if (Data_03001f54 != 0 && (Data_03001ae8 & 0x200) != 0) {
        chosen_angle = heading_word >> 16;
        goto resolve;
    }

    if (Func_080120dc(self, primary) == 0
        && ProbeRing_0800ebec(self, angle, probe) == 0) {
        chosen_angle = heading_word >> 16;
        travel_word = chosen_angle << 16;
    } else {
        /*
         * The requested heading is blocked.  Try six fallbacks fanned out to
         * either side, nearest first, and take the first that clears the same
         * five-ray sweep.
         */
        s32 base = (heading_word >> 16) & 0xffff;

        candidates[0] = (s16)(base + 0x1000);
        candidates[1] = (s16)(base - 0x1000);
        candidates[2] = (s16)(base + 0x2000);
        candidates[3] = (s16)(base - 0x2000);
        candidates[4] = (s16)(base + 0x3000);
        candidates[5] = (s16)(base - 0x3000);

        for (index = 0; index < 6; index++) {
            chosen_angle = candidates[index];
            travel_word = chosen_angle << 16;
            angle = chosen_angle & 0xffff;
            CopyPosition_0800ebec(primary, self->position_08);
            Func_0800447c(WALK_DISTANCE_0800EBEC, angle, primary);
            if (Func_080120dc(self, primary) != 0)
                continue;
            if (ProbeRing_0800ebec(self, angle, probe) != 0)
                continue;
            goto accepted;
        }

        /*
         * Every fallback failed.  `primary` reverts to the object's own
         * position, but chosen_angle/travel_word keep the last candidate -
         * the original leaves both stack slots as the loop left them.
         */
        CopyPosition_0800ebec(primary, self->position_08);
        blocked_flags |= BLOCKED_NO_ROOM_0800EBEC;
    }

accepted:
    /* Look one shorter step ahead and see who is standing there. */
    CopyPosition_0800ebec(forward, self->position_08);
    Func_0800447c(LOOK_DISTANCE_0800EBEC, (travel_word >> 16) & 0xffff, forward);

    other = Data_03001e64;
    for (index = 63; index >= 0; index--, other++) {
        s32 push_angle;
        s32 push_angle16;

        if (other->program_00 == 0)
            continue;
        if ((other->class_58 & OBJECT_LIVE_0800EBEC) == 0)
            continue;
        if (other == self)
            continue;
        if (Func_0800eba0(other->position_08,
                          (s32)self->radius_20 - 2,
                          forward,
                          (s32)other->radius_20 - 2) < 0)
            continue;

        if ((other->class_58 & POSITION_MASK_0800EBEC)
            != POSITION_SOLID_0800EBEC) {
            /* Solid scenery or an object that must not be pushed. */
            blocked_flags |= BLOCKED_OBJECT_0800EBEC;
            continue;
        }

        /* Direction from us to the object; that is the way it gets pushed. */
        push_angle = Func_080044d0(other->position_08[2] - self->position_08[2],
                                   other->position_08[0] - self->position_08[0]);
        push_angle16 = push_angle & 0xffff;
        /*
         * The original reuses the same stack slot for the accepted walk
         * heading, so a push here also redirects our own velocity below.
         */
        chosen_angle = (s32)(s16)push_angle;

        CopyPosition_0800ebec(probe, other->position_08);
        Func_0800447c(PUSH_DISTANCE_0800EBEC, push_angle16, probe);
        if (Func_0800d924(other, probe) != 0) {
            blocked_flags |= BLOCKED_OBJECT_0800EBEC;
            continue;
        }

        CopyPosition_0800ebec(probe, other->position_08);
        Func_0800447c(PUSH_PROBE_0800EBEC, push_angle16, probe);
        if (Func_080120dc(other, probe) != 0) {
            blocked_flags |= BLOCKED_OBJECT_0800EBEC;
            continue;
        }

        CopyPosition_0800ebec(probe, other->position_08);
        Func_0800447c(PUSH_PROBE_0800EBEC, push_angle16 + 0x1000, probe);
        if (Func_080120dc(other, probe) != 0) {
            blocked_flags |= BLOCKED_OBJECT_0800EBEC;
            continue;
        }
        /*
         * Uncertainty: the original repeats this probe verbatim on the same
         * buffer with no intervening change, so the second call can only
         * differ through Func_080120dc's own state.  Reproduced as written.
         */
        if (Func_080120dc(other, probe) != 0) {
            blocked_flags |= BLOCKED_OBJECT_0800EBEC;
            continue;
        }

        CopyPosition_0800ebec(probe, other->position_08);
        Func_0800447c(PUSH_PROBE_0800EBEC, push_angle16 - 0x1000, probe);
        if (Func_080120dc(other, probe) != 0) {
            blocked_flags |= BLOCKED_OBJECT_0800EBEC;
            continue;
        }

        /* Room in every direction: shove the object and stop its recoil. */
        Func_0800447c(PUSH_DISTANCE_0800EBEC, push_angle16, other->position_08);
        other->pushback_38[0] = (s32)0x80000000;
        other->pushback_38[1] = (s32)0x80000000;
        other->pushback_38[2] = (s32)0x80000000;
        pushed_flags |= 1;
    }

    if (blocked_flags == 0 && pushed_flags != 0) {
        /* Pushing costs speed for this frame. */
        self->speed_30 = 0x4000;
        self->acceleration_34 = 0x2000;
    }

resolve:
    stats = Data_03001ebc;
    if (stats != 0) {
        if ((blocked_flags & 3) != 0)
            stats->blocked_steps_19c = (u16)(stats->blocked_steps_19c + 1);
        else
            stats->blocked_steps_19c = 0;
    }

    if (pushed_flags != 0) {
        Func_0800c300(self, 8);
    } else if (blocked_flags != 0) {
        u8 *leader = Func_08077008(*(s32 *)(Data_02000240 + 0x1f4));

        Func_0800c300(self, *(s16 *)(leader + 56) != 0 ? 9 : 22);
    } else {
        Func_0800c300(self, walk_command);
    }

    if (blocked_flags != 0) {
        self->pushback_38[0] = (s32)0x80000000;
        self->pushback_38[1] = (s32)0x80000000;
        self->pushback_38[2] = (s32)0x80000000;
        self->velocity_24[0] = 0;
        self->velocity_24[2] = 0;

        if ((blocked_flags & 3) != 0) {
            /* Still turn towards the requested heading, up to 0x1000. */
            s32 turn = (s32)(s16)((heading_word >> 16) - self->heading_06);

            if (turn > 0x1000)
                turn = 0x1000;
            if (turn < -0x1000)
                turn = -0x1000;
            self->heading_06 = (u16)(self->heading_06 + turn);
        }

        self->animation_timer_64 = 0;
        self->animation_phase_66 = 2;
    } else {
        s32 speed;

        Func_0800d14c(self, primary[0], primary[1], primary[2]);

        /*
         * 0x0800f1b2 .. 0x0800f1cc.  Two inline interworking calls to the
         * IWRAM multiply square the horizontal velocity components; control
         * resumes at 0x0800f1bc and 0x0800f1c8 respectively, which is why
         * this statement straddles a manifest row boundary.
         */
        speed = Func_080045d4(
            MULTIPLY_0800EBEC(self->velocity_24[0], self->velocity_24[0])
            + MULTIPLY_0800EBEC(self->velocity_24[2], self->velocity_24[2]));

        /* blocked_flags is provably 0 on this path; the original stores it. */
        self->velocity_24[0] = blocked_flags;
        self->velocity_24[2] = blocked_flags;
        Func_0800447c(speed, chosen_angle & 0xffff, self->velocity_24);

        if (self->animation_timer_64 != 0)
            self->animation_timer_64 =
                (s16)((u16)self->animation_timer_64 - 1);
    }

    /* 0x0800f1fa: footstep dust, emitted only on a clean unobstructed step. */
    if (Data_03001e70[0x17] != 0
        && self->animation_timer_64 == 0
        && blocked_flags == 0) {
        Effect_0800ebec *effect = Func_0800c150(25,
                                                self->position_08[0],
                                                self->position_08[1],
                                                self->position_08[2]);

        if (effect != 0) {
            Sprite_0800ebec *sprite;
            u16 phase;

            effect->ground_14 = self->ground_14;
            sprite = effect->sprite_50;
            Func_0800c2d8(effect, (const void *)0x08013274);
            effect->kind_23 = 2;
            effect->flags_55 = (u8)blocked_flags;

            if (sprite != 0) {
                Func_0800ba30(sprite, 1);
                sprite->palette_26 = (u8)blocked_flags;
                sprite->angle_1e =
                    (u16)(0x4000 + ((heading_word >> 16) & 0xffff));
                sprite->flags_09 |= 12;
            }

            phase = (u16)self->animation_phase_66;
            if (self->animation_phase_66 == 2) {
                /*
                 * Uncertainty: `sprite` may be null here - the original does
                 * not re-test it before this second call.  Reproduced.
                 */
                Func_0800ba30(sprite, 2);
                self->animation_phase_66 = (s16)blocked_flags;
                phase = 0;
            }
            if (phase != 0)
                effect->angle_06 = 0x8000;

            self->animation_timer_64 = walk_command == 5 ? 12 : 18;
            self->animation_phase_66 ^= 1;
        }
    }

    Func_0800eaf8();
    self->step_04 = (u16)(self->step_04 + 1);
    return 1;
}
