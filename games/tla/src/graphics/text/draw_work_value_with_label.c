#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "tla_edition.h"
#include "sequence_parent_work.h"
#include "text/msg_ids.h"
#include "edition.h"

void Text_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if (LANGUAGE == LANGUAGE_JAPANESE)
#endif

void Text_DrawWorkValueWithLabel(s32 work) {
  Text_DrawNumberAtOffsetFar(gSequenceParentWork.coins, 7, work, 8, 0);
  Text_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
#if (LANGUAGE == LANGUAGE_JAPANESE)
  Window_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}
