#include "types.h"
#include "field_event.h"
#include "flag_call.h"

struct OrbitEffect {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x1c];
    s32 radius;
    u8 unknown_34[0x1c];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[0x0e];
    u16 angle;
    u16 unknown_66;
    u8 unknown_68[4];
    void (*update)(struct OrbitEffect *effect);
};

extern u32 gFrameCount;
extern u32 Data_02006afc[];

u32 __umodsi3(u32 a, u32 b);
struct FieldActor *Object_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(u32 angle);
void Object_SetPartAttribute(struct OrbitEffect *object, s32 value);
struct OrbitEffect *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct OrbitEffect *object, const void *script);
void Func_02003210(struct FieldActor *self);

static inline struct OrbitEffect *CreateOrbitEffect(s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline evaluates the position before the effect type; called directly, the type's second half comes before the z load (differ 2). */
    return Func_02005574(0x2a0, x, y, z);
}

// Every third frame, or every frame once flag 0x200 is set, starts a particle
// circling actor 23 at a random height above it, as overlay 69e's
// Func_020001a4 does under flag 0x201.
void Func_0200326c(void)
{
    struct FieldActor *center;
    struct OrbitEffect *effect;
    struct FieldSprite *sprite;
    s32 angle;
    /* FAKEMATCH: the random value pinned to r2, where the native copies the call's result; unpinned it is shifted straight out of r0 (differ 2). */
    register u32 random asm("r2");
    s32 y;

    if (Flag_IsSet(0x200) == 0 && __umodsi3(gFrameCount, 3) != 0)
        return;
    center = Object_GetById(23);
    if (Flag_IsSet(0x200) != 0) {
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
        Func_0200556c(effect, Data_02006afc);
        Object_SetPartAttribute(effect, 1);
        effect->motion_flags = 0;
        angle = Random16Far() & 0xffff000;
        effect->angle = angle;
        effect->unknown_66 = 0;
        effect->update = (void (*)(struct OrbitEffect *))Func_02003210;
        effect->radius = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
        sprite->next_frame = 0;
        sprite->priority = 2;
    }
}
