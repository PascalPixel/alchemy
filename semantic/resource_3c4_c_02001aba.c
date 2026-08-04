typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3c4 owner at 0x02001aba (370 bytes).
 *
 * The exported entry includes the leading `movs r0,r0` at 0x02001aba before
 * the saved-register prologue at 0x02001abc.  The matching high-register
 * unwind returns at 0x02001c1c, followed by alignment and the three referenced
 * pool words through 0x02001c2b.  The next independent owner starts at
 * 0x02001c2c.
 *
 * All 26 calls resolve through veneers: Func_0808a080 x16,
 * Func_0808a090 x2, Func_08009150 x2, Func_0808a0e8 x2,
 * Func_080f9010 x2, Func_080091c0 x1 and Func_0808a010 x1.  Actor fields use
 * the layout established by the adjacent owners: 12.20 x/y/z at +8/+12/+16
 * and a signed height-table index at +100.
 *
 * The two-stage negative-y test deliberately repeats the actor lookup.  The
 * first adjusted value decides whether more work is needed; the second is
 * shifted to a whole-unit threshold.  Keeping both reads preserves the four
 * lookup sites inside the loop rather than inventing a cached value.
 */

typedef struct Actor_02001aba {
    u8 pad0[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[80];
    s16 height_index;
} Actor_02001aba;

extern s32 Data_0200b350[];

void Func_08009150();
void Func_080091c0();
void Func_0808a010();
Actor_02001aba *Func_0808a080();
void Func_0808a090();
void Func_0808a0e8();
void Func_080f9010();

void Func_02001aba(s32 delay)
{
    Actor_02001aba *actor;
    Actor_02001aba *target;
    s32 x;
    s32 z;
    s32 height;
    s32 index;

    Func_0808a090(10, 0x8000, 0x4000);
    Func_0808a090(11, 0x8000, 0x4000);

    if (delay != 0) {
        Func_080f9010(180);
    }

    target = Func_0808a080(10);
    x = Func_0808a080(10)->x;
    index = Func_0808a080(10)->height_index;
    z = Func_0808a080(10)->z;
    Func_08009150(target, x, Data_0200b350[index], z);

    target = Func_0808a080(11);
    x = Func_0808a080(11)->x;
    index = Func_0808a080(11)->height_index;
    z = Func_0808a080(11)->z;
    Func_08009150(target, x, Data_0200b350[index], z);

    Func_0808a0e8(10);
    Func_0808a0e8(11);

    actor = Func_0808a080(10);
    index = Func_0808a080(10)->height_index;
    actor->y = Data_0200b350[index];

    actor = Func_0808a080(11);
    index = Func_0808a080(11)->height_index;
    actor->y = Data_0200b350[index];

    if (delay != 0) {
        Func_080f9010(0x121);
    }

    for (index = 0; index <= 4; index++) {
        s32 slot = index + 10;

        height = Func_0808a080(slot)->y;
        if (height < 0) {
            height += 0xffff;
        }

        if (height < 0) {
            height = Func_0808a080(slot)->y;
            if (height < 0) {
                height += 0xffff;
            }

            if ((height >> 16) > -30) {
                x = Func_0808a080(slot)->x >> 20;
                z = Func_0808a080(slot)->z >> 20;
                Func_080091c0(4, 9, 1, 1, x, z);
            }
        }
    }

    Func_0808a010(delay);
}
