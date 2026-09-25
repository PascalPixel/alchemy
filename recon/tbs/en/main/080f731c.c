/* Draft, not exact (2026-09-25): 324 of 324 bytes, 12 halfwords differ, register-only.
   Residuals: the five zero stores reuse the loop counter i (r6) where the reference holds a
   fresh zero in r2 and rematerialises the window call argument as movs r1, #0; a plain 0,
   or any other local, lets CSE fold the zero into r1 and costs 4 bytes. In the eight-record
   loop the reference schedules offset += before y += (a sched2 tie). */
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
    s32 message;
    u32 pos;
    u8 *table;

    state = Data_03001f04;
    work = *(&Data_03001f04 - 6);  /* FAKEMATCH: addressed from the neighbouring cell */
    offset = 0;
    for (i = 0; i != 0x800; i++)
        ((struct Particle *)0x02010018)[i].x = 0;

    rec = (struct Particle *)(work + 0x7080);
    y = (s32)0xffe00000;
    table = Data_080f8736;
    for (i = 0; i != 8; i++) {
        rec[i].x = (offset + 24) << 16;
        offset += *table++;
        rec[i].y = y;
        rec[i].field_10 = 0;
        rec[i].field_18 = 0;
        y += (s32)0xfff80000;
    }

    for (i = 0; i != 160; i++)
        ((u16 *)(state + 0x4d8))[i] = 0;

    for (i = 0; i != 40; i++) {
        angle = i * 0x199;
        ((u16 *)(state + 0x4d8))[23 + i] = (Trig_Cos(angle) * 3) >> 15;
        *(u16 *)(state + (pos = (110 - i) * 2 + 0x4d8))  /* FAKEMATCH: keeps the register-offset strh */ = (Trig_Cos(angle) * 3) >> 15;
    }

    i = 0;
    *(s32 *)(state + 0x94) = i;
    *(s32 *)(state + 0x8c) = i;
    *(s32 *)(state + 0x90) = i;
    *(s32 *)(work + 0x7780) = 1;
    *(s32 *)(work + 0x7784) = i;
    *(u16 *)0x04000050 = i;
    window = UiWindow_CreateFar(18, 0, 12, 4, 6);
    *(s32 *)(state + 0x4cc) = window;
    message = 0x905;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
    UiText_DrawCharacterAtOffsetFar(message - 1, *(s32 *)(state + 0x4cc), 0, 0);
}
