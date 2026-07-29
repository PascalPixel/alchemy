typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

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

struct ObjectHeader *Func_020042f4();
struct SceneObject *Func_02004300();
struct EffectSlot *Func_020043ce();
struct EffectSlot *Func_0200445e();

void Func_02000902();
void Func_02000a6e();
void Func_02000a80();
void Func_02000acc();
void Func_02004348();
void Func_02004352();
void Func_02004374();
void Func_0200437e();
void Func_02004398();
void Func_020043b8();
void Func_020043e0();
void Func_020043ec();
void Func_020043f0();
void Func_02004408();
void Func_02004424();
void Func_02004452();
void Func_0200446a();
void Func_02004480();
void Func_02004498();
void Func_020044b0();
void Func_020044ba();
void Func_020045a0();
void Func_02004606();
void Func_02004618();
void Func_0200461e();
void Func_020046e4();
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
    s32 enableOptionalEffect
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

    header = Func_020042f4(0);
    state = header->state >> 12;
    object = Func_02004300(objectId);
    footprint = &Data_0200bf0c[footprintIndex];

    height = footprint_span(footprint->z0, footprint->z1);
    width = footprint_span(footprint->x0, footprint->x1);

    object->effectScale = 0x00008000;
    object->effectColor = 0x00001999;
    originalX = object->x;
    originalZ = object->z;
    originalCellX = footprint_cell(originalX, footprint->x0);
    originalCellZ = footprint_cell(originalZ, footprint->z0);

    Func_02000902(0, originalCellX, originalCellZ, width, height, 0);
    Func_02004398(0, 0x00008000, 0x00001999);
    Func_020043f0(0, 8);
    Func_0200437e(15);
    Func_02004408(
        0,
        rounded_phase_delta(x, originalX),
        rounded_phase_delta(z, originalZ)
    );

    effect = Func_020043ce(0);
    effect->callback = Func_020082a8;
    Func_020043b8(4);

    if ((u32)(state - 6) <= 7)
        Func_02004348((struct SceneObject *)object, 3);
    else
        Func_02004352((struct SceneObject *)object, 2);

    Func_020045a0(239);
    Func_02004374((struct SceneObject *)object, x, y, z);
    Func_02004452(0);
    Func_0200446a(0, 2);
    Func_02004424(0, 0x00004ccc, 0x00001999);

    direction = &Data_0200beb4[state];
    Func_02004480(0, direction->x / 2, direction->z / 2);
    if (enableOptionalEffect != 0)
        Func_02004606();

    Func_02004498(0);
    Func_020044b0(0, 1);
    effect = Func_0200445e(0);
    effect->callback = (void (*)(void))0;
    Func_020043e0((struct SceneObject *)object);
    Func_02004618(288);
    Func_0200461e(213);

    object->x = x;
    object->z = z;
    object->velocityX = 0;
    object->velocityZ = 0;
    Func_020043ec((struct SceneObject *)object, 1);

    targetCellX = footprint_cell(x, footprint->x0);
    targetCellZ = footprint_cell(z, footprint->z0);
    viewCellX = world->viewX >> 20;
    viewCellZ = world->viewZ >> 20;
    targetWorldX = add_wrapped(viewCellX, targetCellX);
    targetWorldZ = add_wrapped(viewCellZ, targetCellZ);

    Func_0200445e(
        targetCellX,
        targetCellZ,
        width,
        height,
        targetWorldX,
        targetWorldZ
    );
    Func_02000a6e(0, targetCellX, targetCellZ, width, height, 255);
    Func_02000a80(2, targetCellX, targetCellZ, width, height, 255);

    /*
     * The final helpers use the object's original cell, not the requested
     * destination used by the three calls above.  The machine code recomputes
     * this pair from the saved original coordinates after those calls.
    */
    originalCellX = footprint_cell(originalX, footprint->x0);
    originalCellZ = footprint_cell(originalZ, footprint->z0);
    originalWorldX = add_wrapped(viewCellX, originalCellX);
    originalWorldZ = add_wrapped(viewCellZ, originalCellZ);
    Func_020044ba(
        originalWorldX,
        originalWorldZ,
        width,
        height,
        originalCellX,
        originalCellZ
    );
    Func_02000acc(2, originalCellX, originalCellZ, width, height, 0);
    Func_020046e4();
}
