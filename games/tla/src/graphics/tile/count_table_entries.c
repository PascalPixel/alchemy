#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

s32 Ui_CountIconTableEntries(void)
{
    return gUiIcon_ItemIconPointersEnd - gUiIcon_ItemIconPointers;
}
