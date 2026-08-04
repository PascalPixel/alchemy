typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Run the following encounter beat, finishing with actors 12 and 21 selected.
 * The owner begins at 0x02002db4, returns at 0x02002f68, and owns its six-word
 * literal pool through 0x02002f83 before the next prologue at 0x02002f84.
 */

extern u8 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_020045cc(void);
extern void Func_020045f4(u32 actor, u32 frames);
extern void Func_02004624(u32 actor, u32 target, u32 frames);
extern void Func_0200463c(u32 actor, u32 mode, u32 frames);
extern void Func_080000c0(u32 value);
extern void Func_08009128(void);
extern void Func_0808a010(u32 frames);
extern void Func_0808a0f0(u32 actor, u32 x, u32 z);
extern void Func_0808a100(u32 actor, u32 mode);
extern void Func_0808a138(u32 actor, u32 mode);
extern void Func_0808a148(u32 actor, u32 target, u32 mode);
extern void Func_0808a170(u32 message);
extern void Func_0808a180(u32 actor, u32 mode);
extern void Func_0808a1b8(u32 actor, u32 speed, u32 mode);
extern void Func_0808a200(u32 actor, u32 mode);
extern void Func_0808a218(void);
extern void Func_0808a250(u32 actor, u32 value);
extern void Func_0808a260(u32 actor, u32 value);
extern void Func_0808a268(u32 actor, u32 value);

void Func_02002db4(void)
{
    u8 *shared;

    Func_0808a0f0(10, 198u << 18, 208u << 17);
    Func_0808a0f0(11, 200u << 18, 200u << 17);
    Func_0808a0f0(12, 194u << 18, 208u << 17);
    Func_0808a1b8(10, 192u << 6, 0);
    Func_0808a1b8(11, 192u << 6, 0);
    Func_0808a1b8(12, 192u << 6, 0);
    Func_0808a0f0(0, 198u << 18, 220u << 17);
    Func_0808a0f0(1, 202u << 18, 216u << 17);
    Func_0808a0f0(2, 198u << 18, 228u << 17);
    Func_0808a1b8(0, 192u << 8, 0);
    Func_0808a1b8(1, 176u << 8, 0);
    Func_0808a1b8(2, 176u << 8, 0);
    Func_0808a148(8, 10, 0);

    shared = Data_03001ebc;
    *(u32 *)(shared + 0x1c0) = 0x209;
    Func_0808a200(0, 0);
    Func_0808a218();
    Func_08009128();
    Func_080000c0(1);
    *(u32 *)(shared + 0x1c8) = 32;
    Func_020045cc();
    Func_0808a010(60);
    Func_0808a170(0x12e1);
    Func_0808a138(11, 1);
    Func_0808a010(20);
    Func_020045f4(11, 30);
    Func_0808a138(12, 1);
    Func_0808a010(20);
    Func_0808a180(12, 0);
    Func_02004624(10, 11, 30);
    Func_0808a100(10, 3);
    Func_0200463c(11, 3, 30);
    Func_02004624(10, 12, 30);
    Func_0808a100(10, 3);
    Func_0200463c(12, 3, 40);
    Func_0808a1b8(10, 192u << 6, 0);
    Func_0808a1b8(11, 192u << 6, 0);
    Func_0808a1b8(12, 192u << 6, 0);
    Func_0808a010(20);
    Func_0200463c(10, 4, 20);
    Func_0808a180(10, 0);
    *(u32 *)(shared + 0x1c0) = 0x200;
    Func_0808a260(0x15, 17);
    Func_0808a268(0x15, 16);
    Data_02000240[0x22b] = 3;
    Func_0808a250(12, 5);
}
