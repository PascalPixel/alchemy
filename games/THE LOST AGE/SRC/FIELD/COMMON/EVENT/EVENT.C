#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"


struct EventWordWork {
    u8 unknown_00[16];
    s32 word_10;
};

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}
