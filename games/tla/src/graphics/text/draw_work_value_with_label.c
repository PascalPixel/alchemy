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
#include "adventure.h"
#include "text/msg_ids.h"
#include "edition.h"

void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if (LANGUAGE == LANGUAGE_JAPANESE)
#endif

void UiText_DrawWorkValueWithLabel(s32 work) {
  UiText_DrawNumberAtOffsetFar(gAdventure.coins, 7, work, 8, 0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
#if (LANGUAGE == LANGUAGE_JAPANESE)
  UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}
