typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;

struct MovementDelta_02004048 {
    s32 x;
    s32 padding04;
    s32 y;
};

struct ActorRecord_02004048 {
    u8 padding00[8];
    s32 x;
    s32 z;
    s32 y;
    s32 target_z;
};

struct ParticleSpec_02004048 {
    u32 kind;
    u32 variant;
    s32 spread_x;
    s32 spread_y;
    u8 padding10[8];
    u16 field18;
    u8 padding1a[2];
    const void *payload;
    u16 padding20;
    u16 field22;
};

/*
 * Actor collision/motion resolver at 0x02004048, complete 628-byte span
 * through its sole return, alignment and seven-word pool before 0x020042bc.
 * It probes four candidate positions, applies the original rotated fallback
 * movement, optionally emits one particle, and preserves all 23 ordered calls.
 */

extern u32 Data_02000240[];
extern void Func_02000118(s32, s32, s32, s32, s32, s32, s32,
                          struct ParticleSpec_02004048 *);
extern void Func_080000c0(s32 frames);
extern u32 Func_080000f8(void);
extern void Func_08009080(struct ActorRecord_02004048 *actor, s32 mode);
extern void Func_08009150(struct ActorRecord_02004048 *actor,
                          s32 x, s32 z, s32 y);
extern void Func_08009158(struct ActorRecord_02004048 *actor);
extern s32 Func_080091d8(struct ActorRecord_02004048 *actor,
                         const s32 target[3]);
extern struct ActorRecord_02004048 *Func_0808a080(s32 actor);
extern void Func_0808a1f0(s32 actor, s32 cue);

void Func_02004048(const struct MovementDelta_02004048 *delta)
{
    u8 *state = *(u8 **)0x03001ebc;
    struct ActorRecord_02004048 *other =
        *(struct ActorRecord_02004048 **)(state + 480);
    s32 actor_id = (s32)Data_02000240[125];
    struct ActorRecord_02004048 *actor = Func_0808a080(actor_id);
    struct ParticleSpec_02004048 spec;
    s32 target[3];
    s32 distance;
    u32 frame_flags = *(u32 *)0x03001e40 & 4;

    target[0] = actor->x + delta->x * 0x18000;
    target[1] = actor->z;
    target[2] = actor->y + delta->y * 0x18000;
    distance = Func_080091d8(actor, target);

    if (frame_flags == 0) {
        s32 x;
        s32 coefficient_x;
        s32 coefficient_y;

        spec.field22 = (u16)(((Func_080000f8() << 12) >> 16) + 0xf800);
        x = actor->x +
            (s32)(((Func_080000f8() * 12u) >> 16) << 16) - 0x60000;
        coefficient_x =
            (s32)(((Func_080000f8() * 5u) >> 16) * 0x1999) + 0x7ffd;
        coefficient_y =
            (s32)(((Func_080000f8() * 5u) >> 16) * 0x1999) + 0x7ffd;

        Func_02000118(x, actor->z, actor->y,
                      delta->x * coefficient_x,
                      0, delta->y * coefficient_y,
                      0x00800000, &spec);
    }

    if (distance < 0) {
        Func_0808a1f0(actor_id, 0x102);
        Func_08009150(actor, actor->x, actor->z, actor->y + 0x80000);
        Func_08009080(actor, 7);
        Func_08009158(actor);
        do {
            Func_080000c0(1);
        } while (actor->z != actor->target_z);
        Func_08009080(actor, 6);
        Func_080000c0(3);
        return;
    }

    target[0] = actor->x + (delta->x << 19);
    target[1] = actor->z;
    target[2] = actor->y + (delta->y << 19);
    distance = Func_080091d8(actor, target);
    if (distance > 0)
        return;

    target[0] = actor->x + delta->x * 0x5b333 - delta->y * 0x5b333;
    target[1] = actor->z;
    target[2] = actor->y + delta->y * 0x5b333 - delta->x * 0x5b333;
    distance = Func_080091d8(actor, target);
    if (distance > 0) {
        actor->x += delta->y * 0x18000;
        actor->y += delta->x * 0x18000;
        return;
    }

    target[0] = actor->x + (delta->x + delta->y) * 0x5b333;
    target[1] = actor->z;
    target[2] = actor->y + (delta->x + delta->y) * 0x5b333;
    distance = Func_080091d8(actor, target);
    if (distance > 0) {
        actor->x -= delta->y * 0x18000;
        actor->y -= delta->x * 0x18000;
        return;
    }

    other->x += delta->x * 0x18000;
    actor->x += delta->x * 0x18000;
    other->y += delta->y * 0x18000;
    actor->y += delta->y * 0x18000;
}
