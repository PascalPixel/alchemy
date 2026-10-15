#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "flag_call.h"

extern u32 gFrameCount;
extern const u8 Data_020068b0[];

u32 Random16Far(void);
s32 Math_Sine(u32 angle);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Func_0200576c(s32 type, s32 x, s32 y, s32 z);
void Func_02005764(struct ObjectRuntime *object, const void *script);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02000090(void);

static inline struct ObjectRuntime *Object_Create(s32 type, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters order the arguments; a direct call loads the type first */
    return Func_0200576c(type, x, y, z);
}

// Every third frame, or every frame once flag 0x201 is set, spawns a particle
// at a random height above object 31 that circles it (Func_02000090).
void Func_020004c8(void)
{
    struct ObjectRuntime *center;
    struct ObjectRuntime *effect;
    struct FieldSprite *sprite;
    s32 angle;

    if (Flag_IsSet(0x201) == 0 && gFrameCount % 3 != 0)
        return;
    center = Object_GetById(31);
    if (Flag_IsSet(0x201) != 0) {
        u32 random;
        s32 y;

        random = Random16Far();
        /* FAKEMATCH: the barriers keep the random value's copy, the centre's y load and the first shift in the native's order in both branches, so jump2 joins them after the shift (differ 3 without). */
        asm("");
        y = center->y;
        asm("");
        effect = Object_Create(672, center->x, (s32)((u32)(random << 8) >> 16 << 16) + y - 0x1c0000, center->z);
    } else {
        u32 random;
        s32 y;

        random = Random16Far();
        /* FAKEMATCH: as in the first branch. */
        asm("");
        y = center->y;
        asm("");
        effect = Object_Create(672, center->x, (s32)((u32)(random << 6) >> 16 << 16) + y - 0x1c0000, center->z);
    }
    if (effect != 0) {
        sprite = effect->animation;
        Func_02005764(effect, Data_020068b0);
        Object_SetPartAttribute(effect, 1);
        effect->motion_flags = 0;
        angle = Random16Far() & 0xffff000;
        effect->action = angle;
        effect->unknown_66 = 0;
        effect->update = Func_02000090;
        effect->speed_limit = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
        sprite->next_frame = 0;
        sprite->priority = 1;
    }
}
