/* Near miss, differ 10 of 113 (five reordered pairs): The Broken Seal's Scene_FadeColorFromWhite
 * with this game's event work (mode at 0x18a, status at 0x1b2) and party_state.h's scene_cue.
 * With the twin's one-pass store in both branches what remains is load scheduling: the native
 * loads the event work before it starts the cue's offset, and loads the palette address
 * 0x050001e6 before it builds the colour, at the first store and on every pass of its loop.
 * Taking that address into a local first measured 18; four minutes of alchemy permute stayed
 * at 10. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

void DisplayTransition_Finish(s32 mode, s32 frames);
void WaitFrames(s32 frames);

struct SceneEventWork {
    u8 unknown_000[0x18a];
    s8 mode;
    u8 unknown_18b[0x27];
    u16 status;
};

#define PLTT_BACKDROP (*(u16 *)0x05000000)

// 白からの色戻し
s32 Func_080d5c70(void)
{
    struct SceneEventWork *work = Ram_HeapSlots->event_work;
    s32 i;
    s32 c;

    Audio_PlayCue(gPartyState.scene_cue);
    Audio_PlayCue(298);
    Audio_PlayCue(147);
    if (work->mode == 3) {
        s32 color = 0x7fff;

        do {
            /* FAKEMATCH: the one-pass loop keeps the store before the call's arguments, as in The Broken Seal. */
            *(u16 *)0x050001e6 = color;
        } while (0);
        DisplayTransition_Finish(0x401, 16);
        work->status = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            color = (c << 10) | (c << 5) | c;
            *(u16 *)0x050001e6 = color;
            WaitFrames(1);
        }
    } else {
        s32 color = 0x7fff;

        do {
            /* FAKEMATCH: the one-pass loop keeps the store before the call's arguments, as above. */
            PLTT_BACKDROP = color;
        } while (0);
        DisplayTransition_Finish(0x207, 16);
        work->status = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            color = (c << 10) | (c << 5) | c;
            PLTT_BACKDROP = color;
            WaitFrames(1);
        }
    }
}
