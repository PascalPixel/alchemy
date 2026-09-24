#include "TYPES.H"

s32 Main_030003f0(s32 num, s32 den);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
void *Engine_ActorGet(s32 id);
void Engine_ObjectSetAnimation(void *object, s32 animation);
void Engine_ObjectSetPalette(void *object, s32 palette);
s32 Engine_RandomNext(void);
s32 Engine_MathModulo(s32 value, s32 modulus);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_AudioPlayCue(s32 cue);
void Engine_ActorSetSpriteFlags(void *actor, s32 flags);
void Engine_ObjectSetScript(void *object, const void *script);
s32 Engine_MathCos(u16 angle);
s32 Engine_MathSin(u16 angle);
void Engine_ObjectSetPosition(void *object, s32 x, s32 y, s32 z);

#include "IWRAM_CALL.H"

struct EmberSprite {
    u8 unknown_00[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Ember {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[16];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[12];
    struct EmberSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
};

extern u8 *gFieldWork;
extern s32 gEmberTimer;
extern s32 gEmberState[16];
extern s32 gEmberMask;
extern s32 gFrameCount;

/* Passes its constants straight into the argument registers; a direct call
 * shares one -1 between the first two arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Sixteen-actor effect step (actors 16..31): sets the work values from the
 * effect timer, ages the actors that have come to rest, and when the frame
 * mask allows, launches the first idle one on a random bearing. */
/* FAKEMATCH: both WorkSetValues calls go through the Call3 inline wrapper. */
void SoruFunka_Func020010c4(void)
{
    u8 *work;
    s32 level;
    s32 t;
    u8 i;
    struct Ember *spark;
    struct Ember *ember;
    s16 angle;
    s32 x;
    s32 z;

    work = gFieldWork;
    level = Main_030003f0(gEmberTimer, 10);
    if (level != 0) {
        *(s32 *)(work + 0x40c) = 0;
        Call3((void (*)())Engine_WorkSetValuesIfNonNegative, level << 16, level << 16, 0x10000);
    } else {
        *(s32 *)(work + 0x40c) = 1;
        Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    }
    t = gEmberTimer;
    if (t != 0)
        gEmberTimer = t - 3;

    for (i = 0; i < 16; i++) {
        if (gEmberState[i] != 0) {
            spark = Engine_ActorGet(i + 16);
            if (spark->target_x == (s32)0x80000000 && spark->target_z == (s32)0x80000000) {
                gEmberState[i]++;
                if (gEmberState[i] == 2)
                    Engine_ObjectSetAnimation(spark, 3);
                if (gEmberState[i] == 19) {
                    spark->x = 0;
                    spark->y = 0;
                    spark->z = 0;
                    spark->velocity_x = 0;
                    spark->velocity_y = 0;
                    spark->velocity_z = 0;
                    spark->target_x = 0x80000000;
                    spark->target_y = 0x80000000;
                    spark->target_z = 0x80000000;
                    Engine_ObjectSetPalette(spark, 15);
                } else if (gEmberState[i] == 20) {
                    gEmberState[i] = 0;
                }
            }
        }
    }

    if (gEmberMask == 999)
        return;
    if (gEmberMask & gFrameCount)
        return;
    for (i = 0; i < 16; i++) {
        angle = Engine_MathModulo(Engine_RandomNext(), 0xffff);
        ember = Engine_ActorGet(i + 16);
        if (gEmberState[i] == 0) {
            if (Engine_GameFlagIsSet(0x246) == 0)
                Engine_AudioPlayCue(246);
            gEmberState[i] = 1;
            ember->motion_flags = 0;
            ember->speed = 0x80000;
            ember->acceleration = 0x10000;
            Engine_ActorSetSpriteFlags(ember, 0);
            ember->sprite->layer = 1;
            Engine_ObjectSetAnimation(ember, 2);
            Engine_ObjectSetScript(ember, (const void *)0x0200ba00);
            ember->x = Iwram_MulQ16(Engine_MathCos(angle), (((u32)(Engine_RandomNext() << 8) >> 16) << 16) + 0x1000000) + 0x1450000;
            ember->y = 0;
            ember->z = Iwram_MulQ16(Engine_MathSin(angle), (((u32)(Engine_RandomNext() << 8) >> 16) << 16) + 0x1000000) + 0x12e0000;
            x = Iwram_MulQ16(Engine_MathCos(angle), ((Engine_RandomNext() & 63) << 16) + 0x80000);
            z = Iwram_MulQ16(Engine_MathSin(angle), ((Engine_RandomNext() & 63) << 16) + 0x80000);
            Engine_ObjectSetPosition(ember, x + 0x1450000, 0, z + 0x11e0000);
            Engine_ObjectSetPalette(ember, 0);
            gEmberTimer = 30;
            return;
        }
    }
}
