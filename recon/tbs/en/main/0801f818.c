/* Pin- and barrier-free SaveState_BuildSummaryHeader trial: all six TBS editions 174
 * native/built, 173 common, differ 2: the frame load precedes movs r1, #242.
 * General, low, early-clobber, memory and explicitly tied operands on the
 * retained barrier differ 10, exchanging the bound and word-pointer r1/r2.
 * Moving that barrier after the frame load, with frames as input or output,
 * also differs 10. u16/u32 bounds and a value-preserving narrow shift added
 * to 242 retain 2; frame[n - 242] differs 4. A two-minute prefix search
 * (seed 401) compiled 4603 candidates without an improvement. The linked
 * barrier still differs 10 with const, byte or void checksum cursors.
 * Extracting the final frame/checksum work into an inline helper differs
 * 37: sum loses its native r8 lifetime, with five fewer instructions.
 * source retains its pin and barrier pending all-edition byte identity. */
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

extern u8 gSceneState[];
extern u32 gLoadedStateWord;
extern u8 gOptionMirror;
extern u32 GameFlagBytes[];

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
