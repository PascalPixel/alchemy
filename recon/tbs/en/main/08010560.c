/* Sole draft proposal for Map_PlayMetatileCopySequence, EN 08010560..080105d4.
 * Improved residual: all six TBS editions emit 53 instructions, 50 common,
 * differ 6. u16 next preserves the native post-wait load into r0 and copy
 * direction. Remaining: adds r3,r0,#0 / cmp r3,r9 instead of mov ip,r0 /
 * cmp ip,r9, and the r8 argument move schedules earlier. next can represent
 * every record->source value and the 0xffff sentinel without loss.
 * The earlier source/test cancelling updates remain exact.
 * Previous best u32 next differed8, loading r3 and copying back into r0.
 * Earlier trials: 7 guarded sentinel copies, 9 source/control definitions,
 * 8 existing-test reuse forms, 8 const-carrier comparisons; none exact.
 * Follow-up: 8 goto-loop carrier/args-position forms (late args changes the
 * native signed loads); 4 zero/restore pairs under sentinel predicates;
 * 4 narrow source/next/cast forms (only next improved); 4 promoted/const/
 * ordinary-register comparison forms. None exact. Whole-TU context checked.
 * Other map sites: 12 split restores, 10 equality/XOR guards, 4 inline
 * multiply/position carriers; 10 byte/cell controls plus natural copy helper.
 */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "iwram_call.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "global_cells.h"
#include "map_scroll.h"
#include "resource.h"
#include "resource_ids.h"
#include "flag_ids.h"

struct MapCopyCommand {
    u16 source;
    s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
    struct MapCopyCommand *record = (struct MapCopyCommand *)command;
    u32 source = record->source;
    u32 end = 0xffff;
    u32 test = source;

    /* FAKEMATCH: the conditional cancelling updates end CSE before the
     * comparison, preserving its ip copy while source stays in r0. Both
     * updates disappear; removing them loses the copy (53 -> 52 English
     * instructions, five rows differ). */
    if (record) {
        source ^= test;
        source ^= test;
    }
    if (test != end) {
        u32 stop = end;
        s16 *args = (s16 *)(command + 1);

        while (1) {
            s32 source_y = args[0];
            s32 width = args[1];
            s32 height = args[2];
            s32 delay = args[3];
            args += 5;

            Map_CopyMetatileIndicesRect(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)width, (u16)height);
            record++;
            WaitFrames((u16)delay);
            source = record->source;
            {
                u16 next = source;

                if (source == stop) next = stop;
                if (next == stop)
                    break;
            }
        }
    }
}
