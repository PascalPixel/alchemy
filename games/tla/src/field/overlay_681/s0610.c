#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 Data_020050d0[];

u32 Random16Far(void);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);

s32 Func_02000610(void)
{
    struct FieldSprite *sprite = Object_GetById(9)->animation;

    sprite->rotation -= 0x300;
    return 0;
}

// 拡大率を乱数で揺らす
void Func_0200062c(struct ObjectRuntime *object)
{
    u32 random = Random16Far();
    s32 scale = object->scale_x;
    s32 bias;
    s32 limit;

    /* FAKEMATCH: the native loads the scale and then the pooled -0x800 before it shifts the random value; without these two barriers the shifts come before the pool load (one reordered instruction, differ 2), and with only the second the pool load rises above the call. */
    __asm__ volatile("");
    bias = -0x800;
    /* FAKEMATCH: second barrier of the pair above: the pool load stays before the shifts. */
    __asm__ volatile("" : "+r"(bias));
    scale += (random << 12) >> 16;
    scale += bias;
    object->scale_x = scale;
    object->scale_y = object->scale_y + ((Random16Far() << 12) >> 16) + bias;
    limit = 240;
    /* FAKEMATCH: second barrier of the three above: the limit is started before scale_x is loaded. */
    __asm__ volatile("" : "+r"(limit));
    scale = object->scale_x;
    /* FAKEMATCH: third barrier of the three above: scale_x is loaded before the limit is shifted. */
    __asm__ volatile("" : "+r"(limit));
    limit <<= 4;
    limit += 255;
    if (scale <= limit)
        object->scale_x = 0x2000;
    if (object->scale_y <= limit)
        object->scale_y = 0x2000;
}

s32 Func_02000678(void)
{
    struct ObjectRuntime *source = Object_GetById(9);
    struct ObjectRuntime *object;
    struct FieldSprite *sprite;
    s32 x = source->x;

    /* FAKEMATCH: the native loads the source x before it starts the offset 0x80000; without the barrier the offset is started first (one reordered pair, differ 2). */
    __asm__ volatile("" : "+r"(x));
    object = Engine_ObjectCreate(14, x + 0x80000, source->y - 0x20000, source->z);
    object->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004dac(object, Data_020050d0);
    object->scale_x = 0x6666;
    object->scale_y = 0x6666;
    /* FAKEMATCH: the native stores both scales before it loads the sprite; without the object passed through this asm the sprite is loaded while the scale is built (two reordered instructions, differ 4). A volatile barrier here swaps the two arguments of the call above instead. */
    __asm__("" : "+r"(object));
    sprite = object->animation;
    sprite->priority = 1;
    object->bounce = 0;
    object->unknown_4c = 0;
    object->update = (void (*)(void))Func_0200062c;
    return 0;
}
