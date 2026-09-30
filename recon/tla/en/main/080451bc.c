#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
void UiWindow_SetTilemapEntry(s32, s32, s32, s32, s32);

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

extern u32 gFrameTick;
extern s32 Ui_ObjectPulseScales[];
s32 AffineMatrix_BuildForEffect(struct Effect *efx);

s32 Menu_BuildLocalizedPatternTiles(void)
{
    u32 *vram = (u32 *)0x06006280;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            Iwram_FillWords(tile, 64, 0x44444444);
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
                tile[x] = XorWord(tile[x], Data_08037250[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], Data_08037250[mi].word1);
            }
        }
    }
}
