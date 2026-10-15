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
#include "object_lookup.h"
#include "system.h"
#include "ram_buffer.h"


struct EventWordWork {
    u8 unk00[0x10];
    s32 word10;
};

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}

void Event_SetSceneEvents(s32 events)
{
    ((struct EventWordWork *)Ram_HeapSlots->eventWork)->word10 = events;
}
