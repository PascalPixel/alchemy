#include "RESOURCE.H"
/* DRAFT: ResourceObject_Create, score 200 (EN). Only the dead
 * "movs r2,#1; negs r2,r2" after the Register call is missing; TLA's twin
 * (0x08022d40) has the same dead -1 in r3, so the source had a -1 test.
 * 2026-10-04, measured: the -1 survives only when the test's arm empties
 * after reload (flow2 keeps the branch, jump2 deletes it, reload's -1
 * stays). Empty, dead-assignment, duplicate-return and goto arms vanish
 * earlier (jump1, cse2's jump, flow1 or ce). Score 0 is "if (Register(entry,
 * id) == -1) goto done; found->dirty = found->dirty; done:" (reload_cse
 * drops the self-store): dead code for an instruction count, which S2 does
 * not allow, so it is not used. The real arm is still unknown.
 * 2026-10-04 sun w2 finisher 3: an arm "return (struct ResourceObject *)-1"
 * does empty after reload (cse puts the call result, already -1 in r0, in
 * the return), but the other path's mov r0, r8 keeps cmp r0, r2; beq (32
 * with a different pool shift). An empty static inline call as the arm
 * (with and without an argument) and "return found" vanish early (32).
 * 2026-10-04 sun w3 finisher 6, alchemy diff 32 (all of it the two missing
 * instructions plus pool shifts). All of these vanish early (32): arms
 * "found = (struct AnimationObject *)found->part", "entry = found",
 * "found = found", "found->part[0] = found->part[0]" (struct self-copy),
 * a ternary "== -1 ? (struct ResourceObject *)found->part : found",
 * "return found->part" / "return found" duplicates, switch with case -1
 * (empty, or returning found, with and without default), "i = Register;
 * if (i == -1) return found", "!= -1" return, goto fail with a duplicate
 * return, and a second result pointer obj = found set before the NULL test
 * and reassigned in the arm. Real arms that store (flags = 1, flags |= 1,
 * slot = resource) keep their store (43-97). A scan of the whole built
 * tbs-en.elf finds no matched C function with a dead "movs #1; negs" before
 * its epilogue, so no house idiom to copy.
 * 2026-10-05 deep finisher 1, still 32. The -da dumps of the score-0
 * self-store show the mechanism exactly: local-alloc gives the reloaded
 * byte a REG_EQUIV to its own address, reload deletes the store back to
 * it, flow2 deletes the now dead load, the post-reload jump pass keeps the
 * branch over the emptied arm (its life notes are stale), and jump2 drops
 * branch and compare but not the split -1 (two insns by then). So the arm
 * has to be a read-and-write-back of the same byte after the call. Every
 * no-op spelling of that matches (0): "found->dirty |= 0", "&= 0xff",
 * "&= ~0x100" (a bit the u8 cannot hold), "= (u8)found->dirty", and
 * "found->count = found->count"; flags (just stored before the call) is
 * 29. None is used: each is code without effect, which S2 does not allow.
 * A value read before the call and restored in the arm keeps its store
 * (36: memory is not equivalent across the call), "found = entry" and
 * "entry = found; return entry" arms keep a move (33, 35), a -1 arm
 * returning entry is 34 and passing found to Register is 36. A duplicate
 * return is merged by the first jump pass already (the cross-jump that
 * would merge register-equal arms only runs in jump2).
 */
#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "ANIMSPR.H"
#include "VRAM_BLOCK.H"

extern struct AnimationObject *gSpriteObjects;

s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id);

/* Take the first free object of the 64, give it a resource entry and its
   tiles, and fill its OAM words for the metadata's width and height. */
struct ResourceObject *ResourceObject_Create(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationObject *entry;
    struct AnimationObject *found;
    u32 *word;
    s32 resource;
    s32 tile;
    s32 i;
    u32 shape;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    resource = Resource_FindFreeEntry();
    entry = gSpriteObjects;
    if (metadata->width == 0)
        return NULL;
    for (i = 0; i <= 63; i++, entry++) {
        if (entry->width == 0) {
            found = entry;
            break;
        }
    }
    if (found == NULL)
        return NULL;
    if (resource == 96)
        return NULL;
    tile = VramBlock_LoadCached(resource, 0, 0);
    if (tile == 0)
        return NULL;
    found->slot = resource;
    found->rotation = 0;
    found->flags = 1;
    switch ((u32)((metadata->width << 8) + metadata->height)) {
    case 0x0808: shape = 0; break;
    case 0x0810: shape = 0x8000; break;
    case 0x1008: shape = 0x4000; break;
    case 0x1010: shape = 0x40000000; break;
    case 0x1020: shape = 0x80008000; break;
    case 0x2010: shape = 0x80004000; break;
    case 0x2020: shape = 0x80000000; break;
    case 0x2040: shape = 0xc0008000; break;
    case 0x4020: shape = 0xc0004000; break;
    case 0x4040: shape = 0xc0000000; break;
    default: shape = 0; break;
    }
    word = (u32 *)found->part;
    *word++ = 0;
    *word++ = shape | 0x2000;
    *word++ = tile | 0x800;
    *word++ = 0;
    *word++ = 0x6000;
    *word = (gVramBlockCache[93].offset >> 5) | 0x800;
    ResourceMetadata_Register(entry, id);
    return (struct ResourceObject *)found;
}
