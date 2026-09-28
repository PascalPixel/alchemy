/* NONMATCHING: resource_3bc at 0x02008ba4 (184 bytes with its pool),
 * FieldScene_RunSupplementalSequenceOne, after FIELD/KOROSSEO_MARUTA/ACTIVE_ACTOR.C,
 * stays listing.
 *
 * Remaining difference: it loads message 0x2073 once from its pool and shows
 * later lines from that register, as a link-time message symbol does; the
 * main image has no name for the message.
 */
#include "SITES.H"

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_02000240[];
    u8 *base;
    s32 p10;
    s32 p8;
    s32 base7_2073;
    s32 threea0;
    s32 mode;

    p10 = *(s32 *)StageSceneWork;
    base = Data_02000240;
    p8 = *(s32 *)(base + 500);
    mode = *(s16 *)(base + 450);
    if (mode == 2) {
        Event_Begin();
        base7_2073 = (s32)Data_00002073;
        threea0 = (a0 << 1) + a0;
        Event_SetMessage(threea0 + base7_2073);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(p8, 0) == 0) {
            s32 t1 = base7_2073 + 1;
            Event_SetMessage(threea0 + t1);
            Event_ShowMessage(a0, 0);
            *(s32 *)((0x1c0 + p10)) = 0x200;
            *(s32 *)((0x1c8 + p10)) = 15;
            Event_CloseScreen();
            Event_WaitForScreen();
            Func_02003262_head(a0);
            Event_OpenScreen();
            Event_WaitForScreen();
        } else {
            s32 t2 = base7_2073 + 2;
            Event_SetMessage(threea0 + t2);
            Event_ShowMessage(a0, 0);
        }
        Event_End();
    }
}
