/* Draft of SceneData_SelectTable9e1cByState, resource_3b7 at 0x020081a8 (from FIELD/TOREBI_IZUMI/TOPIC.C).
 * Remaining difference: it compares the scene with 0xbd, which the game loads from its literal pool as a link-time value; an integer compares with an immediate. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

enum TopicMessage {
    MSG_LUCKY_WHEELS_RULES_PULL_LEVER = 0xe34,
    MSG_LUCKY_WHEELS_PRIZES_PRIZES_DETERMINED = 0xe35
};


/* Signed topic cursors share the scene workspace with its halfword state. */
extern s8 SceneWork_Bytes[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];
extern u8 Value_00000e39;
extern u8 Value_00000e19;
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

s32 SceneData_SelectTable9e1cByState(void)
{
    if (gGameState.scene == (s32)&Value_000000bd) {
        return (s32)Data_02009f30;
    }
    return (s32)Data_02009e1c;
}
