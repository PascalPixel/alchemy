#include "types.h"
#include "sequence_parent_work.h"

void Text_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void Text_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
extern u8 MsgCoinsLabel[];

void Text_DrawWorkValueWithLabel(s32 work) {
  Text_DrawNumberAtOffsetFar(gSequenceParentWork.coins, 7, work, 8, 0);
  Text_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
}
