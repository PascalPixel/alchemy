#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"

extern char MsgAbilityWoreOff;
extern char MsgItemWoreOff;

void Func_080cb82c(void);
void Func_080cb8a4(void);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);

void FieldEvent_ShowStatusMessage(void)
{
    gPartyState.avoid_distance = 0;
    Func_080cb82c();
    if (gPartyState.avoid_by_item == 0) {
        UiText_DrawQuantity(150, 4);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgAbilityWoreOff, 1);
    } else {
        UiText_DrawQuantity(236, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgItemWoreOff, 1);
    }
    Func_080cb8a4();
}

struct ArcEntry {
    u8 unknown_00[5];
    u8 param;
};

struct ArcGroup {
    u8 unknown_00[25];
    u8 dirty;
    u8 flags;
    u8 unknown_1b[13];
    struct ArcEntry *entries[1];
};

struct ArcEffectObject {
    u8 unknown_00[6];
    u16 angle;
    u8 unknown_08[0x48];
    struct ArcGroup *records;
    u8 unknown_54[0x10];
    s16 phase;
    u16 step;
    u8 unknown_68[4];
    void (*callback)(struct ArcEffectObject *);
};

struct ArcScene {
    u8 unknown_00[0x10];
    struct ArcEffectObject *main_object;
    u8 unknown_14[0x6fc];
    s16 arc_tile_slot;
};

extern const u8 Data_080ed90c[];
extern char MsgMonstersAttackLess;

s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
void VramBlock_LoadCached(s32 slot, s32 size, const u8 *tiles);
s32 GameFlag_Test(s32 flag);
void Animation_ApplyChildValuesFar(void *object, s32 value);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
void Func_080e1154(struct ArcEffectObject *obj);

// エナジー演出 16：使う者が回りながら弧の粒を降らせ、点滅して終わる
void BattleFx_RunEffect16(void)
{
    struct ArcScene *scene;
    struct ArcEffectObject *object;
    struct ArcGroup *group;
    struct ArcEntry *entry;
    u32 angle;
    s32 flags;
    s32 slot;
    s32 count;
    s32 phase;
    s32 zero;

    scene = Ram_HeapSlots->effect_work;
    /* FAKEMATCH: forced temporary, as The Broken Seal's twin: the zero the two
     * counters take lives in r6 from here; the one-pass block keeps its set
     * before the object load (one reordered row without it). */
    zero = 0;
    do { } while (0);
    object = scene->main_object;
    group = object->records;
    entry = group->entries[0];
    angle = object->angle;
    flags = group->flags;
    slot = Resource_FindFreeEntry();
    {
        /* FAKEMATCH: forced temporaries and a one-pass block: ⚓️ reloads the
         * scene before it builds 0x710, and loads the tile address between
         * the two shifts of the slot's sign extension (one reordered row each
         * without them). */
        struct ArcScene *work = scene;
        const u8 *tiles;
        s32 id;

        do { } while (0);
        work->arc_tile_slot = slot;
        id = slot << 16;
        tiles = Data_080ed90c;
        VramBlock_LoadCached(id >> 16, 0x100, tiles);
        gPartyState.avoid_distance = 0x09600000;
        gPartyState.avoid_by_item = GameFlag_Test(0x145);
        Animation_ApplyChildValuesFar(object, 0);
        object->callback = Func_080e1154;
        object->phase = zero;
        object->step = zero;
    }
    Audio_PlayCue(140);
    WaitFrames(15);
    object->phase = 1;
    WaitFrames(10);
    for (count = 0; count < 20; count++) {
        entry->param = 7;
        group->dirty = 1;
        WaitFrames(2);
        entry->param = 0;
        group->dirty = 1;
        group->flags = 1;
        WaitFrames(3);
    }
    object->callback = NULL;
    object->angle = angle;

    // 70 フレーム、5 フレームの周期で flags に 2 を立てては戻す
    for (count = 0; count < 70; count++) {
        phase = count % 5;
        if (phase == 0) {
            group->dirty = 1;
            group->flags = flags | 2;
        } else if (phase == 2) {
            group->dirty = 1;
            group->flags = flags;
            entry->param = 0;
        }
        if (count == 15)
            Audio_PlayCue(174);
        WaitFrames(1);
    }
    {
        /* FAKEMATCH: the flags go back through a pointer, ahead of the dirty
         * mark, and one-pass blocks stand on both sides of the wait: ⚓️ copies
         * the flags out of r11 before it sets the zero, stores the entry's
         * param before the wait's argument, and reloads the scene before it
         * builds 0x710 (one reordered row each without them). */
        u8 *restore = &group->flags;

        *restore = flags;
    }
    group->dirty = 1;
    entry->param = 0;
    do { } while (0);
    WaitFrames(1);
    do { } while (0);
    Resource_ResetEntry(scene->arc_tile_slot);
    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgMonstersAttackLess, 1);
}
