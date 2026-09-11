#include "types.h"
#include "scene.h"
#include "abi/ui/apply_table_scale_to_object.h"

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
