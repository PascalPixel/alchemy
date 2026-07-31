typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_389 owner at 0x0200115c, 192 bytes: the scene-script callback for
 * scene 99 - stop the trailing task, run one dialogue beat with actor 13, walk
 * it through three waypoints, and set event flag 0x869.
 *
 * Named before it was opened, twice over:
 *  - it appears as the middle word of the scene-script record
 *    0xffff0063 / 0x0200915d / 0xffffffff.  Under the proven 0x02008000 link
 *    base 0x0200915d is file offset 0x115c plus the Thumb bit, and the record's
 *    low halfword is 0x63 = 99.
 *  - the overlay's entry-0 root (semantic/overlays/resource_389_c_0200121c.c)
 *    branches on `Data_02000240[225] == 99`, so 99 is this overlay's scene id
 *    and this is the routine that runs for it.
 *
 * It also settles a third row for free: its first instruction removes the task
 * 0x020090c9, which is file offset 0x10c8 plus the Thumb bit - so 0x020010c8 is
 * a per-frame task, which is what the entry-0 root installs at 0x02001382.
 *
 * Complete owner: `push {lr}` at 0x0200115c through the single epilogue at
 * 0x0200120c, ending `pop {r0} / bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x0200115c-0x0200120f is reached as
 * instructions, 0x02001210-0x0200121b is not (0x020090c9, 0x0000132f,
 * 0x00000869).  180 code + 12 pool = 192, exactly the inventory row's figures.
 *
 * Calls: 18 sites against an advertised 17 - a beat, the safe direction.
 * Per-target multiset from `bun tools/overlay_call_targets.ts resource_389
 * 115c`:
 *   Func_0808a0d0  x3   0x11ca 0x11d6 0x11e4
 *   Func_0808a080  x2   0x11a6 0x11f4
 *   Func_080000d8  x1   0x1160
 *   Func_0808a018  x1   0x1164
 *   Func_0808a1e8  x1   0x1170
 *   Func_0808a138  x1   0x1178
 *   Func_0808a1b8  x1   0x1184
 *   Func_0808a170  x1   0x118a
 *   Func_0808a180  x1   0x1192
 *   Func_0808a110  x1   0x119a
 *   Func_0808a010  x1   0x11a0
 *   Func_0808a090  x1   0x11be
 *   Func_0808a0f0  x1   0x11ee
 *   Func_080770c8  x1   0x1204
 *   Func_0808a020  x1   0x1208
 *
 * The three Func_0808a0d0 waypoints are kept as three sites; the first two
 * share their second argument (600) and differ only in the third, which is
 * exactly the near-twin case the handover warns against normalising.
 *
 * 0x0000132f is a text id; its neighbours 0x132b-0x132e appear in the same
 * scene-script table, so the family is contiguous.
 *
 * The one clear behavioural bracket: the actor-10 presentation bit 2 is cleared
 * before the walk and set again after it.
 */

struct SceneActor {
    u8 unknown_00[0x23];
    u8 flags23;                     /* 0x23 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneActor *Func_0808a080();  /* scene actor by selector */
void Func_080000d8();                /* remove a task by entry */
void Func_0808a010();                /* wait this many frames */
void Func_0808a018();                /* scene bracket helper */
void Func_0808a020();                /* scene bracket close */
void Func_0808a090();                /* set an x/y scale pair, 16.16 */
void Func_0808a0d0();                /* move an actor to a waypoint */
void Func_0808a0f0();                /* place/aim the camera at a point */
void Func_0808a110();                /* one-argument actor call */
void Func_0808a138();                /* one-argument actor call */
void Func_0808a170();                /* show a text entry by id */
void Func_0808a180();                /* close the text entry */
void Func_0808a1b8();                /* three-argument presentation call */
void Func_0808a1e8();                /* three-argument presentation call */
void Func_080770c8();                /* set an event flag */

void Func_0200115c(void)
{
    struct SceneActor *actor;

    /* 0x020090c9 = Func_020010c8 + Thumb bit, this overlay's own task. */
    Func_080000d8((void *)0x020090c9);

    Func_0808a018();

    Func_0808a1e8(13, 256, 30);                 /* 128 << 1 */
    Func_0808a138(13, 2);
    Func_0808a1b8(0, 0x0000a000, 0);            /* 160 << 8 */
    Func_0808a170(0x132f);
    Func_0808a180(13, 0);
    Func_0808a110(13, 3);
    Func_0808a010(30);

    actor = Func_0808a080(10);
    actor->flags23 = (u8)(actor->flags23 & ~2);

    Func_0808a090(13, 0x00020000, 0x00010000);  /* 128 << 10, 128 << 9 */

    Func_0808a0d0(13, 600, 216);                /* 150 << 2 */
    Func_0808a0d0(13, 600, 248);
    Func_0808a0d0(13, 568, 296);                /* 142 << 2, 148 << 1 */

    Func_0808a0f0(13, 0, 0);

    actor = Func_0808a080(10);
    actor->flags23 = (u8)(actor->flags23 | 2);

    Func_080770c8(0x869);
    Func_0808a020();
}
