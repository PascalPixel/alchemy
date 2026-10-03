#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PRESENTATION.H"

extern struct BattlePresentationTransition *gTransitionWork;
void BattleUnit_ClearField12bForGroup(void);
void Palette_CopyBanksWithBrightnessOffset(s32 offset);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Camera_ConfigureScene(s32 scene);
void UiWork_FinalizeSharedSlotFar(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *actions);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
s32 BattlePres_WaitSync(void);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *actions);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
void Camera_InitDefaultTransform(void);

extern u8 gKeysHeld[];
void BattleCamera_SetRange(s32, s32, s32, s32, s32);

void Palette_CopyBanksWithBrightnessOffset(s32 offset)
{
    s32 iter;
    s32 bank;
    s32 col;
    s32 mask;
    s32 idx;

    bank = 15;
    iter = 0;
    mask = 31;
    do
    {
        for (col = 0; col <= 15; col++)
        {
            u32 color;
            s32 blue;
            s32 green;
            s32 red;
            s32 value;

            idx = bank * 16 + col;
            color = ((u16 *)0x05000000)[idx];
            blue = (color >> 10) & mask;
            green = (color >> 5) & mask;
            red = color & mask;
            blue += offset;
            green += offset;
            red += offset;
            if (blue > 31)
                blue = 31;
            if (green > 31)
                green = 31;
            if (red > 31)
                red = 31;
            if (blue < 0)
                blue = 0;
            if (green < 0)
                green = 0;
            if (red < 0)
                red = 0;
            value = blue << 10;
            value |= green << 5;
            value |= red;
            ((s16 *)0x04FFFFE0)[idx] = (s16)value;
        }
        iter++;
        bank = 5;
    } while (iter <= 1);
}

/* Builds and sorts this turn's actions, setting guard levels for defensive
   actions. Returns the action count, or -1 when the link fails. */
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions)
{
    struct BattleUnit *character;
    struct BattleSession *battle;
    u8 *mode;
    s32 count;
    s32 added;
    s32 i;

    battle = gBattleWork;
    {
        struct BattleActionRecord *queued = battle->actions;
        u32 n;

        for (n = 0; n < 20; n++) {
            queued[n].unit_id = 0xff;
            queued[n].priority = 0x8000;
        }
    }

    BattleUnit_ClearField12bForGroup();
    Palette_CopyBanksWithBrightnessOffset(8);
    GameFlag_SetBitFar(0x16b);
    mode = &battle->encounter_mode;
    Camera_ConfigureScene(0);
    UiWork_FinalizeSharedSlotFar();

    if (*mode != 2) {
        count = BattlePres_BuildUnitEntries(actions);
        if (count < 0)
            goto finish;
        if (count != 0 && actions[0].command == 99 && BattleEscape_CheckSuccess() == 0)
            *mode = 2;
    } else {
        count = 0;
    }

    if (gBattleWork->two_sided != 0) {
        added = BattlePresentation_AppendLinkedActions(actions, count);
        if (BattlePres_WaitSync() < 0) {
            count = -1;
            goto finish;
        }
        count += added;
        if (added < 0) {
            count = -1;
            goto finish;
        }
    } else {
        count += BattlePres_BuildOpponentEntries(actions + count);
    }

    BattleQueue_SortByPriority(actions, count);
    if (count > 0) {
        struct BattleActionRecord *action = actions;

        i = count;
        do {
            if (action->command == 3 || action->command == 7) {
                character = Owner_GetStateFar(action->unit_id);
                character->guard_level = action->command == 3 ? 1 : 2;
            }
            action++;
            i--;
        } while (i != 0);
    }

finish:
    GameFlag_ClearBitFar(0x16b);
    Camera_InitDefaultTransform();
    gTransitionWork->target_yaw = 0x2000;
    return count;
}

/* A tagged slot names a member by its low nibble; bit 7 selects the enemy. */
s16 Battle_GetTaggedSlotValue(s32 slot)
{
    struct BattleSession *battle = gBattleWork;

    if ((slot & 0x80) != 0)
        return battle->enemy_units[slot & 0xf];
    return battle->party_units[slot & 0xf];
}

/* Removed members are skipped; the end marker means no matching slot. */
s32 Battle_FindTaggedSlotByValue(u32 value)
{
    /* FAKEMATCH: indexing the two arrays introduces index shifts and moves the
       session load. Keep the existing separate byte walks over its members. */
    s32 index;
    s32 tag;
    s32 offset;
    char *base;
    s16 item;

    base = (char *)gBattleWork;
    if (value <= 7) {
        tag = 0x80;
        index = 0;
        tag <<= 1;
        offset = (u32)&((struct BattleSession *)0)->party_units;
first:
        item = *(s16 *)(offset + (u32)base);
        if (item == 0xff)
            return -1;
        if (item == 0xfe)
            goto next_first;
        if (item == value)
            return index | tag;
next_first:
        offset += sizeof(s16);
        index++;
        goto first;
    }

    tag = 0xc0;
    index = 0;
    base += sizeof(s16);
    tag <<= 1;
    offset = (u32)&((struct BattleSession *)0)->enemy_units - sizeof(s16);
second:
    item = *(s16 *)(offset + (u32)base);
    if (item == 0xff)
        return -1;
    if (item == 0xfe)
        goto next_second;
    if (item == value)
        return index | tag;
next_second:
    offset += sizeof(s16);
    index++;
    goto second;
}

/* battle/presentation/cam/shoulder_alt.c */
void BattlePres_AdjustCameraByShoulderKeysAlt(void)
{
    /* FAKEMATCH: separate cell loads change the literal pool and register
       order. The existing walk reaches slot 44 from camera slot 12. */
    void **slot = (void **)&gCameraWork;
    struct BattleCamera *cam = slot[0];
    struct BattlePresentationTransition *trans = slot[44 - 12];
    volatile u32 *keys = (volatile u32 *)gKeysHeld;

    if ((*keys & 512) != 0) {
        cam->yaw += 512;
    }
    if ((*keys & 256) != 0) {
        cam->yaw -= 512;
    }
    if (trans->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/runtime/reserved_no_op_b.c */
void Battle_ReservedNoOp9B2C(void)
{
}
