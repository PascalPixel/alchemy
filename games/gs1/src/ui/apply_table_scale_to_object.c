#include "types.h"
#include "scene.h"

/* ui/apply_table_scale_to_object.c */
struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned z : 16;
    unsigned unk : 16;
};

struct Object {
    u8 filler0[6];
    u16 src_6;
    u8 src_8;
    u8 filler9[6];
    u8 out_15;
    u8 filler16[4];
    u8 out_20;
    u8 mode_21 : 2;
    u8 rest_21 : 6;
    u16 pos_22 : 9;
    u16 affine_22 : 5;
    u16 rest_22 : 2;
};

extern volatile u32 gIw;
extern s32 gRom[];
s32 AffineMatrix_BuildForEffect(struct Effect *efx);

void Ui_ApplyTableScaleToObject(struct Object *obj)
{
    s32 v = gRom[(gIw >> 1) & 7];
    struct Effect efx;

    if (v < 0)
        v += 255;
    v >>= 8;

    if (obj != 0) {
        efx.x = v;
        efx.y = v;
        efx.z = 0;
        obj->affine_22 = AffineMatrix_BuildForEffect(&efx);
        obj->mode_21 = 3;
        obj->pos_22 = obj->src_6 + 0xfff0;
        obj->out_20 = obj->src_8 + 0xf0;
        obj->out_15 = 0xfc;
    }
}

/* menu/core/build_localized_pattern_tiles.c */
struct TileMask {
    u32 word0;
    u32 word1;
};

typedef void (*FillProc)(void *dst, s32 size, u32 word);

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask MaskTbl[];

#define FillWords ((FillProc)0x03000168)

s32 Menu_BuildLocalizedPatternTiles(void)
{
    u32 *vram = (u32 *)0x06006280;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], MaskTbl[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], MaskTbl[mi].word1);
            }
        }
    }
}

/* ui/window/draw_three_tile_column.c */
volatile int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  int one;
  s32 tile;
  s32 tmp;
  s32 y;
  int pos;
  tmp = 2;
  tmp = tmp;
  tmp = arg3 *tmp;
  tile = tmp + 0xF315;
  y = arg2;
  Ui_SetRange(arg0, 0x400 | tile, arg1, y, 0);
  if (1)
  {
    one = 1;
    Ui_SetRange(arg0, tmp + 0xF314, arg1 + one, arg2, 0);
    pos = arg1 + 2;
    Ui_SetRange(arg0, tile, pos, arg2, 0);
  }
}
