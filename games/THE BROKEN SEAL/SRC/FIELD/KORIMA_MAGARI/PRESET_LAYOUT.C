#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"

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

void State_ApplyRectByLayoutSelector(void)
{
    if (**(s16 **)0x020092c4 == 1) {
        s32 fifth = 4;
        s32 sixth = 9;
        Map_CopyCellAttributeRect(0, 0, 1, 4, fifth, sixth);
    } else {
        s32 fifth = 6;
        s32 sixth = 9;
        Map_CopyCellAttributeRect(0, 0, 1, 4, fifth, sixth);
    }
}
