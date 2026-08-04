typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * The two records below are the objects reached through this owner's literal
 * pool.  Their named offsets are deliberately explicit: these are the
 * offsets exercised by the disassembly, rather than guessed game-wide types.
 */
typedef struct Resource3bdActor {
    u8 unknown00[6];
    s16 phase;
    u8 unknown08[0x10];
    s32 x;
    s32 y;
    u8 unknown20[0x4c];
    s32 flags;
} Resource3bdActor;

typedef struct Resource3bdState {
    s16 mode;
    s16 mode_argument;
    u8 unknown04[2];
    s16 phase;
    s16 field08;
    s16 field0a;
} Resource3bdState;

/* Literal-pool addresses: 0x02001001, 0x02001000, 0x0200bf6c, 0x02008f95. */
extern volatile u8 Data_02001000[];
extern volatile u8 Data_02001001[];
extern volatile Resource3bdState **Data_0200bf6c;
extern void *Data_02008f95;

extern s32 Func_03000380(s32 value, s32 mode);
extern s32 Func_030003ac(s32 value, s32 mode);
extern void Func_080000c0(s32 frames);
extern void Func_080000d0(void *callback, s32 frames);
extern void Func_080000d8(void *callback);
extern s32 Func_080000f8(void);
extern void Func_080091f0(s32 x, s32 y, s32 z);
extern void Func_0808a010(s32 value);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern Resource3bdActor *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a170(s32 value);
extern void Func_0808a180(s32 value, s32 mode);
extern void Func_0808a460(void);
extern void Func_080f9010(s32 value);
extern void Func_02000f10(s32 id, s32 x, s32 phase);
extern void Func_02003ad4(void);

/* Literal words at 0x020013e4, 0x020013e8, 0x020013ec, and 0x020013f0. */
static const s32 kActorPhaseDelta = (s32)0xffffcccd;
static const s32 kActorPhaseStep = 0x00006666;
static const s32 kLowerLimit = -1;
static const s32 kMotionDelta = (s32)0xf9f8f402u;

void Func_0200109c(s32 scene)
{
    volatile Resource3bdState *state;
    Resource3bdActor *actor;
    s32 i;
    s32 cell;
    s32 current;
    s32 success;
    s32 scene_kind;
    s8 scene_step;
    u8 phase;
    u8 frame;
    u8 counter;
    s32 phase_input;
    s16 actor_phase;
    u32 motion_x;

    success = 0;
    Func_0808a018();
    Func_0808a460();
    Func_0808a170(0x21db);
    Func_0808a180(16, 0);
    Func_0808a020();

    /* 0x020010d4..0x020010ee: initialise records 11..15. */
    for (i = 0; i <= 4; i++) { /* branch 0x020010ee */
        actor = Func_0808a080(i + 11);
        actor->flags = 0; /* store 0x020010e6 */
        actor->x = 0x10000; /* store 0x020010e8 */
        actor->y = 0x10000; /* store 0x020010ea */
    }

    scene_kind = (s8)Data_02001001[0];
    scene_step = (s8)Data_02001001[1];
    state = *Data_0200bf6c;
    cell = Func_03000380((s32)scene_step << 16, 5);
    state->phase = (s16)(cell + 0x4000); /* store 0x02001118 */
    phase = (u8)Data_02001001[0];
    frame = Data_02001001[1];

    /* 0x0200111a..0x02001256: the three scene-state arms. */
    switch (scene_kind) {
    case 0: /* branch 0x0200111c */
        if (scene == 16) { /* branch 0x02001120 */
            phase = 1;
            Func_080f9010(110);
        } else {
            Func_080f9010(114);
        }
        /* branch 0x0200112c skips to the shared state write. */
        Data_02001000[0] = 0; /* store 0x02001138 */
        break; /* branch 0x0200113a */

    case 1: /* branch 0x0200113e */
        if (scene == 16) { /* branch 0x02001142 */
            Func_080f9010(110);
        } else if (scene == 20) { /* branch 0x0200114e */
            phase = 2;
            Func_080f9010(110);
            Func_080000c0(30);
            actor_phase = state->phase;
            for (i = 0; i <= 4; i++) { /* branch 0x020011c0 */
                s32 id = i + 11;
                Func_02000f10(id, 0x180000, (s32)(u16)actor_phase);
                Func_080f9010(151);
                actor = Func_0808a080(id);
                actor->x = 0; /* store 0x02001192 */
                current = kActorPhaseStep;
                actor->y = current; /* store 0x02001198 */
                actor->x = current; /* store 0x0200119a */
                /* The machine reloads actor->x after each wait; the scheduler
                 * advances that record asynchronously. `current` is r6's
                 * local fixed-point scratch and is not the loop predicate. */
                do { /* branch 0x020011b2 */
                    Func_080000c0(1);
                    current += 0xC00;
                } while (actor->x <= kLowerLimit);
                actor_phase = (s16)((u16)actor_phase + kActorPhaseDelta);
            }
            Func_080000c0(30);
            success = 1;
        } else { /* branch 0x020011d8 */
            Func_080f9010(114);
            phase = 0;
        }
        /* branch 0x0200114a joins the scene-20 comparison above. */
        break; /* branch 0x020011cc */

    case 2: /* branch 0x020011dc */
        if (scene != (s32)scene_step + 16) { /* branch 0x020011e4 */
            phase = 0;
            Func_080f9010(114);
            Func_080000c0(30);
            for (i = 0; i <= 4; i++) { /* branch 0x02001242 */
                s32 id = i + 11;
                actor = Func_0808a080(id);
                Func_080f9010(151);
                current = actor->x;
                if (current > kActorPhaseStep) { /* branch 0x02001214 */
                    do { /* branch 0x0200122e */
                        actor->y = current; /* store 0x02001216 */
                        actor->x = current; /* store 0x02001218 */
                        Func_080000c0(1);
                        current += kMotionDelta;
                    } while (actor->x > kActorPhaseStep);
                }
                Func_0808a0f0(id, 0, 0);
            }
        } else {
            Func_080f9010(110);
            success = 1;
            Func_080000c0(30);
        }
        break; /* branch 0x02001244 */
    }

    Data_02001001[0] = phase; /* store 0x0200125c */
    if (success == 0) /* branch 0x02001260 */
        return; /* branch 0x02001262 */

    /* r7 still points at 0x02001001 here: `r7 - 1` is 0x02001000,
     * and the zero-extended byte is the selector for the branch at 0x12aa. */
    counter = (u8)(Data_02001000[0] + 1);
    Data_02001000[0] = counter; /* store 0x0200126a */
    /* The ARM sequence sign-truncates the pre-offset value to eight bits,
     * then adds five before calling the established s32 modulo helper. */
    phase_input = (s32)(s8)((((u32)Func_080000f8() * 4) >> 16) + frame + 1);
    Data_02001001[1] = (u8)Func_030003ac(phase_input + 5, 5); /* store 0x02001286 */

    state = *Data_0200bf6c;
    state->mode = 0; /* store 0x0200128c */
    state->mode_argument = 0; /* store 0x0200128e */
    state->field08 = 0x200; /* store 0x02001294 */
    state->field0a = 0x3000; /* store 0x020012a0 */
    Func_080000d0(Data_02008f95, 0xC80);

    if (counter <= 2) { /* branch 0x020012ac; r5 is the incremented 0x02001000 byte */
        if (state->mode != 99) { /* branch 0x020012b6 */
            do { /* branch 0x020012c8 */
                Func_080000c0(1);
            } while (state->mode != 99);
        }
        Func_080000c0(10);
        Func_080f9010(110);
    } else {
        Data_02001001[0] = 99; /* store 0x020012da */
        if (state->mode != 2) { /* branch 0x020012e4 */
            do { /* branch 0x020012f6 */
                Func_080000c0(1);
            } while (state->mode != 2);
        }
        state->mode = 2; /* store 0x020012fe */
        state->mode_argument = 0; /* store 0x02001300 */
        Func_080091f0(0x20000, 0x20000, 0x10000);
        Func_0808a010(20);
        Func_080091f0(0x40000, 0x40000, 0x10000);
        state->mode = 99; /* store 0x0200132c */
        Func_080f9010(190);

        motion_x = 0x180000;
        actor_phase = state->phase;
        do { /* branch 0x02001396 */
            for (i = 0; i <= 4; i++) { /* branch 0x0200137e */
                s32 id = i + 11;
                actor = Func_0808a080(id);
                actor->x -= 16; /* store 0x0200135a */
                actor->y -= 16; /* store 0x02001360 */
                Func_02000f10(id, (s32)motion_x, (s32)(u16)actor_phase);
                actor_phase = (s16)((u16)actor_phase + kMotionDelta);
            }
            motion_x += kMotionDelta;
            actor_phase = (s16)((u16)actor_phase + 0xC00);
            Func_080000c0(1);
        } while ((s32)motion_x > 0);

        for (i = 0; i <= 4; i++) { /* branch 0x020013ae */
            Func_0808a0f0(i + 11, 0, 0);
        }
        Func_02003ad4();
        Func_080f9010(80);
    }
    /* branch 0x020012d6 reaches the common return path. */
    Func_080000d8(Data_02008f95);
}
