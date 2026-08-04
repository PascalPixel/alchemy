#include "types.h"

/*
 * Resource 3b8 cutscene at 0x02000ff8.
 *
 * Complete owner: `push {lr}` at 0x02000ff8 and the matching `pop {r0} / bx r0`
 * interworking return at 0x020017ca.  The body is one straight-line call
 * carpet with no conditional branches; the only branch, at 0x020013f4, hops the
 * mid-function literal pool at 0x020013f6-0x02001407.  A second pool at
 * 0x020017ce-0x020017e7 follows the return.  Both are data.
 *
 * No register value survives any call here: every argument is materialised
 * immediately before its `bl`, and no result is consumed, so the owner is a
 * pure sequence of 250 import calls.
 *
 * As elsewhere in this overlay, every `bl` reaches the import band above the
 * code and each import is named by the address its call site computes, the
 * convention the byte-exact sources in `assets/code/resource_3b8_c_*.c`
 * already use; the numeric name carries no further meaning.  Old-style
 * declarations are required because several of these addresses are reached
 * with different argument counts from this one owner (0x02005452, 0x020054aa,
 * 0x020056f2, 0x02005718, 0x0200579c, 0x020058a8, 0x02005958, 0x02005972,
 * 0x0200598e, 0x020059c4, 0x02005a22, 0x02005aa0, 0x02005b28, 0x02005b62 and
 * 0x02005c1c).
 *
 * Shifted immediates are written as their computed values (for example
 * `movs r1, #130 / lsls r1, #2` is spelled 520).  Pool words 0x00014ccc /
 * 0x0000a666 and 0x0000cccc / 0x00006666 are passed as plain constants; they
 * are fixed-point pairs, not addresses.
 */

void Func_080000c0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a218();
void Func_0808a460();
void Func_0808a578();
void Func_0808a580();

void Func_02000ff8(void)
{
    Func_080770c8(0x96b);
    Func_0808a018();
    Func_0808a460();
    Func_0808a170(0x2021);
    Func_0808a0d0(0, 520, 424);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a210(0x2300000, -1, 0x1700000, 1);
    Func_0808a218();
    Func_0808a090(20, 0x10000, 0x8000);
    Func_0808a580(20, 40, 0);
    Func_0808a0d0(20, 584, 360);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x101, 40);
    Func_0808a1b8(20, 0x8000, 0);
    Func_0808a090(21, 0x20000, 0x10000);
    Func_0808a090(22, 0x20000, 0x10000);
    Func_0808a0c8(21, 528, 352);
    Func_0808a0d0(22, 528, 368);
    Func_080000c0(3);
    Func_0808a100(21, 1);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a110(22, 3);
    Func_0808a010(30);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a580(20, -16, 0);
    Func_0808a010(10);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a100(21, 4);
    Func_0808a110(22, 4);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x105, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x102, 40);
    Func_0808a090(21, 0x10000, 0x8000);
    Func_0808a580(21, 8, 0);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x101, 80);
    Func_0808a010(10);
    Func_0808a1e8(22, 0x102, 40);
    Func_0808a090(22, 0x10000, 0x8000);
    Func_0808a580(22, 8, 0);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a100(21, 3);
    Func_0808a110(22, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(22, 4);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x105, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x101, 40);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x102, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a138(22, 2);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a110(20, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x102, 40);
    Func_0808a180(20, 0);
    Func_0808a010(20);
    Func_0808a150(21, 22, 60);
    Func_0808a148(21, 20, 0);
    Func_0808a148(22, 20, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x100, 40);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(22, 3);
    Func_0808a010(30);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a1f0(20, 0x102);
    Func_0808a138(20, 2);
    Func_0808a010(30);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x101, 40);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(20, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1e8(22, 0x101, 40);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(20);
    Func_0808a150(21, 22, 60);
    Func_0808a148(21, 20, 0);
    Func_0808a148(22, 20, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x105, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(22, 3);
    Func_0808a010(30);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a110(20, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a138(22, 2);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x102, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(22, 0x100, 40);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a1e8(20, 0x102, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x101, 40);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(20);
    Func_0808a150(21, 22, 60);
    Func_0808a148(21, 20, 0);
    Func_0808a148(22, 20, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1f0(21, 0x102);
    Func_0808a1f0(22, 0x102);
    Func_0808a130(21, 2);
    Func_0808a138(22, 2);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1b8(20, 0, 0);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a110(22, 4);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a1b8(20, 0x8000, 0);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a110(21, 3);
    Func_0808a010(30);
    Func_0808a180(21, 0);
    Func_0808a010(20);
    Func_0808a1e8(20, 0x105, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a110(22, 3);
    Func_0808a010(30);
    Func_0808a180(22, 0);
    Func_0808a010(10);
    Func_0808a110(20, 3);
    Func_0808a010(40);
    Func_0808a150(21, 22, 60);
    Func_0808a110(21, 3);
    Func_0808a010(20);
    Func_0808a110(22, 3);
    Func_0808a010(20);
    Func_0808a1b8(22, 0x4000, 0);
    Func_0808a010(40);
    Func_0808a090(21, 0x14ccc, 0xa666);
    Func_0808a090(22, 0x14ccc, 0xa666);
    Func_0808a578(21, 0, 120);
    Func_0808a580(22, 0, 120);
    Func_0808a0f0(21, 0, 0);
    Func_0808a0f0(22, 0, 0);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(40);
    Func_0808a090(20, 0xcccc, 0x6666);
    Func_0808a580(20, -16, 0);
    Func_0808a010(30);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(30);
    Func_0808a1b8(20, 0, 0);
    Func_0808a010(30);
    Func_0808a090(20, 0x10000, 0x8000);
    Func_0808a580(20, 120, 0);
    Func_0808a580(20, 60, 0);
    Func_0808a0f0(20, 0, 0);
    Func_0808a020();
}
