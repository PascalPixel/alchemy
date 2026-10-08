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
#include "system.h"
#include "mem.h"
#include "owner_state.h"
#include "serial_runtime.h"
#include "tla_edition.h"
#include "text/msg_ids.h"



s32 UpdateNameEntries(void)
{
    u16 name_text[16];
    void *buffer;
    u8 *name_entry;
    s32 named_count;
    s32 index;
    s32 len;
    s32 i;

    buffer = Runtime_BumpAllocateAlternatePool(340);
    named_count = 0;
    index = 0;
    while (index <= 2) {
        name_entry = UnitGet(index + 128);
        if (SerialRuntime_BeginTransferB(name_entry) == -1) {
            break;
        }
        SerialRuntime_WaitForTransferB();
        if (name_entry[298] != 0) {
            named_count += 1;
        }
        WaitFrames(2);
        Ui_AdjustValueWithoutLimitFar((s32)MsgEnemyLabel, name_text);
        i = 0;
        if (name_text[i] != 0) {
            do {
                i += 1;
                if (i > NAME_PREFIX_LAST_INDEX) {
                    break;
                }
            } while (name_text[i] != 0);
        }
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
        name_text[i] = ' ';
        len = i + 1;
#else
        len = i;
#endif
        for (i = 14; i >= len; i--) {
            name_entry[i] = name_entry[i - len];
        }
        for (i = 0; i < len; i++) {
            name_entry[i] = name_text[i];
        }
        name_entry[14] = 0;
        index += 1;
    }
    MemFree(buffer);
    buffer = Runtime_BumpAllocateAlternatePool(340);
    if (SerialRuntime_BeginTransferB(Resource_FarCall005(1)) != -1) {
        SerialRuntime_WaitForTransferB();
        WaitFrames(2);
    }
    MemFree(buffer);
    return named_count;
}
