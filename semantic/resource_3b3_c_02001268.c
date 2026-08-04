typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 3b3 forward-clearance transition at 0x02001268
 * (328-byte whole-owner span, 17 calls).
 *
 * The executable body ends at the interworking return at 0x020013a6.  Its two
 * referenced pool words (Data_02000240 and 0xfff00000) extend the owner to the
 * next prologue at 0x020013b0.
 *
 * The owner tests two grid-centred probes, first four and then eight 16.16
 * units along the actor's quarter-turn heading.  Terrain and actor-overlap
 * checks must both accept the probes.  Success brackets a scene transition,
 * updates motion fields, briefly clears the actor's endpoint flags, moves the
 * camera/marker to the accepted probe cell, then restores the original flags.
 * Every rejecting path returns zero without opening the scene bracket.
 */

typedef struct TransitionActor {
    u8 reserved00[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved14[20];
    s32 lift;
    u8 reserved2c[4];
    s32 speedA;
    s32 speedB;
    u8 reserved38[29];
    u8 endpointFlags;
} TransitionActor;

extern u8 Data_02000240[];

extern TransitionActor *Func_0808a080(s32 index);
extern void Func_08000128(s32 distance, s32 heading, s32 position[3]);
extern s32 Func_080091d8(TransitionActor *actor, s32 position[3]);
extern s32 Func_02000350(s32 position[3], TransitionActor *actor);
extern void Func_0808a018(void);
extern void Func_08009080(TransitionActor *actor, s32 mode);
extern void Func_080000c0(s32 value);
extern void Func_080f9010(s32 value);
extern void Func_080091e0(TransitionActor *actor, s32 enabled);
extern void Func_0808a0c0(s32 selector, s32 xCell, s32 zCell);
extern void Func_0808a020(void);

s32 Func_02001268(void)
{
    TransitionActor *actor = Func_0808a080(0);
    u8 savedFlags = actor->endpointFlags;
    s32 heading = (actor->heading + 0x4000) & 0xc000;
    s32 probe[3];
    s16 probeX;
    s16 probeZ;

    if (Data_02000240[498] != 0) {
        return 0;
    }

    probe[0] = (actor->x & (s32)0xfff00000) + 0x80000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x80000;
    Func_08000128(0x400000, heading, probe);

    if (Func_080091d8(actor, probe) == 1) {
        return 0;
    }
    if (Func_02000350(probe, actor) != 0) {
        return 0;
    }

    probe[0] = (actor->x & (s32)0xfff00000) + 0x80000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x80000;
    Func_08000128(0x800000, heading, probe);

    if (Func_02000350(probe, actor) != 0) {
        return 0;
    }
    if (Func_080091d8(actor, probe) != 0) {
        return 0;
    }

    Func_0808a018();
    Func_08009080(actor, 6);
    Func_080000c0(6);
    Func_080f9010(152);
    Func_08009080(actor, 7);

    actor->speedA = 0x30000;
    actor->speedB = 0x20000;
    actor->lift = 0x40000;
    actor->endpointFlags &= 0x7e;

    Func_080091e0(actor, 0);
    probeX = *(s16 *)((u8 *)probe + 2);
    probeZ = *(s16 *)((u8 *)probe + 10);
    Func_0808a0c0(0, probeX, probeZ);
    Func_08009080(actor, 6);
    Func_080091e0(actor, 1);

    actor->endpointFlags = savedFlags;
    Func_0808a020();
    return 1;
}
