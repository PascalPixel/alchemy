typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02004bd8: pick the line of dialogue an NPC says, from
 * where the player stands, which way the player is moving, which NPC it is,
 * and how many times that NPC has been asked already.
 *
 * The owner is complete: `push {r5, r6, r7, lr} ; sub sp, #12` at 0x02004bd8
 * and the matching `add sp, #12 ; pop {r5, r6, r7} ; pop {r1} ; bx r1` at
 * 0x02004d56..0x02004d5c, followed by an alignment halfword and a 15-word
 * literal pool at 0x02004d5e-0x02004d9b.  The next row, the overlay's
 * import veneer table, begins at 0x02004d9c.  No live frame or register state
 * escapes the row.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive and r0 is set to 0 immediately
 * before the unwind, on every path, so the owner returns 0 - the same `s32`
 * callback shape as the byte-exact siblings 0x02000594 and 0x020005ac.
 *
 * The probe at 0x02004f20 is handed a 12-byte position one unit *below* the
 * caller's object (`y + 0xfff00000`), the same stack-position convention as
 * semantic/overlays/resource_3c8_c_02000374.c.  Its result is the record
 * standing there; the halfword its +0x28 pointer addresses must be 256, and
 * the byte at +0x24 of its sprite identifies the speaker.
 *
 * The candidate list is chosen by comparing |+0x24| against |+0x2c| - the
 * movement deltas this package's records carry, zeroed together by
 * 0x02000374 - and then by the sign of the dominant one divided by 0x10000.
 * `if (v < 0) v += 0xffff` followed by a sign test is the truncating
 * `v / 0x10000 < 0` idiom with the `asrs #16` optimised away, because only
 * the sign is used.
 *
 * Each candidate list is a NUL-terminated byte string scanned for the
 * speaker's id; running off the end takes the "nothing to say" exit.
 *
 * PROGRESS COUNTERS ARE WRITTEN BACK INTO THE OVERLAY IMAGE.  The second
 * scan walks `Data_0200f72c[slot]` / `Data_0200f78c[slot]`, byte cursors
 * that this owner advances with `str r3, [r0, r4]`.  At this overlay's proven
 * 0x02008000 link base those are file offsets 0x772c and 0x778c, i.e. the
 * module's own image - which is loaded into EWRAM and is therefore writable.
 * The number of cursor steps taken becomes the index into the line table
 * `Data_0200f77c[slot]` / `Data_0200f7ec[slot]`.
 *
 * The map-position table is selected by the scene id `Data_02000240[224]`,
 * the same selector the byte-exact sibling 0x02000e04 and this reconstruction's
 * 0x02002f8c read.  Scene 0xb9 uses a four-entry table, everything else an
 * eight-entry table; both hold `(x >> 20, z >> 20)` pairs.  When no entry
 * matches, the index runs one past the end (4 or 8) and that slot is used, so
 * the cursor and line tables have one more entry than the position table.
 * That is what the code does and it is preserved.
 *
 * All four `bl` sites resolve to the overlay-local position probe at
 * 0x0200032c followed by three calls to Func_08009098.
 */

struct Sprite_02004bd8 {
    u8 unknown_00[0x24];
    u8 speaker;                 /* 0x24 */
    u8 unknown_25[3];
    s16 *tag;                   /* 0x28 */
};

struct Actor_02004bd8 {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    u8 unknown_14[0x10];
    s32 deltaX;                 /* 0x24 */
    u8 unknown_28[4];
    s32 deltaZ;                 /* 0x2c */
    u8 unknown_30[0x20];
    struct Sprite_02004bd8 *sprite;  /* 0x50 */
};

struct Position_02004bd8 {
    s32 x;
    s32 y;
    s32 z;
};

extern s16 Data_02000240[];

/* Speaker-id candidate lists, NUL terminated. */
extern u8 Data_0200d1a4[];
extern u8 Data_0200d1a8[];
extern u8 Data_0200d1ac[];
extern u8 Data_0200d1b0[];

/* Fallback line, used by both "nothing to say" exits. */
extern u8 Data_0200d564[];

/* (x >> 20, z >> 20) position pairs. */
extern s32 Data_0200d128[];     /* four pairs, scene 0xb9 */
extern s32 Data_0200d164[];     /* eight pairs, every other scene */

/* Mutable byte cursors inside the overlay image, one per position slot. */
extern u8 *Data_0200f72c[];
extern u8 *Data_0200f78c[];

/* Line tables, one array of lines per position slot. */
extern s32 *Data_0200f77c[];
extern s32 *Data_0200f7ec[];

/* Used for its return value. */
struct Actor_02004bd8 *Func_0200032c();

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_08009098();

s32 Func_02004bd8(struct Actor_02004bd8 *object)
{
    struct Position_02004bd8 probe;
    struct Actor_02004bd8 *partner;
    struct Sprite_02004bd8 *sprite;
    u8 *candidates;
    u8 **cursors;
    s32 **lines;
    s32 *positions;
    s32 lastSlot;
    s32 slot;
    s32 step;
    s32 x;
    s32 deltaX;
    s32 deltaZ;
    s32 magnitudeX;
    s32 magnitudeZ;
    u8 speaker;

    probe.x = object->x;
    probe.y = object->y + 0xfff00000;
    probe.z = object->z;
    partner = Func_0200032c(&probe, 0);

    sprite = partner->sprite;
    if (*sprite->tag != 256) {
        goto tagFallback;
    }

    deltaX = object->deltaX;
    deltaZ = object->deltaZ;
    magnitudeX = deltaX >= 0 ? deltaX : -deltaX;
    magnitudeZ = deltaZ >= 0 ? deltaZ : -deltaZ;
    if (magnitudeX > magnitudeZ) {
        candidates = (deltaX / 0x10000 < 0) ? Data_0200d1a4 : Data_0200d1a8;
    } else {
        candidates = (deltaZ / 0x10000 < 0) ? Data_0200d1ac : Data_0200d1b0;
    }

    speaker = sprite->speaker;
    while (*candidates != 0 && *candidates != speaker) {
        candidates++;
    }
    if (*candidates == 0) {
        goto fallback;
    }

    goto afterFallback;

fallback:
    Func_08009098(object, Data_0200d564);
    return 0;

afterFallback:

    if (Data_02000240[224] == 0xb9) {
        positions = Data_0200d128;
        cursors = Data_0200f72c;
        lines = Data_0200f77c;
        lastSlot = 3;
    } else {
        positions = Data_0200d164;
        cursors = Data_0200f78c;
        lines = Data_0200f7ec;
        lastSlot = 7;
    }

    x = object->x;
    slot = 0;
    if ((x >> 20) != positions[0] || (object->z >> 20) != positions[1]) {
        for (;;) {
            slot++;
            if (slot > lastSlot) {
                break;
            }
            if ((x >> 20) == positions[slot * 2]
                && (object->z >> 20) == positions[slot * 2 + 1]) {
                break;
            }
        }
    }

    step = 0;
    for (;;) {
        u8 *cursor = cursors[slot];

        if (*cursor == 0) {
            goto fallback;
        }
        if (*cursor == partner->sprite->speaker) {
            break;
        }
        cursors[slot] = cursor + 1;
        step++;
    }

    Func_08009098(object, lines[slot][step]);
    return 0;

tagFallback:
    Func_08009098(object, Data_0200d564);
    return 0;
}
