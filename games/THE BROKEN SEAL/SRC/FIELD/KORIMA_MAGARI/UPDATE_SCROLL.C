#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"
extern u8 gBgScroll[];
extern u32 KorimaMagari_ShakeScroll[];
extern u32 KorimaMagari_ShakeChance;

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

extern s16 *gOv;
extern s16 *gOv2;
extern u16 *gOv3;
extern u8 gUnk[];

u32 Random16Far(void);

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

s32 IwramSignedDivide();   /* 0x02000efa */

s32 IwramSignedDivide();   /* 0x02000f08 */

void State_UpdateScrollRegistersWithPreset(void)
{
    u16 line;
    u32 *source;
    volatile u32 *destination;

    line = *(volatile u16 *)0x04000006;
    source = (u32 *)(gBgScroll + 4);
    destination = (volatile u32 *)0x04000014;

    if (line == 227 || line <= 52) {
        if (((Random16Far() * 100) >> 16) < KorimaMagari_ShakeChance) {
            source = KorimaMagari_ShakeScroll;
        }
    }

    *destination = *source++;
    destination = (volatile u32 *)0x04000018;
    *destination++ = *source++;
    *destination = *source;
}

void State_CopyPresetA0d0WithOffsetB0(void)
{
    u32 *dst;
    const u32 *src;
    u16 *p;

    src = (const u32 *)(gBgScroll + 4);
    dst = KorimaMagari_ShakeScroll;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = *src;
    p = (u16 *)KorimaMagari_ShakeScroll;
    p[1] += 0xb0;
    p[3] += 0xb0;
    p[5] += 0xb0;
}
