#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 Data_02004f90[];

struct ObjectRuntime *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
void Func_02004b20(struct ObjectRuntime *object, const u8 *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);

static __inline__ struct ObjectRuntime *Func_02004b30_(s32 kind, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline builds the kind after the position loads, as in the native (direct: differ 4). */
    return Func_02004b30(kind, x, y, z);
}

// Spawns a pair of sparks at the object, as ☀️'s MakyuriHeya_SpawnSparkPair
// does at a spawn point.
void Func_020039f4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *spark;
    struct FieldSprite *sprite;
    s32 (*mulQ16)(s32 a, s32 b) = Iwram_MulQ16;

    /* FAKEMATCH: through a local pointer the resident multiply's entry loads before the factor's shift, as in the native (direct: differ 2; its result kept in a local: 2). */
    mulQ16(object->scale_x, 0x18000);
    spark = Func_02004b30_(271, object->x, object->y, object->z);
    if (spark != NULL) {
        spark->terrain_height = object->terrain_height;
        sprite = spark->animation;
        Func_02004b20(spark, Data_02004f90);
        spark->motion_flags = 0;
        spark->action = 0;
        spark->linked_object = object;
        if (sprite != NULL) {
            Animation_ApplyChildArgument(sprite, 2);
            sprite->next_frame = 0;
            sprite->priority = 1;
        }
    }
    spark = Func_02004b30_(271, object->x, object->y, object->z);
    if (spark != NULL) {
        spark->terrain_height = object->terrain_height;
        sprite = spark->animation;
        Func_02004b20(spark, Data_02004f90);
        spark->motion_flags = 0;
        spark->action = 0;
        spark->linked_object = object;
        spark->terrain_id = 1;
        if (sprite != NULL) {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->next_frame = 0;
        }
    }
}
