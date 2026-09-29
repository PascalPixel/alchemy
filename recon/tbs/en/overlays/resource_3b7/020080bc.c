/* Draft of FieldScene_RunPromptDialogueE19, resource_3b7 at 0x020080bc (from FIELD/TOREBI_IZUMI/TOPIC.C).
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_EventChooseYesNo, Engine_EventWait, Engine_EventShowMessage). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgTorebiYerFirstTime[];



/* Signed topic cursors share the scene workspace with its halfword state. */
extern s8 SceneWork_Bytes[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];
extern u8 Data_02009f30[];
extern u8 Data_02009e1c[];
extern s32 Data_0200a018[];     /* in-image, file offset 0x2018: 5 topics x 3 ids */
extern u8 Data_0200a05a[];      /* in-image 0x205a: four X tile coordinates */
extern u8 Data_0200a05e[];      /* in-image 0x205e: four Z tile coordinates */
extern u16 Data_0200a062[];     /* in-image 0x2062: four headings */
extern u8 Data_0200a070[];      /* scratch EWRAM above the image */
extern u8 Data_0200a0d0[];      /* scratch EWRAM above the image: 4 x 24 bytes */

void Func_02000a9a(s32);
s32 Func_02002608(s32, s32);
u8 *Func_02002786(s32);
u8 *Func_02003036();
u8 *Func_02003042();
void Func_02002f80();

void FieldScene_RunPromptDialogueE19(s32 object)
{
    s32 cue = (s32)MsgTorebiYerFirstTime;
    Event_SetMessage(cue);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(object, 0);
}
