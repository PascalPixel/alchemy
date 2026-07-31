#include "types.h"

/*
 * The proximity variant of the map spawn walk already reconstructed at
 * 0x0808e9c0.  It reads the same three-byte sequence records - tile X, tile Z,
 * record id, terminated by the 0xff/0xff pair - but only instantiates a record
 * whose tile is within eight tiles of the party leader on X and five on Z, and
 * it instantiates kind 22 objects from class-3 records with one of five
 * accepted flag classes.
 *
 * The leader is the object named by the party word at Data_02000240 + 0x1f4,
 * the same slot 0x0800ebec reads; its whole-tile coordinates are the plain
 * arithmetic shifts asrs #20 at 0x0808f354 and 0x0808f35a, with no rounding
 * bias, so they are written as >> 20.
 *
 * The bl Func_080072e4 at 0x0808f3b2 is the runtime _call_via_r0 thunk inside
 * 0x080072e4..0x08007320 - index (0x080072e4 - 0x080072e4) / 4 = 0 - so it is
 * an ordinary indirect call through r0, which holds the provider pointer at
 * Data_02008000 + 0x24, exactly as at 0x0808e9c0.
 *
 * The mov r0,sl at 0x0808f36a loads the record id into r0 for the
 * bl Func_0808ed4c at 0x0808f36e.  The byte-exact source at src/0808ed4c.c
 * takes no parameter, so the callee ignores the value; the call site is
 * reconstructed as it stands, with the argument declared.
 *
 * The record loop is entered by the guarded do/while at 0x0808f3c0 and
 * 0x0808f456 and runs to the -1 terminator without breaking out, so a matched
 * record does not stop the scan.
 *
 * Unlike 0x080c0cec's row this owner ends 4-aligned at 0x0808f480 with no
 * filler halfword.  The epilogue at 0x0808f46e unwinds the frame and ends
 * pop {r0} ; bx r0, so the owner is void.  One direct caller, 0x08098460.
 */

struct SpawnRecord_0808f32c {
    s32 kind;
    s16 id;
    s16 condition;
    u32 flags;
};

struct SceneObject_0808f32c {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[0x58];
    const void *handler;
};

struct MapState_0808f32c {
    u8 unknown_00[0x10];
    u8 *spawn_sequence;
};

typedef struct SpawnRecord_0808f32c *(*SpawnProvider_0808f32c)(void);

struct RuntimeServices_0808f32c {
    u8 unknown_00[0x24];
    SpawnProvider_0808f32c spawn_provider;
};

extern struct MapState_0808f32c *Data_03001e70;
extern struct RuntimeServices_0808f32c Data_02008000;
extern u8 Data_02000240[];

struct SceneObject_0808f32c *Func_0808ba1c(u32 slot);
s32 Func_0808ed4c(s32 id);
struct SpawnRecord_0808f32c *Func_080072e4(SpawnProvider_0808f32c provider);
s32 Func_080770c0(s32 condition);
struct SceneObject_0808f32c *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(struct SceneObject_0808f32c *object, const void *script);
void Func_080091e0(struct SceneObject_0808f32c *object, s32 value);

void Func_0808f32c(void)
{
    u8 *sequence = Data_03001e70->spawn_sequence;
    struct SceneObject_0808f32c *leader =
        Func_0808ba1c(*(u32 *)(Data_02000240 + 0x1f4));
    s32 leader_x = leader->x >> 20;
    s32 leader_z = leader->z >> 20;

    if (sequence == 0)
        return;

    for (;;) {
        struct SpawnRecord_0808f32c *record;
        s32 x = *sequence++;
        s32 z = *sequence++;
        s32 id;
        s32 difference;

        if (x == 255 && z == 255)
            return;

        id = *sequence++;
        if (Func_0808ed4c(id) != 0)
            continue;
        if ((u8)(id - 100) > 139)
            continue;

        difference = leader_x - x;
        if (difference < 0) {
            if (x - leader_x > 8)
                continue;
        } else if (difference > 8) {
            continue;
        }

        difference = leader_z - z;
        if (difference < 0) {
            if (z - leader_z > 5)
                continue;
        } else if (difference > 5) {
            continue;
        }

        record = Func_080072e4(Data_02008000.spawn_provider);
        while (record->kind != -1) {
            u32 flags;

            if (record->id != id || (record->kind & 15) != 3) {
                record++;
                continue;
            }

            flags = record->flags & 0xfff00000;
            if (flags != 0 &&
                flags != 0x100000 &&
                flags != 0x200000 &&
                flags != 0x300000 &&
                flags != 0x500000) {
                record++;
                continue;
            }

            if (record->condition != -1 &&
                Func_080770c0(record->condition) == 0) {
                struct SceneObject_0808f32c *object =
                    Func_080090c8(22,
                                  x * 0x100000 + 0x80000,
                                  0,
                                  z * 0x100000 + 0x80000);

                if (object != 0) {
                    Func_08009098(object, (const void *)0x0809e8a0);
                    Func_080091e0(object, 0);
                    object->handler = (const void *)0x0808f28d;
                }
            }

            record++;
        }
    }
}
