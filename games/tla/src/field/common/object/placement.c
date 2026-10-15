#include "field_scene.h"
#include "objcmd.h"
#include "types.h"

// The script of an object that does nothing.
const s32 Object_IdleScript[] =
{
    SC_END,
};

// The Broken Seal's Field_DefaultPlacements: two rows the field copies into
// its work as it starts, one idle object at (300, 0, 300) and the end mark.
const struct ScenePlacement Data_080f21d4[] =
{
    {0, CONDITION_ALWAYS, (s32)Object_IdleScript, PIXELS(300), 0, PIXELS(300), 0x5bb0, 0, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};
