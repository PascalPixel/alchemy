/* NONMATCHING: reference 60 bytes, candidate 92; 42 differing halfwords.
 * Hypothesis 2: explicit row stores avoid the initializer's unresolved memset;
 * copies emit ldmia/stmia triples plus fourth-word str, with a 16-byte frame.
 * Full extent/pool differ; separate globals keep all three address literals.
 * 2026-09-29 slice 4: the reference's three stmia r0!, {r1, r2, r3, r4}
 * stores take the base in r0 and the four row words in r1 to r4, the
 * fixed-register shape of an asm block like the reviewed Dma_Set, not a
 * structure copy (GCC 2.96's Thumb block move uses three-register
 * ldmia/stmia pairs and a memset call for the initializer); 080c0eb8 has
 * the same three stores. It needs a reviewed store macro before it can
 * match. alchemy permute (3 jobs, 10 minutes) reached 2610 with 43 casts
 * and reorders; not kept.
 */
#include "TYPES.H"

struct TransformRow {
    s32 value[4];
};

struct TransformMatrix {
    struct TransformRow row[3];
};

void *Runtime_AllocateBlock(s32 kind, s32 size);
extern s32 gTransformStackDepth;
extern void *gTransformStackTop;
extern struct TransformMatrix gTransform;

void Render_ResetTransformState(void)
{
    struct TransformRow row;
    void *buf = Runtime_AllocateBlock(2, sizeof(gTransform));

    row.value[0] = 0x10000;
    row.value[1] = 0;
    row.value[2] = 0;
    row.value[3] = 0;
    gTransformStackDepth = 0;
    gTransformStackTop = buf;
    gTransform.row[0] = row;
    gTransform.row[1] = row;
    gTransform.row[2] = row;
}
