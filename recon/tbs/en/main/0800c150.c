/* 2026-09-29 alchemy permute: score 975 to 60 on the permuter's scorer (0
   is exact); remaining 1 reordered. Kept rewrites: 1x remove a temporary
   (permuter, to 455), then by hand the case-2 list entry assigned in one
   chained statement with the list pointer (the entry add now takes r4 as
   reload register because the kind store address already holds r3) and the
   first sprite stored through entry++. The one move left is the reload
   copy of entry (mov r2, r8) that the reference schedules above the radius
   store. The script pointer 0x0801358c still needs its own label in the
   unidentified data before adoption. */
/* Draft, not exact. Whole owner [0800c150, 0800c2d8), 392 bytes with own
   pools. Baseline on 2026-09-26: 376 bytes, 148 differing halfwords,
   82 aligned edits; equal topology, not merely pool placement.
   The two direct callers at 0800ebec/0800f2f8 pass descriptor IDs 25/24
   plus object XYZ and consume the returned object and its sprite at +80.
   Callees: FindFreeObject returns one of 64 112-byte slots; bc70 returns
   a sprite; 185000 returns a 20-byte descriptor; d130 sets XYZ and clears
   motion. All four interfaces and the complete own listing were checked.

   H1: separate u8 zero lifetimes should recover short-reach pool loads at
   animation_kind and unknown_59, preserving the 8-byte frame and calls.
   Prediction: pools after case 0 and before the tile-coordinate stores.
   Acceptance: exact 392 bytes, then compare/coverage/verify. Diagnostic:
   full aligned difference plus generated pool placement. One trial only;
   retain the candidate and result here if the prediction fails.
   H1 result: 392/392 bytes, 90 differing halfwords, 45 aligned edits.
   All nine pool words now match at their exact offsets, including both
   zero pools and the repeated 0xffff word. Frame/calls/topology remain.
   Residual: case-2 list entry uses an extra copy and shifts the join by
   two bytes; the initialization block owns the byte-store bases and
   plain zero in different registers; final signed divide uses r2/r4.
   The old pool-placement issue is closed, not an adopted function. */

#include "DMA.H"

struct FieldObject {
    u32 script;
    u16 unknown_04;
    u16 unknown_06;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 unknown_22[14];
    s32 speed_limit;
    s32 acceleration;
    u8 unknown_38[12];
    s32 unknown_44;
    s32 unknown_48;
    s32 unknown_4c;
    void *animation;
    u8 animation_kind;
    u8 unknown_55;
    u8 unknown_56[3];
    u8 unknown_59;
    u8 unknown_5a;
    u8 unknown_5b[9];
    s16 tile_x;
    s16 tile_z;
};

struct AnimationMetadata {
    u8 unknown_00[9];
    u8 radius;
};

struct ObjectSpriteList {
    u8 unknown_00[24];
    s32 count;
};

extern struct ObjectSpriteList *Data_03001e68;

struct FieldObject *ObjectDispatch_FindFreeObject(void);
void *Func_0800bc70(s32 id);
struct AnimationMetadata *Func_08185000(s32 id);
void Object_SetPositionAndResetMotion(struct FieldObject *object, s32 x, s32 y, s32 z);

struct FieldObject *Func_0800c150(s32 id, s32 x, s32 y, s32 z)
{
    struct FieldObject *object;
    void *sprite;
    s32 kind;
    u32 *list;
    u32 *entry;
    volatile u32 zero;

    ObjectDispatch_FindFreeObject();
    kind = id / 4096;
    id &= 0xfff;
    object = ObjectDispatch_FindFreeObject();
    if (object != NULL) {
        object->radius = 16;
        switch (kind) {
        case 0:
            sprite = Func_0800bc70(id);
            if (sprite != NULL) {
                object->animation_kind = 1;
                object->animation = sprite;
                object->radius = Func_08185000(id)->radius >> 1;
            } else {
                /* FAKEMATCH: narrow zero retains its own pool-load lifetime. */
                u8 empty_kind = 0;
                object->animation_kind = empty_kind;
            }
            break;
        case 2:
            entry = (list = (u32 *)Data_03001e68 + Data_03001e68->count++) + 2;
            object->animation_kind = kind;
            zero = 0;
            object->animation = entry;
            Dma_Set(&zero, entry, 0x85000004, (volatile u32 *)0x040000d4);
            sprite = Func_0800bc70(id);
            if (sprite != NULL) {
                object->radius = Func_08185000(id)->radius >> 1;
                *entry++ = (u32)sprite;
            }
            sprite = Func_0800bc70(id + 1);
            if (sprite != NULL)
                *entry = (u32)sprite;
            break;
        }
    }
    if (object != NULL) {
        Object_SetPositionAndResetMotion(object, x, y, z);
        object->script = 0x0801358c;
        object->speed_limit = 0x20000;
        object->unknown_04 = 0;
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        object->acceleration = 0x10000;
        object->unknown_55 = 3;
        object->unknown_48 = 0x10000;
        object->unknown_44 = 0x4000;
        object->unknown_59 = (u8)0;
        object->unknown_5a = 1;
        object->unknown_4c = 0;
        object->unknown_06 = 0x4000;
        object->tile_x = x / 65536;
        object->tile_z = z / 65536;
    }
    return object;
}
