/* Draft, not exact (2026-09-24): 320 of 324 bytes, 78 halfwords differ. The shape, pool and
   loops match; residuals: the eight-record loop keeps its table pointer where the reference
   keeps a separate up-counter (4 bytes), the mirrored cosine store adds the state base instead
   of using a register-offset strh, and the zero stores pick r1 where the reference picks r2. */
#include "TYPES.H"

/* 28-byte animation record, as in the 0x02010000 tables and work + 0x7080. */
struct Particle {
    s32 x;
    s32 y;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 field_18;
};

extern u8 *Data_03001f04;
extern u8 *Data_03001eec;
extern u8 Data_080f8736[];

u32 Trig_Cos(s32 angle);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

void Unnamed_080f731c(void)
{
    u8 *state;
    u8 *work;
    struct Particle *rec;
    s32 offset;
    s32 i;
    s32 y;
    s32 angle;
    s32 window;
    u8 *table;

    state = Data_03001f04;
    work = *(&Data_03001f04 - 6);  /* FAKEMATCH: addressed from the neighbouring cell */
    offset = 0;
    rec = (struct Particle *)0x02010018;
    for (i = 0; i != 0x800; i++) {
        rec->x = 0;
        rec++;
    }

    rec = (struct Particle *)(work + 0x7080);
    table = Data_080f8736;
    y = (s32)0xffe00000;
    for (i = 0; i != 8; i++) {
        rec->x = (offset + 24) << 16;
        offset += *table++;
        rec->y = y;
        rec->field_10 = 0;
        rec->field_18 = 0;
        y += (s32)0xfff80000;
        rec++;
    }

    for (i = 0; i != 160; i++)
        ((u16 *)(state + 0x4d8))[i] = 0;

    for (i = 0; i != 40; i++) {
        angle = i * 0x199;
        ((u16 *)(state + 0x4d8))[23 + i] = (Trig_Cos(angle) * 3) >> 15;
        *(u16 *)(state + (110 - i) * 2 + 0x4d8) = (Trig_Cos(angle) * 3) >> 15;
    }

    *(s32 *)(state + 0x94) = 0;
    *(s32 *)(state + 0x8c) = 0;
    *(s32 *)(state + 0x90) = 0;
    *(s32 *)(work + 0x7780) = 1;
    *(s32 *)(work + 0x7784) = 0;
    *(u16 *)0x04000050 = 0;
    window = UiWindow_CreateFar(18, 0, 12, 4, 6);
    *(s32 *)(state + 0x4cc) = window;
    UiText_DrawCharacterAtOffsetFar(0x905, window, 0, 8);
    UiText_DrawCharacterAtOffsetFar(0x904, *(s32 *)(state + 0x4cc), 0, 0);
}
