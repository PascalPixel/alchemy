#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "types.h"
#include "field_event.h"
#include "battle_session.h"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(s32,s32))ObjectDispatch_ApplyValueToChildrenFar)

s32 *BtlObjGet(s32);

struct StatusIconRecord
{
    u8 unknown_00[0x14];
    u8 width;
    u8 unknown_15[4];
    u8 dirty;
};

struct StatusIconEntry
{
    u8 unknown_00[6];
    u8 priority;
};

s32 ResourceMetadata_RegisterFar(struct StatusIconRecord *record, s32 animation);
void ResourceMetadata_UnregisterFar(struct StatusIconRecord *record, s32 entry);
void Animation_SetWorkEntryFar(struct StatusIconEntry *entry, s32 index);

void *GetMotionRecord(struct MotionObject *object, s32 record_index);

#define ICON_MAX 14

// Cycles the status icon over the slot's conditions every 80 frames, and
// switches at once when the shown condition ends.
void BattleStatusIcon_Cycle(struct BtlObj *slot)
{
    struct StatusIconEntry *icon;
    struct StatusIconRecord *rec;
    struct MotionObject *owner;
    s32 anim;
    s32 handle;
    s32 prev;
    s32 changed = 0;

    if (slot->cycle_timer >= 0)
        slot->cycle_timer--;

    if (((s16)slot->active_conditions != 0 && slot->animation_entry == 0)
        || (slot->animation_entry != 0 && (((s16)slot->active_conditions >> slot->selected_condition) & 1) == 0)
        || slot->cycle_timer == 0)
    {
        anim = -1;
        owner = slot->object;
        if ((s16)slot->active_conditions != 0)
        {
            prev = slot->selected_condition;
            for (anim = prev + 1;; anim++)
            {
                if (anim > ICON_MAX - 1)
                    anim = 0;
                if ((((s16)slot->active_conditions >> anim) & 1) != 0)
                    break;
            }

            if (prev != anim || slot->animation_entry == 0)
            {
                /* FAKEMATCH: the native stores the condition before it sets changed; plainly changed is set first (one reordered row, differ 2). The one-pass block ends the store's scheduling block, as an empty barrier would, without its 4 estimated bytes. */
                do
                {
                    slot->selected_condition = anim;
                } while (0);
                changed = 1;
            }
            slot->cycle_timer = 80;
        }
        else
        {
            changed = 1;
        }

        rec = GetMotionRecord(owner, 0);
        if (rec != 0)
        {
            if (anim >= 0)
            {
                if (rec->width == 32)
                    anim += 702;
                else
                    anim += 688;
            }

            if (slot->animation_entry != 0 && changed != 0)
            {
                ResourceMetadata_UnregisterFar(rec, (s32)slot->animation_entry);
                slot->animation_entry = 0;
            }

            if (anim >= 0 && changed != 0)
            {
                handle = ResourceMetadata_RegisterFar(rec, anim);
                slot->animation_entry = (struct SpriteEntry *)handle;
                if (handle == -1)
                    slot->animation_entry = 0;
                icon = (struct StatusIconEntry *)slot->animation_entry;
                if (icon != 0)
                {
                    icon->priority = 3;
                    Animation_SetWorkEntryFar(icon, 0);
                }
            }

            rec->dirty = 1;
            if (anim >= 0)
                slot->animation = anim;
            else
                slot->animation = 0;
        }
    }
}

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    struct BtlUnit *state;
    s32 value;

    state = UnitGet(id);
    value = 1;
    if (state->hp != 0) {
        if (state->sleep != 0 || state->stun != 0 || state->immobile != 0)
            value = (state->side != 1) * 4;
    } else {
        s32 changed = state->side ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode(*BtlObjGet(id), value);
    ObjectDispatch_ApplyValueToChildrenFar(*BtlObjGet(id), (id & 3) + 14);
}

