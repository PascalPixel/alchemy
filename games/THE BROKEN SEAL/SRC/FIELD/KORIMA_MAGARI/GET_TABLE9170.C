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

u8 *SceneData_GetTable9170(void)
{
    return (u8 *)0x02009170;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable91d0(void)
{
    return (u8 *)0x020091d0;
}

u8 *SceneData_GetTable91e0(void)
{
    return (u8 *)0x020091e0;
}

u8 *SceneData_GetTable9240(void)
{
    return (u8 *)0x02009240;
}
