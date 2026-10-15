#include "types.h"
#include "field_event.h"
#include "flag_call.h"

struct OrbitEffect {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[0x1c];
    s32 radius;
    u8 unk34[0x1c];
    struct FieldSprite *sprite;
    u8 unk54;
    u8 motionFlags;
    u8 unk56[0x0e];
    u16 angle;
    u16 unk66;
    u8 unk68[4];
    void (*update)(struct OrbitEffect *effect);
};

extern u32 gFrameCount;
extern u32 gData_02003214[];

u32 __umodsi3(u32 a, u32 b);
struct FieldActor *Object_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Cosine(u32 angle);
s32 Math_Sine(u32 angle);
void Object_SetPartAttribute(struct OrbitEffect *object, s32 value);
struct OrbitEffect *Func_02002828(s32 type, s32 x, s32 y, s32 z);
void Func_02002820(struct OrbitEffect *object, const void *script);

// Circles the object around actor 23, as overlay 6a5's Func_02000090 circles actor 31.
void Func_02000148(struct FieldActor *self)
{
    struct FieldActor *centre = Object_GetById(23);
    u16 angle = self->timer;
    s32 sine;
    s32 x;
    s32 z;

    self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
    sine = Math_Sine(angle);
    z = centre->z;
    x = self->x;
    asm volatile(""); /* FAKEMATCH: both positions are read before the sine is doubled; without it the shift comes before the second read */
    z += sine * 2;
    self->z = z;
    self->targetX = x;
    asm volatile(""); /* FAKEMATCH: the target z store stays ahead of the timer load; without it the store falls behind (6 differ; placed by alchemy permute) */
    self->targetZ = z;
    *(s16 *)&self->timer += -0x800;
}

static inline struct OrbitEffect *CreateOrbitEffect(s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline evaluates the position before the effect type; called directly, the type's second half comes before the z load (differ 2). */
    return Func_02002828(0x2a0, x, y, z);
}

// Every third frame, or every frame once flag 0x201 is set, starts a particle
// circling actor 23 at a random height above it.
void Func_020001a4(void)
{
    struct FieldActor *center;
    struct OrbitEffect *effect;
    struct FieldSprite *sprite;
    s32 angle;
    /* FAKEMATCH: the random value pinned to r2, where the native copies the call's result; unpinned it is shifted straight out of r0 (differ 2). */
    register u32 random asm("r2");
    s32 y;

    if (Flag_IsSet(0x201) == 0 && __umodsi3(gFrameCount, 3) != 0)
        return;
    center = Object_GetById(23);
    if (Flag_IsSet(0x201) != 0) {
        random = Random16Far();
        /* FAKEMATCH: the two barriers keep the copy of the random value, then the height load, ahead of the shift; without the first differ 4, without the second differ 3. */
        asm("");
        y = center->y;
        asm("");
        effect = CreateOrbitEffect(center->x, (s32)((u32)(random << 8) >> 16 << 16) + y - 0x1c0000, center->z);
    } else {
        random = Random16Far();
        /* FAKEMATCH: as above; both branches end alike, so the jump pass shares their tail. */
        asm("");
        y = center->y;
        asm("");
        effect = CreateOrbitEffect(center->x, (s32)((u32)(random << 6) >> 16 << 16) + y - 0x1c0000, center->z);
    }
    if (effect != 0) {
        sprite = effect->sprite;
        Func_02002820(effect, gData_02003214);
        Object_SetPartAttribute(effect, 1);
        effect->motionFlags = 0;
        angle = Random16Far() & 0xffff000;
        effect->angle = angle;
        effect->unk66 = 0;
        effect->update = (void (*)(struct OrbitEffect *))Func_02000148;
        effect->radius = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
        sprite->nextFrame = 0;
        sprite->priority = 1;
    }
}
