/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * The regularity that makes these readable, and it is CHECKED per site rather
 * than assumed: the callee value always appeared in the draft's argument list
 * at exactly the position matching the veneer's register index.  The compiler
 * loaded the callee into rN and the draft read rN as argument N.  So a
 * __call_via_r0 site has ZERO real arguments, r1 has one, r2 has two.  Each
 * site below was confirmed by checking that the register and the argument
 * position agree.
 *
 * The provider read from Data_02008000.spawn_provider is the CALLEE, not an
 * argument: __call_via_r0 takes no arguments at all. The draft already had
 * the value right and its role wrong.
 */
#include "types.h"

struct SpawnRecord_0808e9c0 {
    s32 kind;
    s16 id;
    s16 condition;
    u32 flags;
};

struct SceneObject_0808e9c0 {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[0x0f];
    u8 active_23;
    u8 unknown_24[0x35];
    u8 active_59;
    u8 unknown_5a[10];
    s16 tile_x;
    s16 tile_z;
};

struct SpawnedObject_0808e9c0 {
    struct SceneObject_0808e9c0 *object;
    u8 id;
    u8 unknown_05;
    s8 x;
    s8 z;
};

struct MapState_0808e9c0 {
    u8 unknown_00[0x10];
    u8 *spawn_sequence;
};

struct Runtime_0808e9c0 {
    u8 unknown_000[0x11c];
    struct SpawnedObject_0808e9c0 spawned[10];
};

typedef struct SpawnRecord_0808e9c0 *(*SpawnProvider_0808e9c0)(void);

typedef struct SpawnRecord_0808e9c0 *(*SpawnProviderCall_0808e9c0)(void);

struct RuntimeServices_0808e9c0 {
    u8 unknown_00[0x24];
    SpawnProvider_0808e9c0 spawn_provider;
};

struct Dma3_0808e9c0 {
    const void *source;
    void *destination;
    u32 control;
};

extern struct MapState_0808e9c0 *Data_03001e70;
extern struct Runtime_0808e9c0 *Data_03001ebc;
extern struct RuntimeServices_0808e9c0 Data_02008000;

struct SceneObject_0808e9c0 *Func_080090c8(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);
void Func_0808e9a8(struct SceneObject_0808e9c0 *object);
void Func_080091e0(struct SceneObject_0808e9c0 *object, s32 value);
s32 Func_080770c0(s32 condition);
void Func_080090d0(struct SceneObject_0808e9c0 *object);
void Func_08009080(struct SceneObject_0808e9c0 *object, s32 mode);
void Func_08009140(struct SceneObject_0808e9c0 *object);

/*
 * Instantiate the map's class-3 and class-19 spawn records.  The sequence
 * supplies an X/Z tile pair and a record ID; at most ten created objects are
 * published to the runtime table.
 */
void Func_0808e9c0(void)
{
    volatile struct Dma3_0808e9c0 *dma =
        (volatile struct Dma3_0808e9c0 *)0x040000d4;
    struct SpawnedObject_0808e9c0 *output = Data_03001ebc->spawned;
    u8 *sequence = Data_03001e70->spawn_sequence;
    u32 zero = 0;
    s32 output_count = 0;

    dma->source = &zero;
    dma->destination = output;
    dma->control = 0x85000014;

    if (sequence == 0)
        return;

    for (;;) {
        struct SpawnRecord_0808e9c0 *record;
        u8 x = *sequence++;
        u8 z = *sequence++;
        u8 id;

        if (x == 0xff && z == 0xff)
            return;

        id = *sequence++;
        if ((u8)(id - 100) > 139)
            continue;

        record = ((SpawnProviderCall_0808e9c0)Data_02008000.spawn_provider)();
        while (record->kind != -1) {
            u32 kind = (u32)record->kind & 0x1ff;

            if (record->id == id && kind == 0x13) {
                struct SceneObject_0808e9c0 *object =
                    Func_080090c8(
                        0x14,
                        (s32)x * 0x100000 + 0x80000,
                        0,
                        (s32)z * 0x100000 + 0x80000);

                if (object != 0) {
                    s32 fixed_x;
                    s32 fixed_z;

                    Func_0808e9a8(object);
                    Func_080091e0(object, 0);
                    if (Func_080770c0(record->condition) != 0) {
                        if ((record->flags & 0xfff00000) == 0x500000) {
                            Func_080090d0(object);
                            record++;
                            continue;
                        }
                        Func_08009080(object, 2);
                    }
                    Func_08009140(object);

                    fixed_x = object->x;
                    fixed_z = object->z;
                    object->tile_x =
                        (s16)((fixed_x < 0 ? fixed_x + 0xffff : fixed_x) >>
                              16);
                    object->tile_z =
                        (s16)((fixed_z < 0 ? fixed_z + 0xffff : fixed_z) >>
                              16);
                    object->active_23 = 1;
                    object->active_59 = 1;

                    output->object = object;
                    output->id = (u8)record->id;
                    output->x =
                        (s8)((fixed_x < 0 ? fixed_x + 0xfffff : fixed_x) >>
                             20);
                    output->z =
                        (s8)((fixed_z < 0 ? fixed_z + 0xfffff : fixed_z) >>
                             20);
                    output++;
                    output_count++;
                    if (output_count == 10)
                        return;
                    break;
                }
            } else if (
                record->id == id &&
                kind == 3 &&
                (record->flags & 0xfff00000) == 0x300000 &&
                Func_080770c0(record->condition) == 0) {
                struct SceneObject_0808e9c0 *object =
                    Func_080090c8(
                        0x1c,
                        (s32)x * 0x100000 + 0x80000,
                        0,
                        (s32)z * 0x100000 + 0x80000);

                if (object != 0) {
                    s32 fixed_x;
                    s32 fixed_z;

                    Func_0808e9a8(object);
                    Func_080091e0(object, 0);
                    Func_08009140(object);
                    Func_08009080(object, 1);

                    fixed_x = object->x;
                    fixed_z = object->z;
                    object->tile_x =
                        (s16)((fixed_x < 0 ? fixed_x + 0xffff : fixed_x) >>
                              16);
                    object->tile_z =
                        (s16)((fixed_z < 0 ? fixed_z + 0xffff : fixed_z) >>
                              16);
                    object->active_23 = 1;
                    object->active_59 = 1;

                    output->object = object;
                    output->id = (u8)record->id;
                    output->x =
                        (s8)((fixed_x < 0 ? fixed_x + 0xfffff : fixed_x) >>
                             20);
                    output->z =
                        (s8)((fixed_z < 0 ? fixed_z + 0xfffff : fixed_z) >>
                             20);
                    output++;
                    output_count++;
                    if (output_count == 10)
                        return;
                    break;
                }
            }

            record++;
        }
    }
}
