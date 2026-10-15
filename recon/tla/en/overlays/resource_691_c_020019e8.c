/* Near miss: Func_020019e8 and Func_02001a38 each differ 2 (one reordered row): the cue argument is copied
 * before the work pointer is loaded; the listing copies it after the party state address. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "magma_rock.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Func_020014bc(s32 mode, s32 cue);
void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003b7c(s32 a);
void Func_02003bcc(s32 value);
void Func_02003c54(s32 a);

void Func_020019e8(s32 mode, s32 cue)
{
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    if (party->scene == (s32)&SceneId_Ov691Room1)
        work->target = Object_GetById(15);
    else if (party->scene == (s32)&SceneId_Ov691Room2)
        work->target = Object_GetById(14);
    Func_020014bc(mode, cue);
    GameFlag_SetBit(0x200);
}

void Func_02001a38(s32 mode, s32 cue)
{
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    if (party->scene == (s32)&SceneId_Ov691Room1)
        work->target = Object_GetById(16);
    else if (party->scene == (s32)&SceneId_Ov691Room2)
        work->target = Object_GetById(10);
    Func_020014bc(mode, cue);
    GameFlag_SetBit(0x201);
}

