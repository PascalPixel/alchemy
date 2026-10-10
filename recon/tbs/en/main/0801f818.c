/* Pin-free SaveState_BuildSummaryHeader trial: 174 native and built
 * instructions, 173 common, differ 2. Removing the bound binding and
 * its empty barrier moves the build-stamp load before movs r1, #242.
 * Keeping the barrier with an ordinary bound differs 10. A 3-minute
 * whole-function search (seed 103, exact stack and branch comparisons)
 * found no improvement. The staging permuter (seed 241, two jobs, two
 * minutes) likewise found none across 1,612 distinct candidates.
 * The linked source keeps its binding/barrier. */
#include "types.h"
#include "render_input.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "window.h"
#include "btlwork.h"
#include "dma.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "btltype.h"
#include "party_state.h"
#include "mem.h"
#include "btlpty.h"
#include "b5_context.h"
#include "menu_list.h"
#include "btlsys.h"
#include "scene.h"
#include "save_state.h"
#include "game_state.h"
#include "battle_unit.h"
#include "runtime_interfaces.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "irq.h"
#define Runtime_GetBuildStampTimeFar ((u32 (*)(s32))Runtime_GetBuildStampTimeFar)

#define gTitleExtraOptionEnabled (*(s16 *)&gTitleExtraOptionEnabled)

extern volatile u32 gKeysHeld;

extern u8 gSceneState[];
extern u32 gLoadedStateWord;
extern u8 gOptionMirror;
extern u32 GameFlagBytes[];

void UiText_ShowPositionedMessageAndWait(s32, s32);
u32 SaveState_BuildSummaryHeader(void)
{
    u16 owners[14];
    struct SaveSummary *summary;
    struct BtlUnit *owner;
    u8 *source;
    u8 *destination;
    u32 *word;
    s32 i;
    u32 sum = 0;

    summary = (struct SaveSummary *)(gSaveBuffer - 16);
    gGameState.build_stamp = Runtime_GetBuildStampTimeFar(0);
    gGameState.play_time = gLoadedStateWord;
    {
        u32 *copy = (u32 *)gSceneState;
        copy[64] = gLoadedStateWord;
    }
    gGameState.saved_options = gOptionMirror;
    owner = UnitGetFar(gGameState.selected_actor);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->play_time = gGameState.play_time;
    summary->area = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
    summary->class_id = owner->class_index;
    summary->coins = gGameState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar(owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->palette_glow = gGameState.palette_glow[0];
    summary->palette_glow_sub = gGameState.palette_glow[1];
    summary->send_flag = gGameState.unknown_20f;
    summary->flag_count = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_TestFar(i))
            summary->flag_count++;
    }
    summary->has_flag_20 = GameFlag_TestFar(32) != 0;
    {
        /* The plain bound still exchanges the stamp load and first move.
         * Earlier const, early-initializer, volatile-load and unsigned-long
         * field trials left that swap; a literal bound differed 11. */
        s32 n;
        u32 *frame = &gGameState.build_stamp;
        u32 frames;

        n = 242;
        frames = *frame;
        word = GameFlagBytes;
        i = 0;
        n <<= 2;
        summary->frames = frames;
        goto test;
        do {
            sum += *word++;
            i++;
test:;
        } while (i < n);
    }
    summary->checksum = sum;
    return sum;
}
