typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

extern void Func_080770c8();
extern void Func_02001c78();
extern u8 *Func_0808a080();
extern s32 Func_02000e00();
extern void Func_0808a090();
extern void Func_08009150();
extern void Func_080f9010();
extern void Func_02000324();
extern void Func_0200013c();
extern void Func_0808a1e8();
extern void Func_0808a100();
extern void Func_080000c0();
extern void Func_0808a010();

/* Move actor 0 to the nearest scene anchor and stage its arrival effect. */
void Func_02000e2c(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s16 scene = *(s16 *)(0x02000240 + 448);
    s16 countdown = 60;
    const s32 *anchors;
    s32 anchor_count;
    s32 nearest = 0;
    s32 best_distance = 0x0f000000;
    s32 index;
    u8 *actor;
    s32 descriptor[10] = { 0 };

    Func_080770c8(0x200);
    Func_02001c78(1);

    if (scene == 89) {
        anchors = (const s32 *)0x02009f30;
        anchor_count = 3;
    } else if (scene == 90) {
        anchors = (const s32 *)0x02009f48;
        anchor_count = 5;
    } else {
        anchors = (const s32 *)0x02009f70;
        anchor_count = 2;
    }

    for (index = 0; index < anchor_count; index++) {
        s32 distance = Func_02000e00(Func_0808a080(0) + 8, anchors + index * 2);
        if (distance <= best_distance) {
            best_distance = distance;
            nearest = index;
        }
    }

    Func_0808a090(0, 0x20000, 0x10000);
    actor = Func_0808a080(0);
    Func_08009150(actor, anchors[nearest * 2], anchors[nearest * 2 + 1], 0);
    actor = Func_0808a080(0);
    *(s32 *)(actor + 40) = 0x600000;
    Func_080f9010(152);
    actor = Func_0808a080(0);
    Func_02000324(actor, *(s32 *)(Func_0808a080(0) + 12));
    Func_080f9010(241);

    actor = Func_0808a080(0);
    descriptor[2] = 0x10000;
    descriptor[3] = 0x0000cccc;
    descriptor[4] = 0x40000;
    descriptor[5] = 0x00013333;
    descriptor[6] = 214;
    Func_0200013c(*(s32 *)(actor + 8), *(s32 *)(actor + 12),
                  *(s32 *)(actor + 16), 0, 0, 0x700000, descriptor);
    Func_0808a1e8(0, 0x104, 0);
    Func_0808a100(0, 18);

    *(s16 *)(workspace + 0xcba) = 600;
    do {
        countdown--;
        if (*(s16 *)0x02000472 > 0) {
            *(s16 *)0x02000472 -= 5;
            if (*(s16 *)0x02000472 <= 0)
                *(s16 *)0x02000472 = 0;
            else if (countdown == 0)
                countdown = 1;
        }
        Func_080000c0(1);
    } while (countdown != 0);

    actor = Func_0808a080(0);
    descriptor[2] = 0x10000;
    descriptor[3] = 0x0000cccc;
    descriptor[4] = 0x10000;
    descriptor[5] = 0x00013333;
    descriptor[6] = 214;
    Func_0200013c(*(s32 *)(actor + 8), *(s32 *)(actor + 12),
                  *(s32 *)(actor + 16), 0, 0, 0x700000, descriptor);
    Func_080f9010(288);
    Func_080f9010(152);
    actor = Func_0808a080(0);
    *(s32 *)(actor + 40) = 0x600000;
    Func_0808a100(0, 1);
    Func_0808a010(10);
    *(s16 *)(workspace + 0xcba) = countdown;
    Func_02001c78(0);
}
