#include "TYPES.H"

struct FieldObject {
    u8 reserved_00[8];
    s32 x;
    u8 reserved_0c[4];
    s32 z;
    u8 reserved_14[0x3c];
    u8 *sprite;
    u8 removed;
};

struct ObjectTableWork {
    u8 reserved_000[0x34];
    struct FieldObject *objects[58];
    u8 reserved_11c[0xc4];
    struct FieldObject *anchor;
};

extern struct ObjectTableWork *Data_03001ebc;

void Object_Destroy(struct FieldObject *object);


/* Destroys every object in table slots 8 to 65 that has left the area around
 * the anchor object, 160 units either side of it in x and from 200 less to
 * 100 more in z; objects still at x and z zero are kept. Each one is marked
 * removed, bit 0 of its sprite's byte 29 is cleared, and its slot emptied. */
void BattleEffect_ClearOutOfBoundsObjects(void)
{
    struct ObjectTableWork *work = Data_03001ebc;
    struct FieldObject *anchor;
    s32 x;
    s32 z;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    struct FieldObject **slot;
    s32 mask;
    s32 i;

    anchor = work->anchor;
    x = anchor->x;
    left = x - 0xa00000;
    right = x + 0xa00000;
    z = anchor->z;
    top = z - 0xc80000;
    bottom = z + 0x640000;
    slot = work->objects;
    mask = ~1;

    for (i = 57; i >= 0; i--) {
        struct FieldObject *object = *slot;

        if (object != NULL) {
            s32 object_x = object->x;
            s32 object_z = object->z;

            if (object_x != 0 || object_z != 0) {
                if (object_x < left || object_x > right ||
                    object_z < top || object_z > bottom) {
                    u8 *sprite;
                    u8 *removed = &object->removed;
                    u32 flags;

                    *removed = 1;
                    sprite = object->sprite;
                    flags = sprite[29];
                    sprite[29] = flags & mask;
                    Object_Destroy(object);
                    sprite = NULL;
                    *slot = (struct FieldObject *)sprite;
                }
            }
        }

        slot++;
    }
}
