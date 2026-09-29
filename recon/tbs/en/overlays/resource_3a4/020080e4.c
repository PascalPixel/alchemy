/* Draft of resource_3a4 0x020080e4 (SceneData_SelectTableByWord224), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x4d-0x57 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "YAMA.H"

extern u8 Value_0000004d;
extern u8 Data_0200c194[];
extern u8 Value_0000004e;
extern u8 Data_0200c20c[];
extern u8 Value_0000004f;
extern u8 Data_0200c26c[];
extern u8 Value_00000050;
extern u8 Data_0200c314[];
extern u8 Value_00000051;
extern u8 Data_0200c3ec[];
extern u8 Value_00000052;
extern u8 Data_0200c464[];
extern u8 Value_00000053;
extern u8 Data_0200c524[];
extern u8 Value_00000054;
extern u8 Data_0200c59c[];
extern u8 Value_00000055;
extern u8 Data_0200c644[];
extern u8 Value_00000056;
extern u8 Data_0200c704[];
extern u8 Value_00000057;
extern u8 Data_0200c77c[];
extern u8 Data_0200c164[];


/*
 * Table getter for resource_3a4, published from the overlay's header as an
 * entry point.
 *
 * The eight-byte owner at 0x02000204 includes its one pool word at
 * 0x02000208; the load reads that word and returns it as an address,
 * without dereferencing it.
 */
s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c194;
    }
    if (v == (s32)&Value_0000004e) {
        return (s32)Data_0200c20c;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c26c;
    }
    if (v == (s32)&Value_00000050) {
        return (s32)Data_0200c314;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200c3ec;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200c464;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200c524;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200c59c;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c644;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c704;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200c77c;
    }
    return (s32)Data_0200c164;
}
