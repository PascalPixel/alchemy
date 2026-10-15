#ifndef SUHALLA_INTERIORS_H
#define SUHALLA_INTERIORS_H

#include "types.h"
#include "field_service.h"
#include "field_scene.h"
#include "scene.h"

struct SceneRecord
{
    u8  unk00[9];
    u8  flags;
    u8  unk0a[20];
    u16 angle;
    u8  unk20[6];
    u8  field26;
};

union SceneActor
{
    struct
    {
        u8                  unk00[12];
        s32                 y;
        u8                  unk10[64];
        struct SceneRecord *record;
        u8                  unk54;
        u8                  mode;
        u8                  unk56[3];
        u8                  flags;
    } fields;
    u8 bytes[90];
};

#endif
