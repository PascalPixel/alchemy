typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b3 ambient-particle emitter at 0x020011c4 (164 bytes, 6 calls).
 *
 * The function occupies 0x020011c4-0x02001259.  Alignment at 0x0200125a and
 * its three referenced pool words extend the whole-owner span to 0x02001268,
 * where the next prologue begins.  The pool supplies the global frame flags,
 * the shared 0xb333 effect coefficient, and packed descriptor 0x00090001.
 *
 * Emission is suppressed on three out of every four global frames.  On an
 * eligible frame, two independent random samples jitter the primary actor's
 * x/z coordinates by -8..8 whole 16.16 units.  A third sample chooses one of
 * five vertical bands above 0x30000, then the quotient helper normalizes that
 * value by ten before the particle is created.  Keeping all random samples in
 * separate statements is significant: it preserves their machine call order.
 */

typedef struct ActorPosition {
    u8 reserved00[8];
    s32 x;
    s32 y;
    s32 z;
} ActorPosition;

typedef struct ParticleOptions {
    s32 callerReserved;
    s32 count;
    s32 scaleStart;
    s32 scaleEnd;
} ParticleOptions;

extern ActorPosition *Func_0808a080(s32 index);
extern u32 Func_080000f8(void);
extern s32 Func_03000380(s32 value, s32 divisor);
extern void Func_0200013c(s32 x, s32 y, s32 z, s32 kind,
                          s32 verticalOffset, s32 unused,
                          u32 descriptor, ParticleOptions *options);

void Func_020011c4(void)
{
    ActorPosition *actor = Func_0808a080(0);
    volatile u32 *frameFlags = (volatile u32 *)0x03001e40;
    ParticleOptions options;
    u32 sample;
    s32 x;
    s32 z;
    s32 verticalOffset;

    if ((*frameFlags & 3u) != 0) {
        return;
    }

    /* The callee contract starts at count; the first stack word is reserved. */
    options.count = 10;
    options.scaleStart = 0xb333;
    options.scaleEnd = 0xb333;

    sample = Func_080000f8();
    x = actor->x + ((s32)((sample * 17u) >> 16) - 8) * 0x10000;

    sample = Func_080000f8();
    z = actor->z + ((s32)((sample * 17u) >> 16) - 8) * 0x10000;

    sample = Func_080000f8();
    verticalOffset = (s32)((sample * 5u) >> 16) * 0x10000 + 0x30000;
    verticalOffset = Func_03000380(verticalOffset, 10);

    Func_0200013c(x, actor->y, z, 0,
                  verticalOffset, 0, 0x00090001, &options);
}
