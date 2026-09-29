/* Draft of the title overlay's code after its tables, resource_36f at
 * 0x02008054..0x02008538 (was MENU/TITLE/TITLE.C; the table getters link
 * from MENU/TITLE/TABLES.C).
 * Remaining differences, function by function:
 * - FunctionHead_02000054 and Title_Func020001c0: the ROM loads 0, 1, 4,
 *   0xb and 0x1c from their literal pools as link-time values.
 * - Title_RevealSpriteRow and Title_RevealScreen: they keep a counter
 *   (0x0200868c) and the sprite row (0x020086a0) past the loaded image, with
 *   0x3a unreferenced bytes between the image and the counter; their layout
 *   is not known well enough to define it.
 * - Title_LoadBackground: its body differs from 0x2e on, and the ROM loads
 *   its resource number 0x1a as a link-time value.
 * The listing keeps these rows. */
#include "TYPES.H"
extern struct MapRenderWork *gMapWork;
extern u8 gMapCellBuffer[];

#define FrameCounter (*(u32 *)&gFrameCount)
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            ime = &RegIme;                                           \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        RestoreInterrupts(saved);                                          \
    } while (0)
#define DMA3 ((volatile u32 *)0x040000d4)

#include "SCENE.H"
#include "DMA.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

struct VramBlock {
    u16 base;
    u16 offset;
};

struct Sprite {
    u32 words[3];
};

struct TitleWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

extern s16 gCell[];
extern s32 gIw;
extern s32 gIw2;
extern u8 Value_00000000;
extern u8 Value_00000001;
extern u8 Value_00000004;
extern u8 MsgSaveFailed;
extern u8 Data_0000001c[];
extern struct VramBlock Data_03001b10[];
extern s16 Data_02008650;
extern s16 Data_0200868c;
extern u32 Data_020086a0[];
extern volatile u16 RegIme;
extern u8 Value_0000001a[];
extern u16 Data_03001ad0[];

u8 *SceneData_Run(void *);
s32 Main_08000170();
s32 Main_080001d0();
s32 Main_08000290();
void Main_080001a8();
void Main_08000178();
void Main_080001e8(struct Sprite *sprite, s32 value);
void Title_LoadBackground(void);
void Title_RevealSpriteRow(void);

/* Signed halfwords in the shared work area; index 225 selects the mode. */

/* Queue a register write with interrupts masked.
 * FAKEMATCH: the final one-pass restore retains the queue-publication
 * boundary used by the exact world-map transfer family. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: BLEND_FADE.C keeps this address local to restoration. */
    do { RegIme = saved; } while (0);
}

/* FAKEMATCH: expand the destination first while the one-halfword record
 * retains the short-range pool-zero producer used by PALETTE_START.C. */
static __inline__ void ResetCounter(s16 *destination)
{
    struct { u16 value; } zero;

    zero.value = 0;
    *destination = zero.value;
}

static __inline__ void DecodeBackground(const u8 *res)
{
    Engine_ResourceDecodeType01(res, (void *)gMapCellBuffer);
}

s32 FunctionHead_02000054(void)
{
    s32 wait;
    s16 mode = gCell[225];

    if (mode == 10) {
        u8 *object = SceneData_Run(*(void **)&gCell[250]);

        object[85] = 0;
        SceneData_Do(75);
        SceneData_unk2_2(0);
        SceneData_unk3_2(120);
        /*
         * The wait is a guarded do-while, not a plain while: a plain while
         * leaves the test at the top and ends the loop with an unconditional
         * jump back rather than the conditional back-edge the reference has.
         */
        wait = 0;
        if (gIw == 0) {
            do {
                SceneData_unk4_2(1);
                if (++wait > 3599) {
                    break;
                }
            } while (gIw == 0);
        }
        SceneData_Apply((s32)(u32)&Value_00000000, 2);
        return 0;
    }
    if (mode == 9) {
        SceneData_unk5_2(67);
        SceneData_unk6(0);
        SceneData_unk7(17);
        SceneData_unk8(60);
        SceneData_unk8_3();
        SceneData_unk9(240);
        SceneData_unk10(19);
        SceneData_Apply2((s32)(u32)&Value_00000001, 2);
        return 0;
    }
    SceneData_unk11((s32)(u32)&MsgSaveFailed);
    if (gCell[225] == 2) {
        for (;;) {
            SceneData_unk12(19);
            SceneData_unk13(0);
            SceneData_unk14(0);
            if (SceneData_Check() <= 0) {
                goto stop;
            }
            SceneData_unk15(70);
            if (SceneData_unk2(1) != 0) {
                goto stop;
            }
            SceneData_unk16(17);
            SceneData_unk17(30);
            SceneData_unk9_3();
            wait = 0;
            if (gIw2 == 0) {
                do {
                    SceneData_unk18(1);
                    if (++wait > 119) {
                        break;
                    }
                } while (gIw2 == 0);
            }
        }
stop:
        SceneData_Apply3((s32)(u32)&Value_00000001, 1);
    } else {
        SceneData_unk19(64);
        SceneData_unk20(0);
        SceneData_unk10_3();
        SceneData_Apply4((s32)(u32)&Value_00000004, 16);
        SceneData_unk21(17);
    }
    SceneData_unk22(17);
    SceneData_unk23(30);
    SceneData_unk11_3();
    SceneData_unk24(60);
    SceneData_unk25(19);
    return 0;
}

void Title_Func020001c0(s32 mode)
{
    s32 buf;
    s16 *slot;

    buf = Value1(Main_08000170, 0x520);
    slot = (s16 *)0x2008650;
    if (*slot == -1)
        *slot = Main_080001d0();
    Main_080001a8(Main_08000290((s32)Data_0000001c), buf);
    Dma_Set((const void *)buf, (void *)0x050003e0, 0x84000008, (volatile u32 *)0x040000d4);
    Call3((void (*)())Engine_VramLoad, *slot, 0x500, buf + 32);
    {
        volatile u32 *dma = (volatile u32 *)0x040000d4;

        while (dma[2] & 0x80000000)
            ;
    }
    Main_08000178(buf);
}

/* Rebuild the row of eighteen title sprites; each frame reveals one more
 * every two frames, and the newest two blink with the frame counter. */
void Title_RevealSpriteRow(void)
{
    u32 *w;
    struct Sprite *p;
    s32 tile;
    s32 i;
    s32 n;
    s32 y;
    s32 x;

    p = (struct Sprite *)Data_020086a0;
    w = Data_020086a0;
    tile = Data_03001b10[Data_02008650].offset >> 5;
    i = 0;
    y = 0x88;
loop:
    {
        x = 232 - (18 - i) * 8;
        *w++ = 0;
        *w++ = (x << 16) | y | 0x8400;
        *w++ = 0xf000 | tile;
        n = Data_0200868c / 2 - i;
        if (n < 0)
            n = 0;
        if (n <= 2 && (FrameCounter & 1))
            n = 0;
        if (n != 0)
            Main_080001e8(p++, 255);
        tile += 2;
    }
    if (++i <= 17)
        goto loop;
    Data_0200868c++;
}

void Title_RevealScreen(void)
{
    s32 i;
    u8 *event;
    struct IoWriteQueue *q;

    Title_LoadBackground();
    Engine_EventWait(30);
    ResetCounter(&Data_0200868c);
    Title_Func020001c0(0);
    Engine_TaskAddCallback(Title_RevealSpriteRow, 0xc80);
    QUEUE_WRITE(0x4000000, 0x1540);
    QUEUE_WRITE(0x4000050, 0x2fce);
    QUEUE_WRITE(0x4000054, 16);
    QUEUE_WRITE(0x4000052, 0x1010);
    Engine_EventWait(120);
    for (i = 0; i <= 16; i++) {
        QUEUE_WRITE(0x4000054, 16 - i);
        Engine_TaskWait(3);
    }
    event = *(u8 **)&gEventWork;
    *(s32 *)(event + 0x1c0) = 0;
    *(s32 *)(event + 0x1c8) = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    *(s32 *)(*(u8 **)&gEventWork + 0x1c8) = 60;
}

/* Load the title background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
void Title_LoadBackground(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    s32 blank;

    id = (s32)Value_0000001a;
    Engine_BlendSetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    Data_03001ad0[5] = 0;
    blank = 0x1ff;
    res = Engine_ResourceGetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)gMapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    y = 0;
col:
    {
        x = 0;
    row:
        {
            s32 old = tile;

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = (struct ScrollPair *)Data_03001ad0;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct TitleWork **)&gMapWork)->mode = 0x1400;
}
