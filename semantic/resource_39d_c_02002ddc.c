typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d owner 0x02002ddc..0x02002eb7 (0xdc = 220 bytes).
 *
 * Reviewed helper: updates the scene-0xb2 workspace slot, configures record 9,
 * applies the branch-independent record counters, and finishes the six-argument
 * draw call before the interworking return at 0x02002ea2.
 */

extern void Func_02003324();
extern void Func_080000c0();
extern void Func_08009128();
extern void Func_08009180();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern u8 *Func_0808a080();
extern void Func_0808a0f0();
extern void Func_0808a1e0();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern u8 *Func_0808a228();

void Func_02002ddc(void) {
    s32 temp_r5_2;
    void *temp_r5;

    temp_r5 = *(u8 **)0x03001e70 + 0x164;
    Func_0808a018();
    FIELD(temp_r5, s32 *, 0xC) = 0x03800000;
    Func_08009128();
    Func_080000c0(1);
    FIELD(Func_0808a080(9), s8 *, 0x55) = 0;
    Func_0808a0f0(9, 0x680000, 0x01080000);
    temp_r5_2 = 0xffe00000;
    FIELD(Func_0808a080(9), s32 *, 0xC) = temp_r5_2;
    FIELD(Func_0808a080(9), s32 *, 0x3C) = temp_r5_2;
    FIELD(Func_0808a228(), s8 *, 0x55) = 0;
    Func_0808a208(0x0000cccc, 0x00001999);
    Func_0808a210(0x800000, -1, 0xB80000, 1);
    Func_0808a218();
    Func_0808a010(0x1E);
    Func_08009180(0x1D, 0x4A, 4, 0x4A, 5, 4);
    Func_0808a1e0(0x11, 0);
    Func_0808a1e0(0x12, 0);
    Func_02003324();
    Func_0808a1e0(0x11, 1);
    Func_0808a1e0(0x12, 1);
    Func_0808a010(0x14);
    Func_080770c8(0x00000251);
    Func_0808a020();
}
