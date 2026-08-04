#include "types.h"

/*
 * Core-drive row 0x0800c62c, 592 bytes. This row's
 * body as an ordinary, expressible idiom (retention=keep_structured_asm
 * was a default label, not a cannot-express contract) -- confirmed here
 * with a full draft after two earlier passes mapped its structure.
 *
 * bun tools/main_xref.ts 0800c62c: FUNCTION, indirectly published
 * callback (published at 0x0800c0b8/0x0800c5f0/0x0800c61c, a raw word
 * ref at 0x0848b6ec). Per-frame processor over the SAME 64-record,
 * 0x70-byte object array (Data_03001e64) semantic/main/0800ebec.c
 * already reconstructed as struct Object_0800ebec: five of this row's
 * own field accesses land exactly on that header's named fields
 * (program_00@0, heading_06@6, position_08[0/1/2]@8/12/16, ground_14@20)
 * -- redeclared locally here rather than shared via include/, the same
 * choice this reconstruction already made for struct SmshContext (a header-only
 * change stages no exact-byte movement and trips the commit hook's
 * progress-report requirement for nothing).
 *
 * Shape: claim (or reuse, since Func_080048b0 is idempotent per kind) a
 * 0x2c4-byte scratch buffer under kind 52, DMA a fixed ROM table into it,
 * then walk all 64 objects. Each live object (program_00 != 0) with
 * status nibble (byte 0x54 & 0xf) == 1 either takes a short release path
 * (clear a linked sub-object's busy state through Func_08003f78) or, when
 * its world position is non-degenerate and within a signed bounding box
 * around a reference point read from Data_03001e70, looks up a terrain
 * attribute word for its tile, folds two 2-bit sub-fields out of it into
 * the sub-object's own state bytes and back into the object's own kind
 * byte, and publishes a position/ground record plus two IWRAM-multiply
 * results to Func_0800b168. Releases the kind-52 buffer via Func_08002dd8
 * once every object has been walked.
 *
 * 0x03000118 is the same IWRAM-relocated ARM fixed-point multiply already
 * recorded as an ordinary indirect call for 0x0800447c/0x08004ab0/
 * 0x0808b3ec (mov ip,pc / bx fp here, fp preloaded once before the loop).
 *
 * Several intermediate values are transcribed without full narrative
 * confidence -- flagged inline with "Uncertainty:" comments, the same
 * convention semantic/main/0800ebec.c uses for its own unclear spots.
 * Not yet checked byte-exact.
 */

/* Shared fields confirmed against semantic/main/0800ebec.c's
   struct Object_0800ebec (Data_03001e64, 64 records of 0x70 bytes);
   only the fields this row touches are named. Byte 0x22/0x23 (that
   header's unknown_22) are read/written individually here as
   kind_22/flags_23. Bytes 0x54 (status_54) and 0x5c (busy_5c) and the
   embedded pointer at 0x50 (sub_50) are new to this row, inside that
   header's unknown_44[0x14] and unknown_5c[8] ranges respectively. */
struct Object_0800c62c {
    const void *program_00;
    u16 unknown_04;
    u16 heading_06;
    s32 position_08[3]; /* x, y, z */
    s32 ground_14;
    u8 unknown_18[8];
    u8 kind_22;   /* 0x22: selects the per-kind terrain sub-table */
    u8 flags_23;  /* 0x23: bit 0 gates the extra terrain sub-field fold */
    u8 unknown_24[0x18];
    void *sub_50; /* 0x50: pointer to a smaller linked object, below */
    u8 unknown_54[0x08];
    u8 busy_5c; /* 0x5c: nonzero suppresses all processing for this
                 * object this frame */
};

/* Linked sub-object reached through Object_0800c62c::sub_50. Not
   identified against any existing struct; fields named by role only. */
struct SubObject_0800c62c {
    u8 unknown_00[9];
    u8 state_09;    /* 0x09: bits 2-3 hold a 2-bit terrain sub-state */
    u8 unknown_0a[0x0b];
    u8 release_flags_15; /* 0x15: bit 0 tested before Func_08003f78 fires */
    u8 unknown_16[6];
    s32 multiplier_18; /* 0x18: second operand for both IWRAM multiplies */
    u8 unknown_1c;
    u8 entry_no_1c; /* 0x1c: passed to Func_08003f78/Func_08003f04 */
    u8 unknown_25;
    u8 busy_25; /* 0x25: set to 1 once Func_08003f78 releases entry_no_1c */
};

/* Small local records built on the stack and handed to Func_0800b168. */
struct PositionRecord_0800c62c {
    s32 dx; /* object.x - boundX */
    s32 y;
    s32 dz; /* object.z - boundZ */
    s32 ground;
};

struct MultiplyRecord_0800c62c {
    s32 result1;
    s32 result2;
};

extern u8 *Data_03001e70; /* big shared table; see comments below for the
                            * two offsets this row reads from it */
extern u8 *Data_03001e68; /* small state block; only its own +0 halfword
                            * (cleared) and +4 halfword (read) are touched
                            * here, not otherwise identified */
extern struct Object_0800c62c *Data_03001e64;

extern void *Func_080048b0(s32 kind, s32 size);
extern void Func_08002dd8(s32 kind);
extern s32 Func_08003f78(u32 entry_no);
extern s32 Func_0800b168(struct SubObject_0800c62c *sub,
                          struct PositionRecord_0800c62c *position,
                          struct MultiplyRecord_0800c62c *multiplied,
                          s32 heading);

/* IWRAM-relocated fixed-point multiply, opaque -- same helper this reconstruction
 * already treated as opaque for 0x0808b3ec. */
extern s32 Func_03000118(s32 value, s32 multiplier);

void Func_0800c62c(void)
{
    u8 *base = Data_03001e70;
    /* Reference point for the per-object bounding-box test below;
       Uncertainty: role of these two fields on the target of
       Data_03001e70 is not established beyond this use. */
    u32 boundZ = *(u32 *)(base + 0xe4) & 0xffff0000u;
    u32 boundX = *(u32 *)(base + 0xe8) & 0xffff0000u;
    struct Object_0800c62c *object;
    s32 count;
    void *dmaBuffer;
    u32 *dma;

    /* Claim (or reuse) a 0x2c4-byte scratch buffer under kind 52 and DMA
       a fixed ROM table into it. Uncertainty: nothing in this function
       reads the buffer back, so its consumer is elsewhere. */
    dmaBuffer = Func_080048b0(52, 0x2c4);
    dma = (u32 *)0x040000d4;
    dma[0] = 0x08009bb8;
    dma[1] = (u32)dmaBuffer;
    dma[2] = 0x840000b1;

    *(u16 *)Data_03001e68 = 0;

    object = Data_03001e64;
    count = 63;

    while (count >= 0) {
        u8 *statusByte = (u8 *)object + 0x54;
        u8 *busyByte = (u8 *)object + 0x5c;

        if (object->program_00 == 0) {
            goto next_object;
        }

        if (object->position_08[0] != 0 || object->position_08[2] != 0) {
            u8 status = *statusByte & 0xf;

            if (status != 1) {
                goto next_object;
            }

            if (*(s16 *)(Data_03001e68 + 4) != 0 && *busyByte == 0) {
                struct SubObject_0800c62c *sub = object->sub_50;

                Func_08003f78(sub->entry_no_1c);
                sub->busy_25 = status; /* == 1 */
                goto next_object;
            }

            {
                /* Main path: object has a real position and either the
                   Data_03001e68+4 gate was clear or the object is busy. */
                s32 x = object->position_08[0];
                s32 y = object->position_08[1];
                s32 z = object->position_08[2];
                s32 dz = z - boundZ;
                s32 dx = x - boundX;
                s32 boundSum = dz - y;
                struct SubObject_0800c62c *sub = object->sub_50;

                if ((u32)(0x1fffff + dx) > 0x12ffffe) {
                    goto bounds_fail;
                }
                if (boundSum <= (s32)0xffe00000) {
                    goto bounds_fail;
                }
                if (boundSum > (s32)0x00dfffff) {
                    goto bounds_fail;
                }

                {
                    /* Terrain-attribute lookup: tile index from the
                       object's own x/z (fixed-point, 20 fractional
                       bits), sub-table selected by the object's own
                       kind byte. */
                    u8 *terrainTable =
                        *(u8 **)(base + object->kind_22 * 48 + 304);
                    s32 tileIndex = (x >> 20) + ((z >> 20) << 7);
                    u32 *attrWord = (u32 *)(terrainTable + tileIndex * 4);
                    u32 word;
                    u32 bits1415;

                    if ((object->flags_23 & 1) != 0) {
                        word = *attrWord;
                        bits1415 = (word >> 14) & 3;

                        if (bits1415 != 0) {
                            sub->state_09 = (u8)((sub->state_09 & ~0xc) |
                                                  (bits1415 << 2));
                            /* Uncertainty: the same 2-bit value is folded
                               into a second byte at the sub-object's own
                               +0x15 offset here (not release_flags_15,
                               a different field at the same byte the asm
                               reuses -- transcribed as read/modify/write,
                               role not resolved). */
                            sub->release_flags_15 =
                                (u8)((sub->release_flags_15 & ~0xc) |
                                     (bits1415 << 2));
                        }
                    } else {
                        word = *attrWord;
                    }

                    {
                        /* Uncertainty: re-read unconditionally either
                           way, matching the asm exactly rather than
                           reusing the first load. */
                        u32 word2 = *attrWord;
                        u32 bits1213 = (word2 >> 12) & 3;

                        if (bits1213 != 0) {
                            object->kind_22 = (u8)(bits1213 + 255);
                        }
                    }
                    (void)word;
                }

                {
                    struct MultiplyRecord_0800c62c multiplied;
                    struct PositionRecord_0800c62c position;

                    multiplied.result1 = Func_03000118(
                        *(s32 *)((u8 *)object + 0x18), sub->multiplier_18);
                    multiplied.result2 = Func_03000118(
                        *(s32 *)((u8 *)object + 0x1c), sub->multiplier_18);

                    position.dx = dx;
                    position.y = y;
                    position.dz = dz;
                    position.ground = object->ground_14;

                    if ((object->flags_23 & 2) != 0) {
                        position.y += (s32)0xfec00000;
                        position.dz += (s32)0xfec00000;
                        position.ground += (s32)0xfec00000;
                    }
                    if ((object->flags_23 & 4) != 0) {
                        position.y += 0x01400000;
                        position.dz += 0x01400000;
                        position.ground += 0x01400000;
                    }

                    Func_0800b168(sub, &position, &multiplied,
                                  object->heading_06);
                }
                goto next_object;

            bounds_fail:
                if (*busyByte != 0) {
                    goto next_object;
                }
                if ((sub->release_flags_15 & 1) != 0) {
                    goto next_object;
                }
                Func_08003f78(sub->entry_no_1c);
                sub->busy_25 = 1;
                goto next_object;
            }
        } else {
            /* Degenerate position (x == 0 && z == 0). */
            u8 status = *statusByte & 0xf;
            struct SubObject_0800c62c *sub;

            if (status != 1) {
                goto next_object;
            }
            if (*busyByte != 0) {
                goto next_object;
            }
            sub = object->sub_50;
            if ((sub->release_flags_15 & 1) != 0) {
                goto next_object;
            }
            Func_08003f78(sub->entry_no_1c);
            sub->busy_25 = 1;
        }

    next_object:
        count--;
        object = (struct Object_0800c62c *)((u8 *)object + 0x70);
    }

    Func_08002dd8(52);
}
