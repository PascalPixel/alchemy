typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02001150: cutscene step that runs the 0x020005e4 emitter
 * callback and picks one of two anchors depending on how high the object is.
 *
 * The owner is complete: `push {r5, lr}` at 0x02001150 and the matching
 * `pop {r5} ; pop {r0} ; bx r0` at 0x0200120a..0x0200120e, followed by a
 * two-word literal pool at 0x02001210-0x02001217.  No live frame or register
 * state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * `object->callback = 0x020085e5` installs Func_020005e4: at this overlay's
 * proven 0x02008000 link base, 0x020085e5 - 0x8000 - 1 = 0x020005e4.  This is
 * a third independent witness for that base (the byte-exact siblings give
 * 0x020086a1 -> 0x020006a0, 0x0200896d -> 0x0200096c and 0x02008b99 ->
 * 0x02000b98).  The slot is cleared again at 0x02001204, the same
 * install/clear pair the byte-exact sibling 0x02000754 uses.
 *
 * 0x0200d21c is an in-image data address (file offset 0x521c under the same
 * base); the raw pool value is kept as the symbol name, matching the
 * byte-exact sources' `Data_0200dd68`-style spelling.
 *
 * The height test reads offset 0x10 - the z coordinate of the byte-exact
 * siblings' scene record - and compares `z >> 20` against 54 with a signed
 * `bgt`.  Both arms clear bit 0 of the byte at +0x5a of a looked-up record
 * and then select a scale, 210 or 238, which is multiplied by 4 before use.
 *
 * Seventeen distinct `bl` targets, seventeen call sites, all accounted for:
 * 0x02006002, 0x02005ff0, 0x02006020, 0x0200602e, 0x020060a6, 0x02006036,
 * 0x0200603c, 0x02006050, 0x02006098, 0x0200604e, 0x02006074, 0x02006064,
 * 0x02006154, 0x020060fc, 0x0200612c, 0x0200610c, 0x020060a4.  Import naming
 * follows the note in resource_3c8_c_020002f0.c.
 */

struct Object_02001150 {
    u8 unknown_00[10];
    s16 unkA;                   /* 0x0a */
    u8 unknown_0c[4];
    s32 z;                      /* 0x10 */
    u8 unknown_14[0x14];
    s32 unk28;                  /* 0x28 */
    u8 unknown_2c[0x40];
    s32 callback;               /* 0x6c */
};

/* A record whose byte at +0x5a carries a one-bit flag. */
struct Record_02001150 {
    u8 unknown_00[0x5a];
    u8 flag5a;
};

extern u8 Data_0200d21c[];

/* Used for their return values. */
struct Object_02001150 *Func_02006002();
struct Record_02001150 *Func_0200603c();
struct Record_02001150 *Func_02006050();
struct Record_02001150 *Func_02006074();

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02005ff0();
void Func_02006020();
void Func_0200602e();
void Func_020060a6();
void Func_02006036();
void Func_02006098();
void Func_0200604e();
void Func_02006064();
void Func_02006154();
void Func_020060fc();
void Func_0200612c();
void Func_0200610c();
void Func_020060a4();

void Func_02001150(void)
{
    struct Object_02001150 *object;
    struct Record_02001150 *record;
    s32 scale;

    object = Func_02006002(0);

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written.  Whether
     * the callee reads it is unverified. */
    Func_02005ff0(object);

    Func_02006020(0, Data_0200d21c);
    Func_0200602e(0);
    Func_020060a6(0, 6);

    object->unk28 = 0x00040000;
    Func_02006036(0, 0x00040000, 0x00020000);

    if ((object->z >> 20) > 54) {
        record = Func_02006050(0);
        record->flag5a &= 0xfe;
        scale = 238;
    } else {
        record = Func_0200603c(0);
        record->flag5a &= 0xfe;
        scale = 210;
    }

    Func_02006098(0, object->unkA, scale << 2);
    Func_0200604e(1);

    record = Func_02006074(0);
    record->flag5a |= 1;

    Func_02006064(20);

    object->callback = 0x020085e5;      /* = Func_020005e4 | 1 */
    /* r3 still holds that same word at the branch below; it is treated as
     * the leftover of the store, not as a fourth argument. */
    Func_02006154(0, 258, 60);

    Func_020060fc(0, 4);
    Func_0200612c(0, 0);
    Func_0200610c(0, 4);

    object->callback = 0;
    Func_020060a4();
}
