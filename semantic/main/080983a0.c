typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

extern void Func_08098294(s32);
extern void Func_0808fe38(s32);
extern void *Func_0808ba1c(u32);
extern void Func_08091220(s32, s32);
extern void Func_08091200(s32, s32);
extern void Func_08091254(s32);
extern void Func_080030f8(u32);
extern s32 Func_0808e4b4(s32, u32, s32 *);
extern void Func_08096b28(s32, u32, s32);
extern void Func_080f9010(s32);
extern void Func_0808f32c(void);
extern void Func_080041d8(void *, s32);
extern s16 Data_02000240[];

void Func_080983a0(void)
{
    u8 **slots = (u8 **)0x03001ebc;
    u8 *global = slots[0];
    u8 *scene;
    u8 *record;
    u32 *resource;
    s32 scratch;
    s32 choice;
    s32 value;
    s32 one;
    s32 frame;
    s32 index;
    s32 duration;

    Func_08098294(6);
    Func_0808fe38(8);
    index = 250;
    resource = (u32 *)&Data_02000240[index];
    scene = slots[4];
    record = Func_0808ba1c(*resource);
    *(s32 *)(scene + 0x52c) = *(s32 *)(record + 8);
    *(s32 *)(scene + 0x530) = *(s32 *)(record + 16) - *(s32 *)(record + 12);

    Func_08091220(0x10000, 0);
    Func_08091200(0x10001, 1);
    Func_08091254(1);
    Func_080030f8(1);
    choice = Func_0808e4b4(0x50000005, 8, &scratch);
    if (choice != 0)
        Func_08096b28(choice, *resource, scratch);
    Func_080f9010(131);

    one = 1;
    *(s16 *)(global + 0xcb8) = one;
    value = *(s32 *)(scene + 0x52c);
    if (value < 0)
        value += 0xffff;
    *(s16 *)(global + 0xcbc) = value >> 16;
    value = *(s32 *)(scene + 0x530);
    if (value < 0)
        value += 0xffff;
    *(s16 *)(global + 0xcbe) = value >> 16;
    duration = 150;
    duration <<= 2;
    *(s16 *)(global + 0xcba) = duration;
    *(s16 *)(global + 0xcc0) = one;
    Func_0808f32c();

    frame = 0;
    do {
        Func_080030f8(1);
        *(s16 *)(scene + 0x52a) = frame;
        frame++;
    } while (frame <= 18);
    Func_080041d8((void *)0x080982dd, 3200);
    return;
}
