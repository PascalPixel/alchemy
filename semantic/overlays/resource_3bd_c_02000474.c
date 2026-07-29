typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Search the footprint grid along one packed direction until the collision
 * service reports its boundary.
 *
 * This is the genuine outer owner beginning at 0x02000474.  Its high-register
 * prologue, 32-byte local frame, common return at 0x020005f8, and following
 * literal pool form one 404-byte region ending immediately before 0x02000608.
 *
 * The call target at 0x020007de lies numerically inside another overlay span,
 * but is used here as a normal resident service: it receives exactly three
 * arguments, fills the direction index and result metadata, and returns the
 * scene object being tested.  It is not asserted as another semantic owner.
 */

struct FixedPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct SearchResult {
    s32 footprintIndex;
    s32 metadata;
    struct FixedPosition position;
    s32 state;
};

struct SceneObject {
    u8 unknown00[8];
    struct FixedPosition position;
    u8 unknown14[14];
    u8 collisionMode;
};

struct FootprintExtent {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

/*
 * Each direction is one little-endian packed word.  The low half controls Z
 * and the high half controls X; promoting either half to fixed point therefore
 * reproduces the two shift/mask paths used by the original routine.
 */
struct SearchDirection {
    s16 z;
    s16 x;
};

struct SceneObject *Func_020007de(
    s32 *directionIndex,
    s32 *metadata,
    struct SearchResult *result
);
s32 Func_020041b4(
    struct SceneObject *object,
    struct FixedPosition *position
);

static s32 absolute_value(s32 value)
{
    return value < 0 ? -value : value;
}

static s32 fixed_from_low_half(s32 value)
{
    return (s32)((u32)value << 16);
}

static s32 fixed_from_half(s16 value)
{
    return (s32)((u32)(u16)value << 16);
}

static s32 add_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left + (u32)right);
}

static s32 multiply_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left * (u32)right);
}

s32 Func_02000474(struct SearchResult *result)
{
    const struct FootprintExtent *footprints =
        (const struct FootprintExtent *)0x0200bf0c;
    const struct SearchDirection *directions =
        (const struct SearchDirection *)0x0200beb4;
    struct SceneObject *object;
    struct SearchDirection direction;
    s32 directionIndex;
    s32 footprintIndex;
    s32 scanWidth;
    s32 scanHeight;
    s32 originX;
    s32 originZ;
    s32 distance;
    s32 row;
    s32 column;

    result->state = 0;
    object = Func_020007de(&directionIndex, &result->metadata, result);
    if (object == (struct SceneObject *)0)
        return 0;

    object->collisionMode = 2;
    distance = 0;
    footprintIndex = result->footprintIndex;
    scanHeight = (
        absolute_value(footprints[footprintIndex].z0) +
        absolute_value(footprints[footprintIndex].z1)
    ) >> 4;
    scanWidth = (
        absolute_value(footprints[footprintIndex].x0) +
        absolute_value(footprints[footprintIndex].x1)
    ) >> 4;

    direction = directions[directionIndex];
    originX = add_wrapped(object->position.x, fixed_from_half(direction.x));
    originZ = add_wrapped(object->position.z, fixed_from_half(direction.z));
    result->position.y = object->position.y;

    for (;;) {
        /*
         * The footprint index and its leading X/Z extents are deliberately
         * re-read as in the machine code.  The collision service receives the
         * complete position and may modify it before the following increment.
         */
        footprintIndex = result->footprintIndex;
        result->position.z = add_wrapped(
            originZ,
            fixed_from_low_half(footprints[footprintIndex].z0)
        );

        for (row = 0; row < scanHeight; row++) {
            footprintIndex = result->footprintIndex;
            result->position.x = add_wrapped(
                originX,
                fixed_from_low_half(footprints[footprintIndex].x0)
            );

            for (column = 0; column < scanWidth; column++) {
                if (Func_020041b4(object, &result->position) == 2)
                    goto boundary_found;
                result->position.x =
                    add_wrapped(result->position.x, 0x00100000);
            }

            result->position.z =
                add_wrapped(result->position.z, 0x00100000);
        }

        distance++;
        direction = directions[directionIndex];
        originX = add_wrapped(originX, fixed_from_half(direction.x));
        originZ = add_wrapped(originZ, fixed_from_half(direction.z));
    }

boundary_found:
    object->collisionMode = 0;
    if (distance == 0)
        return 0;

    /*
     * Return the last clear step, rather than the position at which the
     * collision was observed.  The object fields are re-read because the
     * collision service may have changed them.
     */
    direction = directions[directionIndex];
    result->position.x = add_wrapped(
        object->position.x,
        multiply_wrapped(distance, fixed_from_half(direction.x))
    );
    result->position.y = object->position.y;
    result->position.z = add_wrapped(
        object->position.z,
        multiply_wrapped(distance, fixed_from_half(direction.z))
    );
    return 1;
}
