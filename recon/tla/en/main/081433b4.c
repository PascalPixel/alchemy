#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_WORK.H"

/* The battle effect work, seen through its shadows of the window, blend and
   BG2 reference registers, which are copied to the hardware once a frame. */
struct DisplayWork {
    u8 unknown_0000[0x77bc];
    u16 win0h;
    u16 win0v;
    u16 win1h;
    u16 win1v;
    u16 winin;
    u16 winout;
    u16 dispcnt;
    u16 bldcnt;
    u16 bldalpha;
    u16 padding;
    s32 bg2x;
    s32 bg2y;
};

#define REG_WIN0H (*(u16 *)0x04000040)
#define REG_WIN1H (*(u16 *)0x04000042)
#define REG_WIN0V (*(u16 *)0x04000044)
#define REG_WIN1V (*(u16 *)0x04000046)
#define REG_WININ (*(u16 *)0x04000048)
#define REG_WINOUT (*(u16 *)0x0400004a)
#define REG_DISPCNT_PLAIN (*(u16 *)0x04000000)
#define REG_BLDCNT (*(u16 *)0x04000050)
#define REG_BLDALPHA (*(u16 *)0x04000052)
#define REG_BG2X (*(s32 *)0x04000028)
#define REG_BG2Y (*(s32 *)0x0400002c)

extern struct DisplayWork *gBattleFxWork;

/* graphics/registers/Display_ApplyWindowBlend.c */

void Graphics_ApplyWindowBlendRegisters(void)
{
    struct DisplayWork *work = gBattleFxWork;

    REG_WIN0H = work->win0h;
    REG_WIN0V = work->win0v;
    REG_WIN1H = work->win1h;
    REG_WIN1V = work->win1v;
    REG_WININ = work->winin;
    REG_WINOUT = work->winout;
    REG_DISPCNT_PLAIN = work->dispcnt;
    REG_BLDCNT = work->bldcnt;
    REG_BLDALPHA = work->bldalpha;
}
