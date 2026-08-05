#include "types.h"

/*
 * Place the scene object associated with the first usable point in `points`.
 *
 * This is the genuine outer owner beginning at 0x02000c2c.  Its high-register
 * prologue, 20-byte local frame, common return at 0x02000e2c, and following
 * literal pool form one 568-byte region ending immediately before 0x02000e64.
 * The diagnostic seed at 0x02000c8a is an entry into this owner's live frame,
 * not a separate C function.
 *
 * Service interfaces whose implementations are not yet reconstructed remain
 * old-style declarations.  Their observed return values are typed where this
 * function consumes them, while every call below preserves its observed
 * argument list.
 */
struct ViewState {
    u8 unknown00[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
};

struct PlacementPoint {
    s16 id;
    s16 x;
    s16 z;
    s16 vertical;
    void *object;
};

struct SceneObject {
    u8 unknown00[0x30];
    s32 scale;
    s32 color;
};

/* Resolved whole-owner call targets. */

















static s32 cell_center(s32 coordinate)
{
    return (s32)(((u32)coordinate & 0xfff00000u) + 0x00080000u);
}

static s32 tile_to_fixed(s16 coordinate)
{
    return (s32)((u32)(s32)coordinate << 20);
}

/* The original rounds negative fixed-point values before its arithmetic shift. */
static s32 fixed_to_tile(s32 coordinate)
{
    if (coordinate < 0)
        coordinate += 0x000fffff;
    return coordinate >> 20;
}

static s32 facing_quadrant(s32 facing)
{
    if (facing < 0)
        facing += 0x00003fff;
    return facing >> 14;
}

extern void * Func_02001cf2();
extern s32 Func_02001c9c();
extern void * Func_0200182a();
extern s32 Func_02001ce6();
extern s32 Func_020018d2();
extern s32 Func_02001d90();
extern void Func_02001e22();
extern void Func_02001e6a();
extern void Func_02001e28();
extern void Func_02001eb8();
extern void Func_02001de2();
extern void Func_02001e0e();
extern void Func_02001e54();
extern void Func_02001ea4();
extern void * Func_02001dfc();
extern void Func_02001e9c();
extern void Func_02001eba();
extern void Func_02001e90();
extern void Func_02001ee0();
extern void Func_02001e66();
extern void Func_02001e46();
extern void Func_02001f2e();
extern void Func_02001f34();
extern void Func_02001ece();
void Func_02000c2c(void *points)
{
    static const u8 *const rotationByQuadrant = (const u8 *)0x02009164;
    static const s8 *const offsetXByQuadrant = (const s8 *)0x02009168;
    static const s8 *const offsetZByQuadrant = (const s8 *)0x0200916c;
    volatile struct ViewState *view;
    volatile struct PlacementPoint *point;
    volatile struct SceneObject *object;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } position;
    s32 facing;
    s32 spawnZ = 0;
    s32 spawnX = 0;
    s32 quadrant;
    s32 transformed;
    s32 foundPosition;
    s32 attempt;
    u8 *effect;

    view = Func_02001cf2(0);
    facing = (view->heading + 0x2000) & 0xc000;

    position.x = cell_center(view->x);
    position.y = view->y;
    position.z = cell_center(view->z);
    Func_02001c9c(0x00100000, facing, &position);

        points,
    point = Func_0200182a(
        fixed_to_tile(position.x),
        fixed_to_tile(position.z)
    );
    if (point == 0)
        return;

    foundPosition = 0;
    for (attempt = 0; attempt <= 10; attempt++) {
        position.x = tile_to_fixed(point->x);
        position.z = tile_to_fixed(point->z);
        Func_02001ce6(0x00100000, facing, &position);

        if (Func_020018d2(
                fixed_to_tile(position.x),
                fixed_to_tile(position.z),
                point->vertical) != 0) {
            break;
        }

        foundPosition = 1;
        if (point->vertical == 0) {
            spawnX = position.x + 0x00200000;
            spawnZ = position.z + 0x00080000;
        } else {
            spawnX = position.x + 0x00080000;
            spawnZ = position.z + 0x00200000;
        }

        point->x = (s16)fixed_to_tile(position.x);
        point->z = (s16)fixed_to_tile(position.z);
    }

    if (!foundPosition)
        return;

    position.y = view->y;
    position.x = cell_center(view->x);
    position.z = cell_center(view->z);
    transformed = Func_02001d90(0x00080000, facing, &position);

    object = (volatile struct SceneObject *)point->object;
    quadrant = facing_quadrant(facing);
    Func_02001e22(transformed, facing);
    Func_02001e6a(0, 8);
    Func_02001e28(6);

    object->scale = 0x00008000;
    object->color = 0x00003333;
    Func_02001eb8(239);
    Func_02001de2(object, rotationByQuadrant[quadrant]);
    Func_02001e0e(object, spawnX, spawnZ, 0);
    Func_02001e54(6);
    Func_02001ea4(0, 2);

    effect = Func_02001dfc(27, 0x00000ccc);
    Func_02001e22(*(void **)(effect + 0x1e0), object);
    Func_02001e9c(0, 0x00004ccc, 0x00003333);
    Func_02001eba(
        0,
        offsetXByQuadrant[quadrant],
        offsetZByQuadrant[quadrant]
    );
    Func_02001e90(24);
    Func_02001ee0(0, 1);
    Func_02001e66(object);
    Func_02001e46(object, 1);
    Func_02001f2e(0x120);
    Func_02001f34(213);
    Func_02001eba(15);
    Func_02001ece();
}
