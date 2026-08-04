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
extern void * Func_02000b8c();
extern s32 Func_02000be4();
extern s32 Func_08000128();
extern void * Func_08000148();
extern void Func_08009080();
extern void Func_080090e0();
extern void Func_08009150();
extern void Func_08009158();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern void * Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a0e0();
extern void Func_0808a100();
extern void Func_080f9010();

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
    s32 spawnX = 0;
    s32 spawnZ = 0;
    s32 quadrant;
    s32 transformed;
    s32 foundPosition;
    s32 attempt;
    u8 *effect;

    view = Func_0808a080(0);
    facing = (view->heading + 0x2000) & 0xc000;

    position.x = cell_center(view->x);
    position.y = view->y;
    position.z = cell_center(view->z);
    Func_08000128(0x00100000, facing, &position);

    point = Func_02000b8c(
        points,
        fixed_to_tile(position.x),
        fixed_to_tile(position.z)
    );
    if (point == 0)
        return;

    foundPosition = 0;
    for (attempt = 0; attempt <= 10; attempt++) {
        position.x = tile_to_fixed(point->x);
        position.z = tile_to_fixed(point->z);
        Func_08000128(0x00100000, facing, &position);

        if (Func_02000be4(
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

    position.x = cell_center(view->x);
    position.y = view->y;
    position.z = cell_center(view->z);
    transformed = Func_08000128(0x00080000, facing, &position);

    object = (volatile struct SceneObject *)point->object;
    quadrant = facing_quadrant(facing);
    Func_0808a018(transformed, facing);
    Func_0808a100(0, 8);
    Func_0808a010(6);

    object->scale = 0x00008000;
    object->color = 0x00003333;
    Func_080f9010(239);
    Func_08009080(object, rotationByQuadrant[quadrant]);
    Func_08009150(object, spawnX, spawnZ, 0);
    Func_0808a010(6);
    Func_0808a100(0, 2);

    effect = Func_08000148(27, 0x00000ccc);
    Func_080090e0(*(void **)(effect + 0x1e0), object);
    Func_0808a090(0, 0x00004ccc, 0x00003333);
    Func_0808a0e0(
        0,
        offsetXByQuadrant[quadrant],
        offsetZByQuadrant[quadrant]
    );
    Func_0808a010(24);
    Func_0808a100(0, 1);
    Func_08009158(object);
    Func_08009080(object, 1);
    Func_080f9010(0x120);
    Func_080f9010(213);
    Func_0808a010(15);
    Func_0808a020();
}
