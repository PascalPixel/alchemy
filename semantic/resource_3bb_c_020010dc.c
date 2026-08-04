typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

/*
 * resource_3bb owner at 0x020010dc, 532 bytes: configure the five-member
 * handler family, place its members around the current object's signed
 * position pair, initialize the family's shared state, then copy the
 * positions of three optional members into their matching slots.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl spill at
 * 0x020010dc through `bx r0` at 0x020012d6. Its five-word literal pool
 * occupies 0x020012d8-0x020012ef; the next owner's prologue is at
 * 0x020012f0. One argument (r0), void.
 *
 * Not found by the structural inventory walk: reached by one `bl` from
 * resource_3bb_c_02000bd4.c. The calls below were checked against all 47
 * ROM call sites. Names describe addresses only; the handler records and
 * the meaning of their signed halfwords remain deliberately unnamed.
 */

extern s32 Func_0808a010();
extern s32 Func_0808a018();
extern s32 Func_0808a020();
extern s32 Func_0808a080();
extern s32 Func_0808a090();
extern s32 Func_0808a0b8();
extern s32 Func_0808a0d0();
extern s32 Func_0808a0f0();
extern s32 Func_0808a100();
extern s32 Func_0808a110();
extern s32 Func_0808a138();
extern s32 Func_0808a170();
extern s32 Func_0808a180();
extern s32 Func_0808a1b8();
extern s32 Func_0808a200();
extern s32 Func_0808a360();
extern s32 Func_0808a370();

void Func_020010dc(s32 self)
{
    u8 *object = (u8 *)Func_0808a080(self);
    s32 x = *(short *)(object + 10);
    s32 y = *(short *)(object + 18);
    u8 *member;

    Func_0808a018();

    Func_0808a090(self, 0x10000, 0x8000);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);

    Func_0808a0f0(0, x << 16, (y << 16) - 0x300000);
    Func_0808a0f0(1, (x << 16) - 0x280000, (y << 16) - 0x100000);
    Func_0808a0f0(2, (x << 16) + 0x100000, (y << 16) - 0x100000);
    Func_0808a0f0(3, x << 16, (y << 16) - 0x200000);
    Func_0808a0f0(self, x << 16, (y << 16) - 0x500000);

    member = (u8 *)Func_0808a080(0);
    *(u16 *)(member + 6) = 0xc000;
    Func_0808a200(0, 0);
    Func_0808a360();
    Func_0808a370();

    Func_0808a170(0x20e9);
    Func_0808a110(self, 3);
    Func_0808a180(self, 0);
    Func_0808a138(self, 2);
    Func_0808a180(self, 0);
    Func_0808a138(self, 2);
    Func_0808a180(self, 0);
    Func_0808a138(self, 2);
    Func_0808a180(self, 0);

    Func_0808a100(3, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(0, 3);
    Func_0808a010(6);
    Func_0808a100(1, 2);

    member = (u8 *)Func_0808a080(0);
    if (member != 0) {
        Func_0808a0b8(1, *(short *)(member + 10), *(short *)(member + 18));
    }

    Func_0808a100(2, 2);
    member = (u8 *)Func_0808a080(0);
    if (member != 0) {
        Func_0808a0b8(2, *(short *)(member + 10), *(short *)(member + 18));
    }

    Func_0808a100(3, 2);
    member = (u8 *)Func_0808a080(0);
    if (member != 0) {
        Func_0808a0b8(3, *(short *)(member + 10), *(short *)(member + 18));
    }

    Func_0808a0d0(self, x - 16, y - 64);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0f0(3, 0, 0);
    Func_0808a0d0(self, x - 16, y - 16);
    Func_0808a0d0(self, x, y);
    Func_0808a1b8(self, x << 16, 10);
    Func_0808a020();
}
