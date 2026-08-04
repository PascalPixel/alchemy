typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d owner 0x02002eb8..0x02002f17 (0x60 = 96 bytes).
 *
 * Reviewed saved-link setup helper. Every service call and constant comes
 * directly from the local disassembly; it returns through pop/bx at
 * 0x02002f12..0x02002f14 and carries no pool tail.
 */

extern void Func_02003208();
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a090();
extern void Func_0808a0d0();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a210();
extern void Func_0808a248();

void Func_02002eb8(void) {
    Func_0808a018();
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a0d0(0, 0x68, 0x98);
    Func_0808a1b8(0, 0x4000, 0x3C);
    Func_0808a1e0(0x11, 0);
    Func_0808a1e0(0x12, 0);
    Func_02003208();
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a248(1);
    Func_0808a020();
}
