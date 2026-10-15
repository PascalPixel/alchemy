#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


s32 Ui_CountSecondTableEntries(void)
{
    return gUiIcon_PsynergyIconPointersEnd - gUiIcon_PsynergyIconPointers;
}
