/* Func_0800f2f8 -- companion/follower walk-step, a sibling of
 * semantic/main/0800ebec.c's Func_0800ebec ("player walk step").
 *
 * The state object's fields match struct Object_0800ebec from 0800ebec.c
 * exactly: position_08[3], step_04, heading_06, velocity_24[0]/[2] (x/z,
 * skipping y at +40), pushback_38[3], animation_timer_64/animation_phase_66.
 * Byte +0x54 (offset 84) is status_54, the same field 0800c62c.c's
 * struct Object_0800c62c names on the same record (that file also documents
 * the record as Data_03001e64, 64 entries of 0x70 bytes).
 *
 * This routine differs from Func_0800ebec in three ways, all evidenced
 * directly in the assembly:
 *   1. Its own speed_30/acceleration_34 tuning constants (slower base walk,
 *      a faster "run" tier, and a dash/catch-up boost on the R button that
 *      skips the clearance check entirely) -- consistent with a trailing
 *      companion that needs to close distance on its leader.
 *   2. A stricter clearance search: the primary heading plus four flanking
 *      offsets (0, +-0x1000, +-0x2000) must ALL pass Func_080122ac before
 *      the position is accepted; if any fails, an 8-entry ring of headings
 *      (+-0x1000/0x2000/0x3000/0x4000 around the primary) is tried in turn,
 *      each subjected to the same 5-probe clearance check, first passing
 *      heading wins.
 *   3. A smoothing step against a shared table (Data_03001e70's pointee,
 *      +0x11a) not present in Func_0800ebec at all -- see the Uncertainty
 *      comment at its use site.
 *
 * Uncertainty: Func_080122ac and Func_08012204 are not established anywhere
 * else in this dig. Func_080122ac's call shape, (state, positionRecord) ->
 * nonzero-on-reject, is identical to Func_080120dc's from 0800ebec.c, so it
 * is declared with the same signature on that basis, not a confirmed match.
 * Func_08012204's role (state-machine transition keyed off the sub-object
 * at effect_50) is inferred from its two-way branch on the return value (9
 * vs 6) and is not otherwise cross-checked.
 */

#include "types.h"
#include "layout_guard.h"

/* Identical record to 0800ebec.c's struct Object_0800ebec; only the fields
 * this row touches are named individually, following that file's own
 * convention of leaving the rest as sized-but-unnamed byte ranges. */
struct Object_0800f2f8 {
    const void *program_00;
    u16 step_04;
    u16 heading_06;
    s32 position_08[3];
    s32 ground_14;
    u8 unknown_18[8];
    u16 unknown_20;
    u16 unknown_22;
    s32 velocity_24[3]; /* x, y, z; y (velocity_24[1]) is never touched here */
    s32 speed_30;
    s32 acceleration_34;
    s32 pushback_38[3];
    u8 unknown_44[0xc];
    void *effect_50; /* 0x50: linked effect/sub-object pointer, read only
                       * when status_54 == 1 */
    u8 status_54;    /* 0x54: state byte; gates the Func_08012204 block and
                       * the dust-effect spawn, Garet's naming from
                       * 0800c62c.c's Object_0800c62c */
    u8 unknown_55[0xf];
    s16 animation_timer_64;
    s16 animation_phase_66;
    u8 unknown_68[8];
};
LAYOUT_SIZE_GUARD(Object0800f2f8_Size, struct Object_0800f2f8, 0x70);
LAYOUT_OFFSET_GUARD(Object0800f2f8_Speed, struct Object_0800f2f8, speed_30, 0x30);
LAYOUT_OFFSET_GUARD(Object0800f2f8_Effect, struct Object_0800f2f8, effect_50, 0x50);
LAYOUT_OFFSET_GUARD(Object0800f2f8_Status, struct Object_0800f2f8, status_54, 0x54);
LAYOUT_OFFSET_GUARD(Object0800f2f8_AnimTimer, struct Object_0800f2f8, animation_timer_64, 0x64);

/* Sub-object reached through Object_0800f2f8::effect_50; not identified
 * against any other struct. Fields named by role only, mirroring the
 * "named by role" convention 0800c62c.c uses for its own SubObject. */
struct Companion_0800f2f8 {
    u8 unknown_00[5];
    u8 flags_05; /* 0x05: bits 0,2,3 cleared then bit 2 set when a sprite is
                  * linked at the dust-effect tail */
    u8 unknown_06[3];
    u8 flags_09; /* 0x09: bits 0,2,3 cleared then bit 3 set, same tail */
    u8 unknown_0a[0x22];
    u8 unknown_38; /* 0x26: written with the low blocked-flags byte */
    u8 unknown_39[5];
    void *link_2c; /* 0x2c: pointer whose target's +6 byte is written by the
                     * Func_08012204 result branch */
};

/* Position/collision-clearance record: object position plus offset,
 * consumed by Func_080122ac. */
struct PositionProbe_0800f2f8 {
    s32 x, y, z;
};

/* Small effect object returned by Func_0800c150; the fields this row
 * touches (+0x22 flags_22/kind, +0x50 sprite_50, +0x55 flags_55) match
 * struct Effect_0800ebec from 0800ebec.c. */
struct Effect_0800f2f8 {
    u8 unknown_00[0x22];
    u8 flags_22;
    u8 unknown_23[0x2d];
    void *sprite_50;
    u8 unknown_54;
    u8 flags_55;
};

typedef s32 (*Multiply_0800f2f8)(s32 a, s32 b);
#define MULTIPLY_0800F2F8 ((Multiply_0800f2f8)0x03000118)

/* Input mirrors, established across dozens of semantic/main/*.c files. */
extern u32 Data_03001ae8; /* keys currently held */
extern u32 Data_03001b04; /* keys newly pressed this frame */
extern u8 Data_03001f54;  /* walk-system-allowed gate */

/* Party/global state block; +0x21c is the "counts as run" button mask,
 * established in 0800ebec.c. */
extern u8 Data_02000240[];

/* d-pad-combo -> heading table, established in 0800ebec.c (16 entries,
 * 0xffff meaning "no direction held"). */
extern const s16 Data_08013254[16];

/* Second table, positioned right after Data_08013254 in ROM but indexed
 * with a WORD stride and read via the same (Data_03001ae8>>4)&15 d-pad
 * index -- not the same table, just adjacent. Uncertainty: only ever read
 * here; role beyond "per-d-pad-direction target value" is not established. */
extern const s32 Data_0801328c[16];

/* Uncertainty: passed as Func_0800c2d8's second argument here but not
 * established anywhere else; role unknown beyond "a small table adjacent to
 * Data_0801328c". */
extern const void *Data_08013280;

/* Stats block, established in 0800ebec.c: +0x19c is blocked_steps_19c. */
extern struct {
    u8 unknown_0[0x19c];
    u16 blocked_steps_19c;
} *Data_03001ebc;

/* Shared table pointer, established in 0800ebec.c as a u8* gating the dust
 * effect at +0x17. This row reads a different field of the same pointee,
 * a u16 at +0x11a, smoothed toward Data_0801328c[dpad_index] in steps of at
 * most +-0x200 (snapping exactly once within 15). Uncertainty: the field's
 * meaning is not established; declared as its own small view rather than
 * folding into 0800c62c.c's or 0800ebec.c's own (different) views of the
 * same pointer, following this dig's convention of per-file local structs
 * over shared headers. */
extern struct {
    u8 unknown_0[0x11a];
    u16 followValue_11a;
} *Data_03001e70;

extern s32 Func_0800447c(s32 distance, s32 angle, s32 position[3]);
extern s32 Func_080045d4(s32 squared_sum);
extern void Func_0800c300(struct Object_0800f2f8 *self, s32 command);
extern void Func_0800d14c(struct Object_0800f2f8 *self, s32 x, s32 y, s32 z);
extern s32 Func_0800eaf8(struct Object_0800f2f8 *self);
extern struct Effect_0800f2f8 *Func_0800c150(s32 kind, s32 x, s32 y, s32 z);
extern void Func_0800c2d8(struct Effect_0800f2f8 *effect, const void *table);
extern void Func_0800ba30(void *sprite, s32 arg);

/* Uncertainty: shape inferred from the call site (state, &positionRecord),
 * identical to Func_080120dc's shape in 0800ebec.c; returns nonzero to
 * reject the probed position. */
extern s32 Func_080122ac(struct Object_0800f2f8 *self,
                          const struct PositionProbe_0800f2f8 *probe);

/* Uncertainty: shape inferred from the call site (self->position_08),
 * single argument, s32 return tested against 9 and 6. */
extern s32 Func_08012204(const s32 position[3]);

#define WALK_COMMAND_0800F2F8 2
#define RUN_COMMAND_0800F2F8 5
#define IDLE_COMMAND_0800F2F8 9

#define BLOCKED_NO_ROOM_0800F2F8 1
#define BLOCKED_OBJECT_0800F2F8 2
#define BLOCKED_NO_INPUT_0800F2F8 4

#define WALK_DISTANCE_0800F2F8 0x70000
#define TURN_CLAMP_0800F2F8 0x1000

/* Tries `heading` at the primary position plus the four flanking offsets
 * (0, +0x1000, -0x1000, +0x2000, -0x2000). All five must clear
 * Func_080122ac for the heading to be accepted. Matches the assembly's
 * unrolled 5-probe sequence exactly (used both for the primary d-pad
 * heading and, from the ring search below, for each of the 8 ring
 * candidates). */
static s32 ClearanceCheck_0800f2f8(struct Object_0800f2f8 *self, s32 heading)
{
    static const s32 offsets[5] = {0, 0x1000, -0x1000, 0x2000, -0x2000};
    s32 i;

    for (i = 0; i < 5; i++) {
        struct PositionProbe_0800f2f8 probe;
        probe.x = self->position_08[0];
        probe.y = self->position_08[1];
        probe.z = self->position_08[2];
        Func_0800447c(WALK_DISTANCE_0800F2F8, heading + offsets[i], (s32 *)&probe);
        if (Func_080122ac(self, &probe) != 0) {
            return 1; /* rejected */
        }
    }
    return 0; /* accepted */
}

/* 8-entry ring fallback: angle +-0x1000/0x2000/0x3000/0x4000 around the
 * primary heading, tried in that interleaved order, first candidate to
 * pass ClearanceCheck_0800f2f8 wins. Matches the assembly's table-building
 * block plus its 8-iteration loop. */
static s32 RingSearch_0800f2f8(struct Object_0800f2f8 *self, s32 primaryHeading, s32 *chosenHeading)
{
    static const s32 ringOffsets[8] = {
        0x1000, -0x1000, 0x2000, -0x2000, 0x3000, -0x3000, 0x4000, -0x4000,
    };
    s32 i;

    for (i = 0; i < 8; i++) {
        s32 candidate = primaryHeading + ringOffsets[i];
        if (ClearanceCheck_0800f2f8(self, candidate) == 0) {
            *chosenHeading = candidate;
            return 0; /* accepted */
        }
    }
    return 1; /* every ring candidate rejected */
}

s32 Func_0800f2f8(struct Object_0800f2f8 *self)
{
    s32 walkCommand = WALK_COMMAND_0800F2F8;
    s32 blockedFlags = 0;
    s32 chosenHeading;
    s32 rawHeading;
    s32 dpadIndex;

    if ((Data_03001ae8 & *(u16 *)(Data_02000240 + 0x21c)) != 0) {
        /* Holding the "run" mask: faster tier. */
        self->speed_30 = 0x10000;
        self->acceleration_34 = 0x14000;
        walkCommand = RUN_COMMAND_0800F2F8;
    } else {
        /* Base walk tier. */
        self->speed_30 = 0x8000;
        self->acceleration_34 = 0x4000;
    }

    if ((Data_03001b04 & 0x200) != 0) {
        /* R just pressed: dash/catch-up boost. */
        self->speed_30 = 0x40000;
    }

    dpadIndex = (Data_03001ae8 >> 4) & 15;
    rawHeading = Data_08013254[dpadIndex];
    chosenHeading = rawHeading;

    if ((u16)rawHeading == 0xffff) {
        /* No direction held. */
        blockedFlags |= BLOCKED_NO_INPUT_0800F2F8;
        goto resolve;
    }

    if (Data_03001f54 != 0 && (Data_03001ae8 & 0x200) != 0) {
        /* Walk system active and R held: skip the clearance check
         * entirely (the dash/catch-up case always succeeds). */
        goto resolve;
    }

    if (ClearanceCheck_0800f2f8(self, rawHeading) != 0) {
        /* Primary heading (or one of its flanking probes) blocked;
         * fall back to the 8-entry ring search. */
        if (RingSearch_0800f2f8(self, rawHeading, &chosenHeading) != 0) {
            blockedFlags |= BLOCKED_NO_ROOM_0800F2F8;
        }
    }

resolve:
    if (Data_03001ebc != 0) {
        if ((blockedFlags & (BLOCKED_NO_ROOM_0800F2F8 | BLOCKED_OBJECT_0800F2F8)) != 0) {
            Data_03001ebc->blocked_steps_19c++;
        } else {
            Data_03001ebc->blocked_steps_19c = 0;
        }
    }

    Func_0800c300(self, blockedFlags != 0 ? IDLE_COMMAND_0800F2F8 : walkCommand);

    if (blockedFlags != 0) {
        s32 turn;

        self->pushback_38[0] = (s32)0x80000000;
        self->pushback_38[1] = (s32)0x80000000;
        self->pushback_38[2] = (s32)0x80000000;
        self->velocity_24[0] = 0;
        self->velocity_24[2] = 0;

        if ((blockedFlags & (BLOCKED_NO_ROOM_0800F2F8 | BLOCKED_OBJECT_0800F2F8)) != 0) {
            turn = (s16)chosenHeading - self->heading_06;
            if (turn > TURN_CLAMP_0800F2F8) {
                turn = TURN_CLAMP_0800F2F8;
            }
            if (turn < -TURN_CLAMP_0800F2F8) {
                turn = -TURN_CLAMP_0800F2F8;
            }
            self->heading_06 = (u16)(self->heading_06 + turn);
        }

        self->animation_timer_64 = 0;
        self->animation_phase_66 = 2;
    } else {
        s32 speed;

        Func_0800d14c(self, self->position_08[0], self->position_08[1], self->position_08[2]);

        speed = Func_080045d4(MULTIPLY_0800F2F8(self->velocity_24[0], self->velocity_24[0]) +
                               MULTIPLY_0800F2F8(self->velocity_24[2], self->velocity_24[2]));

        self->velocity_24[0] = 0;
        self->velocity_24[2] = 0;
        Func_0800447c(speed, (u16)chosenHeading, self->velocity_24);

        if (self->animation_timer_64 != 0) {
            self->animation_timer_64--;
        }
    }

    /* Smooth Data_03001e70's followValue_11a toward Data_0801328c[dpadIndex]
     * in steps of at most +-0x200, snapping exactly once within 15.
     * Uncertainty: this field/table pair is not established anywhere else
     * in this dig; the arithmetic (divide-by-8, clamp to +-0x200, snap
     * inside +-15) is transcribed faithfully but its purpose is unclear --
     * plausibly a trailing camera/formation offset. */
    {
        s32 target = Data_0801328c[dpadIndex];
        s32 current = Data_03001e70->followValue_11a;
        s32 delta = (s16)(target - current);
        s32 step;

        if (delta < 0) {
            delta += 7;
        }
        step = delta >> 3;
        if (step > 0x200) {
            step = 0x200;
        }
        if (step < -0x200) {
            step = -0x200;
        }
        if ((u32)(step + 15) > 30) {
            /* Outside the tight snap window: apply the clamped step. */
        } else {
            /* Within +-15 of the target: snap exactly. */
            step = target - current;
        }
        Data_03001e70->followValue_11a = (u16)(current + step);
    }

    if (self->status_54 == 1) {
        struct Companion_0800f2f8 *companion = self->effect_50;
        s32 result = Func_08012204(self->position_08);

        if (result == 9) {
            *(u8 *)((u8 *)companion->link_2c + 6) = self->status_54;
            companion->unknown_38 = 0;
        } else {
            *(u8 *)((u8 *)companion->link_2c + 6) = 9;
            companion->unknown_38 = self->status_54;
        }

        if (result == 6 && self->animation_timer_64 == 0 && blockedFlags == 0) {
            struct Effect_0800f2f8 *effect = Func_0800c150(24, self->position_08[0],
                                                             self->position_08[1],
                                                             self->position_08[2]);
            if (effect != 0) {
                void *sprite = effect->sprite_50;

                Func_0800c2d8(effect, Data_08013280);
                effect->flags_55 = (u8)blockedFlags;
                effect->flags_22 = 1;

                if (sprite != 0) {
                    u8 *flags05 = (u8 *)sprite + 5;
                    u8 *flags09 = (u8 *)sprite + 9;

                    Func_0800ba30(sprite, 1);
                    *((u8 *)sprite + 0x26) = (u8)blockedFlags;
                    *flags05 = (u8)((*flags05 & ~0xd) | 4);
                    *flags09 = (u8)((*flags09 & ~0xd) | 8);
                }

                self->animation_timer_64 = 10;
            }
        }
    }

    Func_0800eaf8(self);
    self->step_04++;
    return 1;
}
