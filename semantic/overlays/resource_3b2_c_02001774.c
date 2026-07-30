typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x02001774, 232 bytes: code 0x02001774-0x02001853 and
 * the two-word literal pool 0x02001854-0x0200185b (0x02000240, 0xfff00000).
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r5, r6, r7}` (r9/sl/fp) +
 * `push {r7}` (r8), an 8-byte incoming-argument spill frame, and the single
 * epilogue at 0x02001842 ending `pop {r0} / bx r0` — so the owner is **void**.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 1774 --json` (12 sites, 7 distinct targets, all import veneers):
 *   0x02001794 -> 0x02003088 -> Func_0808a080
 *   0x0200179c -> 0x02003088 -> Func_0808a080
 *   0x020017a2 -> 0x02003070 -> Func_0808a018
 *   0x020017dc -> 0x02003010 -> Func_08009150
 *   0x020017e4 -> 0x02002ff8 -> Func_08009080
 *   0x02001808 -> 0x02003010 -> Func_08009150
 *   0x0200181c -> 0x02002ff8 -> Func_08009080
 *   0x02001826 -> 0x02002ff8 -> Func_08009080
 *   0x0200182c -> 0x02003118 -> Func_080f9010
 *   0x02001832 -> 0x02003018 -> Func_08009158
 *   0x0200183a -> 0x02003118 -> Func_080f9010
 *   0x0200183e -> 0x02003078 -> Func_0808a020
 *
 * Behaviour: the overlay's teleport/transition step.  It fetches the party
 * entity (whose selector is the word the scene table holds at byte 500) and
 * the caller's own entity, then moves BOTH by the same (dx, dz) world delta,
 * snapping each result to the 16-tile grid (`& 0xfff00000`) and re-centring it
 * on the tile (`+ 0x00080000`, i.e. + 8.0 in 16.16).  Both entities get the
 * same fixed step rates at +48/+52.  The moved entity is then given facing 3 or
 * 4 depending on the sign of the delta, two sound cues are played around a
 * camera re-attach, and the common exit hook runs.
 *
 * `0x02000240` is the scene table the byte-exact siblings in assets/code
 * already use; the load here is a 32-bit read of the word at byte offset 500,
 * i.e. across `Data_02000240[250]`/`[251]`.
 *
 * UNCERTAINTY: Func_0808a018 at 0x020017a2 sets no argument register, so it
 * receives whatever r0 the preceding Func_0808a080 returned — the caller's
 * entity.  Other converted overlays spell this import with no arguments and
 * note the same residue; that spelling is kept here.  Func_0808a020 at the tail
 * likewise sets nothing.
 */

extern s16 Data_02000240[];             /* the overlay's scene table */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_0808a018();                   /* entity hook, r0 residue */
void Func_0808a020();                   /* common exit hook, no arguments */
void Func_08009150();                   /* place a record at (x, y, z) */
void Func_08009158();                   /* re-attach the camera to a record */
void Func_08009080();                   /* select presentation mode */
void Func_080f9010();                   /* play a sound cue */

#define TILE_MASK    ((s32)0xfff00000)  /* snap 16.16 to the 16-pixel grid */
#define TILE_CENTRE  0x00080000         /* + 8.0 in 16.16 */

struct MoveEntity {
    u8 unknown_00[8];
    s32 x;                              /* 0x08 */
    s32 y;                              /* 0x0c */
    s32 z;                              /* 0x10 */
    u8 unknown_14[0x1c];
    s32 stepX;                          /* 0x30 */
    s32 stepZ;                          /* 0x34 */
};

void Func_02001774(s32 selector, s32 deltaX, s32 deltaZ)
{
    struct MoveEntity *party;
    struct MoveEntity *mover;
    s32 dx;
    s32 dz;

    party = (struct MoveEntity *)
        Func_0808a080(*(s32 *)((u8 *)Data_02000240 + 500));
    mover = (struct MoveEntity *)Func_0808a080(selector);
    Func_0808a018();

    dx = deltaX << 16;
    dz = deltaZ << 16;

    party->stepX = 0x10000;
    party->stepZ = 0x8000;
    Func_08009150(party,
                  ((party->x + dx) & TILE_MASK) + TILE_CENTRE,
                  party->y,
                  ((party->z + dz) & TILE_MASK) + TILE_CENTRE);
    Func_08009080(party, 27);

    mover->stepX = 0x10000;
    mover->stepZ = 0x8000;
    Func_08009150(mover,
                  ((mover->x + dx) & TILE_MASK) + TILE_CENTRE,
                  mover->y,
                  ((mover->z + dz) & TILE_MASK) + TILE_CENTRE);

    if (deltaX < 0 || deltaZ < 0) {
        Func_08009080(mover, 4);
    } else {
        Func_08009080(mover, 3);
    }

    Func_080f9010(226);
    Func_08009158(party);
    Func_080f9010(288);
    Func_0808a020();
}
