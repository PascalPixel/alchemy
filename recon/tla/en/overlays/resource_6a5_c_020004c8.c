/* Near miss, differ 3 of 94: the twin of The Broken Seal's SceneEffect_SpawnParticlesAboveActor, through an Object_Create inline as ☀️'s scene layer has it (a direct call: differ 20). The native loads center->y before the random value's first shift in each branch, so jump2's cross-jump joins the branches after that shift; here sched2 puts the shift first (priority 39 against the load's 38), and the load joins the common tail. Tried 2026-10-15: a Random_Next inline, the shift written five ways, the additions reassociated (same or worse); rnd and y in locals with an empty asm barrier after the load in each branch (16: the call result is no longer copied before the shift); alchemy permute 4 minutes (no gain). */
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

void Func_020004c8(void)
{
    struct ObjectRuntime *center;
    struct ObjectRuntime *effect;
    struct FieldSprite *sprite;
    s32 angle;

    if (Flag_IsSet(0x201) == 0 && gFrameCount % 3 != 0)
        return;
    center = Object_GetById(31);
    if (Flag_IsSet(0x201) != 0)
        effect = Object_Create(672, center->x,
            (s32)((u32)(Random16Far() << 8) >> 16 << 16) + center->y - 0x1c0000, center->z);
    else
        effect = Object_Create(672, center->x,
            (s32)((u32)(Random16Far() << 6) >> 16 << 16) + center->y - 0x1c0000, center->z);
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
