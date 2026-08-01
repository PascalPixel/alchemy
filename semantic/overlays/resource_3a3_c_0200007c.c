typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3a3 scene-table selector at 0x0200007c (160 bytes,
 * 0x0200007c .. 0x0200011b, of which 0x020000f4 .. 0x0200011b is the pool).
 *
 * Complete owner: `push {r5, lr}` at 0x0200007c and the interworking return
 * `pop {r5} / pop {r1} / bx r1` at 0x020000ee.  The popped register is r1, so
 * r0 survives and IS the result -- the owner returns the selected table
 * pointer.  This matches its byte-exact siblings
 * `assets/code/resource_3a3_c_02000030.c` and `_0200011c.c`, which have the
 * same `Data_02000240[224]` three-way selection and the same s32 return.
 *
 * All 5 call sites are placed (row reports calls=5): Func_080770c0 x4,
 * Func_0808a038 x1.
 *
 * The three returned pool words 0x0200940c / 0x020095bc / 0x020093f4 are even
 * and lie in the 0x0200_9xxx band, so under the proven 0x02008000 link base
 * they are in-image DATA at file offsets 0x140c / 0x15bc / 0x13f4 -- the same
 * shape as the siblings' Data_02009120 / Data_02009288 / Data_020090f0.  They
 * are declared as data arrays, not RAM globals, and this owner patches bytes
 * inside two of them before returning, so the tables are not const (the
 * overlay image is writable EWRAM).
 *
 * SHARED STORE SITE.  The `*(sel + 94) = 1` store at 0x020000d8 is reached
 * from both the 0x8fe and 0x907 true edges; it is spelled once behind a
 * short-circuit `||`.
 *
 * The compared constants 0x4b/0x4c appear in the byte-exact siblings as
 * `(s32)&Value_0000004b` / `(s32)&Value_0000004c`, which is a literal-pooling
 * device for that reconstruction; the integers are written directly here.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_0808a038();

extern s16 Data_02000240[];
extern u8 Data_0200940c[];
extern u8 Data_020095bc[];
extern u8 Data_020093f4[];

s32 Func_0200007c(void)
{
    s16 scene;

    scene = Data_02000240[224];

    if (scene == 0x4b) {
        if (Func_080770c0(0x909) != 0) {
            Data_0200940c[142] = 0;
            Data_0200940c[166] = 0;
        }
        return (s32)Data_0200940c;
    }

    if (scene == 0x4c) {
        if (Func_080770c0(0x8fd) != 0) {
            Data_020095bc[46] = 1;
        }
        if (Func_080770c0(0x8fe) != 0 || Func_080770c0(0x907) != 0) {
            Data_020095bc[94] = 1;
        }
        Func_0808a038(Data_020095bc);
        return (s32)Data_020095bc;
    }

    return (s32)Data_020093f4;
}
