#include "types.h"

/*
 * Configure the rectangular scene effect associated with one footprint.
 *
 * This is the independent owner beginning at 0x02000608.  The unusual first
 * `sub sp` reserves the four incoming-register spill slots before the saved
 * register prologue.  Its outer return is at 0x020008a0, followed by a literal
 * pool through 0x020008bc; the next saved-register prologue starts at
 * 0x020008c0.  Branch targets at 0x020007de, 0x02000902, 0x02000a6e,
 * 0x02000a80, and 0x02000acc are shared hidden-context continuations/services,
 * not additional owners of this C file.
 *
 * Several resident services share addresses with code in the overlay image
 * and one service address is called with different observed argument lists.
 * Old-style declarations preserve those call-site ABIs without pretending
 * that the shared interfaces have already been reconstructed.
 */

struct FootprintExtent {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

struct SearchDirection {
    s16 z;
    s16 x;
};

struct ObjectHeader {
    u8 unknown00[6];
    u16 state;
};

struct SceneObject {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[16];
    s32 velocityX;
    u8 unknown28[4];
    s32 velocityZ;
    s32 effectScale;
    s32 effectColor;
};

struct WorldState {
    u8 unknown00[0x13c];
    s32 viewX;
    s32 viewZ;
};

struct EffectSlot {
    u8 unknown00[108];
    void (*callback)(void);
};

extern const struct FootprintExtent Data_0200bf0c[];
extern const struct SearchDirection Data_0200beb4[];
extern struct WorldState *Data_03001e70;

void *Func_0808a080();
void Func_02000244();
void Func_0808a090();
void Func_0808a100();
void Func_0808a010();
void Func_0808a0e0();
void Func_08009080();
void Func_080f9010();
void Func_08009150();
void Func_0808a0e8();
void Func_08009158();
void Func_080091c0();
void Func_0808a5e8();
void Func_020082a8(void);

static s32 add_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left + (u32)right);
}

static s32 subtract_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left - (u32)right);
}

static s32 absolute_wrapped(s32 value)
{
    return value < 0 ? (s32)(0u - (u32)value) : value;
}

static s32 footprint_span(s32 first, s32 second)
{
    return add_wrapped(absolute_wrapped(first), absolute_wrapped(second)) >> 4;
}

static s32 footprint_cell(s32 fixed, s32 extent)
{
    return add_wrapped(fixed, (s32)((u32)extent << 16)) >> 20;
}

/*
 * The engine represents a signed angular displacement in seventeen fractional
 * bits.  Its negative path adds one less than the divisor before shifting.
 */
static s32 rounded_phase_delta(s32 target, s32 origin)
{
    s32 delta = subtract_wrapped(target, origin);

    if (delta < 0)
        delta = add_wrapped(delta, 0x0001ffff);
    return delta >> 17;
}

void Func_02000608(
    s32 footprintIndex,
    s32 objectId,
    s32 x,
    s32 y,
    s32 z,
    void (*callback)(void)
)
{
    volatile struct WorldState *world = Data_03001e70;
    volatile struct ObjectHeader *header;
    volatile struct SceneObject *object;
    volatile struct EffectSlot *effect;
    const struct FootprintExtent *footprint;
    const struct SearchDirection *direction;
    s32 originalX;
    s32 originalZ;
    s32 originalCellX;
    s32 originalCellZ;
    s32 targetCellX;
    s32 targetCellZ;
    s32 originalWorldX;
    s32 originalWorldZ;
    s32 targetWorldX;
    s32 targetWorldZ;
    s32 viewCellX;
    s32 viewCellZ;
    s32 width;
    s32 height;
    s32 state;

    header = (struct ObjectHeader *)Func_0808a080(0);
    state = header->state >> 12;
    object = (struct SceneObject *)Func_0808a080(objectId);
    footprint = &Data_0200bf0c[footprintIndex];

    height = footprint_span(footprint->z0, footprint->z1);
    width = footprint_span(footprint->x0, footprint->x1);

    object->effectScale = 0x00008000;
    object->effectColor = 0x00001999;
    originalX = object->x;
    originalZ = object->z;
    originalCellX = footprint_cell(originalX, footprint->x0);
    originalCellZ = footprint_cell(originalZ, footprint->z0);

    Func_02000244(0, originalCellX, originalCellZ, width, height, 0);
    Func_0808a090(0, 0x00008000, 0x00001999);
    Func_0808a100(0, 8);
    Func_0808a010(15);
    Func_0808a0e0(
        0,
        rounded_phase_delta(x, originalX),
        rounded_phase_delta(z, originalZ)
    );

    effect = (struct EffectSlot *)Func_0808a080(0);
    effect->callback = Func_020082a8;
    Func_0808a010(4);

    if ((u32)(state - 6) <= 7)
        Func_08009080((struct SceneObject *)object, 3);
    else
        Func_08009080((struct SceneObject *)object, 2);

    Func_080f9010(239);
    Func_08009150((struct SceneObject *)object, x, y, z);
    Func_0808a0e8(0);
    Func_0808a100(0, 2);
    Func_0808a090(0, 0x00004ccc, 0x00001999);

    direction = &Data_0200beb4[state];
    Func_0808a0e0(0, direction->x / 2, direction->z / 2);
    if (callback != 0)
        callback();

    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    effect = (struct EffectSlot *)Func_0808a080(0);
    effect->callback = (void (*)(void))0;
    Func_08009158((struct SceneObject *)object);
    Func_080f9010(288);
    Func_080f9010(213);

    object->x = x;
    object->z = z;
    object->velocityX = 0;
    object->velocityZ = 0;
    Func_08009080((struct SceneObject *)object, 1);

    targetCellX = footprint_cell(x, footprint->x0);
    targetCellZ = footprint_cell(z, footprint->z0);
    viewCellX = world->viewX >> 20;
    viewCellZ = world->viewZ >> 20;
    targetWorldX = add_wrapped(viewCellX, targetCellX);
    targetWorldZ = add_wrapped(viewCellZ, targetCellZ);

    Func_080091c0(
        targetCellX,
        targetCellZ,
        width,
        height,
        targetWorldX,
        targetWorldZ
    );
    Func_02000244(0, targetCellX, targetCellZ, width, height, 255);
    Func_02000244(2, targetCellX, targetCellZ, width, height, 255);

    /*
     * The final helpers use the object's original cell, not the requested
     * destination used by the three calls above.  The machine code recomputes
     * this pair from the saved original coordinates after those calls.
    */
    originalCellX = footprint_cell(originalX, footprint->x0);
    originalCellZ = footprint_cell(originalZ, footprint->z0);
    originalWorldX = add_wrapped(viewCellX, originalCellX);
    originalWorldZ = add_wrapped(viewCellZ, originalCellZ);
    Func_080091c0(
        originalWorldX,
        originalWorldZ,
        width,
        height,
        originalCellX,
        originalCellZ
    );
    Func_02000244(2, originalCellX, originalCellZ, width, height, 0);
    Func_0808a5e8();
}
