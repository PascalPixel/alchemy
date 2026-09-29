/* Draft of FieldScene_RunScene39cSequenceB, resource_39c at 0x0200b6ac (split from FIELD/MAKYURI_HEYA/FIELD_PROBE_SCENE.C).
 * Remaining difference: it compares the scene with 0x36, which the game loads from its literal pool as a link-time value; an integer compares with an immediate. Its calls still use per-call-site names. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s16 Data_02000240[];
extern s16 Data_02000240_t[][1];
extern u8 *Data_03001ebc;
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Value_00000039;
extern u8 Data_00000036[];

void FieldScene_RunScene39cSequenceB(void)
{
    Event_Begin();
    if (Data_02000240_t[224][0] == (s32)Data_00000036) {
        Actor_WalkToAndWait(0, 0x1d8, 0x258);
        Actor_FaceDirection(0, 0x4000, 10);
        Camera_MoveTo(0x1d00000, -1, 0x2900000, 1);
        Actor_SetSpriteFlags(Actor_Get(0), 0);
        Func_02003756_tail(Actor_Get(0)->x.fixed, 0, 0x2be0000, 223);
        Map_CopyCellsTo(92, 46, 92, 40, 3, 2);
        FIELD_AT_OFFSET(Actor_Get(0), s32, 72) = 0x8000;
        Actor_SetSpritePriority(0, 2);
        Call3(Func_02009296_tail, 0, 6, -1);
        *(s32 *)(Data_03001ebc + 0x1c0) = 0x203;
        Event_Wait(60);
        Event_RequestExit(8);
    } else {
        Call3(Func_020092be_tail, 0, 6, -1);
    }
    Event_End();
}
