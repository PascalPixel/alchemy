/* Draft of Func_020010d8 (overlay 679, 46 instructions), differ 4: spawns object 14
 * above the given object, copies its ground height and sprite priority. Loading the
 * child's sprite before copying the height took it from differ 28 to 8. One
 * scheduling swap remains: the native stores next_frame before it loads the
 * parent's byte 9 and then its own; here the parent's sprite pointer is copied
 * first and its own byte is loaded after the mask. The height store before the
 * script address now comes from a tagged empty asm (differ 8 to 4); another empty
 * asm after next_frame's store or a u32 priority temporary changes nothing (4),
 * an empty asm after the animation call gives 6.
 * Measured: creation through a forwarding inline (no change); the priority copy as
 * a u8 bitfield (differ 8, other rows), as explicit masks (17 to 31). alchemy
 * permute, 5 minutes: nothing closer. */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 Data_02001ee8[];

struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgument(struct FieldSprite *sprite, s32 value);
void Func_02001c74(struct ObjectRuntime *object, const u8 *script);

void Func_020010d8(struct ObjectRuntime *object)
{
    struct ObjectRuntime *child;
    struct FieldSprite *parent;
    struct FieldSprite *sprite;

    child = Engine_ObjectCreate(14, object->x, object->y + 0x40000, object->z + 0x40000);
    parent = object->animation;
    if (child != NULL) {
        sprite = child->animation;
        child->terrain_height = object->terrain_height;
        /* FAKEMATCH: an empty asm keeps the height store ahead of the script address load, as in the native (differ 8 without). */
        asm("");
        Func_02001c74(child, Data_02001ee8);
        child->motion_flags = 0;
        if (sprite != NULL) {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->next_frame = 0;
            sprite->priority = parent->priority;
        }
    }
}
