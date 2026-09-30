#include "TYPES.H"
#include "CALL.H"
extern u8 gVinasuPushScript[];
extern u8 gVinasuPushCells[];

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_MapCopyCellAttributes();
s32 Engine_GameFlagIsSet();
s32 OverlayObject_PrepareObjectWithCommand15();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpriteFlags();
void OverlayObject_WaitUntilIdle();
void Engine_AudioPlayCue();
void Engine_ObjectDispatchRelease();
s32 Engine_ActorSetPosition();
void Engine_ActorEnableActionCallback();
void Engine_ActorStartAction();
void Engine_EventWait();
void Engine_MapAnimateCells();
void Engine_GameFlagSet();
void Engine_EventEnd();

/* Venus Lighthouse room: when actor 9 stands on cell (46, 45) and flag 0x301
 * is clear, push it through, shift the map cells, release actor 10 and set
 * the flag. */
void VinasuHeya_RunCellPushScene(void)
{
    u32 i;
    u8 *p10;
    s32 p8;
    s32 p9;
    s32 rec;
    s32 rec5;
    u8 *rec8;
    u8 *record;
    s32 v5;
    s32 slot8;

    slot8 = 0;
    rec8 = Engine_ActorGet(9);
    rec5 = Engine_ActorGet(0);
    v5 = 45;
    Engine_EventBegin();
    Engine_MapCopyCellAttributes(109, 43, 7, 5, v5, 43);
    p10 = (s32)rec8 + 35;
    if ((2 & rec8[35]) != 0) {
        Engine_MapCopyCellAttributes(45, 45, 1, 1, 46, v5);
    } else {
        Call6(Engine_MapCopyCellAttributes, 48, 42, 1, 1, (*(s32 *)((s32)rec8 + 8) >> 20), (*(s32 *)((s32)rec8 + 16) >> 20));
    }
    p9 = (*(s32 *)((s32)rec8 + 8) >> 20);
    if (p9 != 46) {
    } else {
        p8 = (*(s32 *)((s32)rec8 + 16) >> 20);
        if (p8 != 45) {
        } else {
            rec = Engine_GameFlagIsSet(0x301);
            if (rec != 0) {
            } else {
                if ((*(s32 *)(rec5 + 16) >> 20) <= 45) {
                    record = OverlayObject_PrepareObjectWithCommand15(0x2e80000, 0, 0x2c00000, 20);
                    slot8 = (s32)record;
                    Engine_ActorSetSpritePriority(0, 3);
                }
                record = Engine_ActorGet(9);
                Engine_ActorSetSpriteFlags((s32)record, 0);
                rec8[34] = rec;
                rec8[85] = 3;
                *(s32 *)((s32)rec8 + 72) = 0x1999;
                *(s32 *)((s32)rec8 + 68) = rec;
                Engine_MapCopyCellAttributes(43, 45, 1, 1, p9, p8);
                OverlayObject_WaitUntilIdle((s32)rec8);
                Engine_AudioPlayCue(188);
                rec8[85] = rec;
                *(s32 *)((s32)rec8 + 12) = -0x100000;
                Engine_ActorSetSpritePriority(9, 3);
                { s32 two = 2; *p10 = two; } /* FAKEMATCH: a block-local word temporary keeps the 2 from being shared with the later |= 2 */
                Engine_MapCopyCellAttributes(45, 45, 1, 1, p9, p8);
                {
                    u8 *record = Engine_ActorGet(0);
                    u8 value = *(volatile u8 *)&record[35];
                
                    record[35] = (u8)(value | 1);
                }
                Engine_ObjectDispatchRelease(slot8);
                rec8[89] = rec;
                *p10 |= 2;
                *(u8 *)(Engine_ActorGet(10) + 89) = rec;
                {
                    u8 *record = Engine_ActorGet(10);
                    u8 value = *(volatile u8 *)&record[35];
                
                    record[35] = (u8)(value | 2);
                }
                Engine_ActorSetPosition(10, 0x3280000, 0x2d80000);
                Engine_ActorEnableActionCallback(10, (s32)gVinasuPushScript);
                Engine_ActorStartAction(10);
                Engine_EventWait(30);
                Engine_AudioPlayCue(158);
                Engine_MapAnimateCells((s32)gVinasuPushCells, 110, 41);
                Call6(Engine_MapCopyCellAttributes, 46, 41, 1, 1, p9, 42);
                Engine_GameFlagSet(0x301);
            }
        }
    }
    Engine_EventEnd();
}
