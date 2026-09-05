/* Placement query followed by the tile-(10,20) scene transition. */
#include "types.h"

#define FieldScene_RunTile10x20Transition Func_02000a68
struct PlacementResult_02000a68 {
    s32 words[6];
};
/* Raw overlay relocation spellings for this owner's fifteen call sites. */
void Func_0200367a(void); void Func_02003724(void);
s32 Func_02000eea(struct PlacementResult_02000a68 *res);
void Func_02001096(struct PlacementResult_02000a68 res);
void Func_02003728(s32,s32); void Func_0200371c(s32,s32,s32);
void Func_020036ba(s32); void Func_020037e0(s32);
void Func_02003748(s32,s32); u8 *Func_020036ee(s32);
void Func_020036c0(s32,s32,s32,s32,s32,s32);
s32 Func_02000d3c(s32,s32,s32,s32,s32,s32);
void Func_020036fc(s32); u8 *Func_0200372a(s32);
void Func_020036f8(u8 *,s32);

static __inline__ void DrawPlacement_02000a68(
    s32 left, s32 top, s32 width, s32 height, s32 tile, s32 palette)
{
    Func_020036c0(left, top, width, height, tile, palette);
}
void FieldScene_RunTile10x20Transition(void)
{
    struct PlacementResult_02000a68 res;
    Func_0200367a();

    if (Func_02000eea(&res)) {
        Func_02001096(res);
        if (res.words[1] == 10 && (res.words[2] >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Func_02003728(10, 3);
            Func_0200371c(10, -18, 6);
            Func_020036ba(30);
            Func_020037e0(240);
            Func_02003748(10, 8);
            Func_020036ee(10)[35] = 2;
            zero = 0;
            DrawPlacement_02000a68(0, 17, 2, 4, 19, 17);
            Func_02000d3c(2, 20, 17, 1, 4, zero);
            Func_020036fc(0x200);
            actor = Func_0200372a(10);
            Func_020036f8(actor, 0);
        }
    }

    Func_02003724();
}
