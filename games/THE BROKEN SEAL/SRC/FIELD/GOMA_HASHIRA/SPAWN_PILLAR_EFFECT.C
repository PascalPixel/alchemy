#include "TYPES.H"

u8 *Object_GetById(s32 actor);
u8 *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetMode(u8 *obj, s32 anim);
void Engine_ObjectSetScript(u8 *obj, u8 *script);
void ObjectGroup_SetChildValue(u8 *obj, s32 palette);

struct ScriptTable {
    u8 *script[3];
};

/* The three scripts a pillar effect may play. */
extern struct ScriptTable GomaHashira_PillarScripts;

void SceneEffect_AdvancePositionByAxisMode(u8 *o);

struct Sprite389 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

/* Spawn a pillar effect object: flags pick the script (low nibble) and the
 * palette (bits 16-19); the high half of layer picks the sprite layer, 0 to
 * copy the leader's, and its low half is stored at +102. */
void GomaHashira_SpawnPillarEffect(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 flags, s32 layer)
{
    struct ScriptTable table;
    u8 *leader;
    u8 *obj;
    struct Sprite389 *spr;
    u32 sel;

    leader = Object_GetById(0);
    table = GomaHashira_PillarScripts;
    obj = Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = *(struct Sprite389 **)(obj + 80);
    Object_SetMode(obj, (flags + 1) & 15);
    Engine_ObjectSetScript(obj, table.script[flags & 15]);
    ObjectGroup_SetChildValue(obj, ((u32)flags >> 16) & 15);
    obj[85] = 0;
    ((u8 *)spr)[38] = 0;
    *(s32 *)(obj + 108) = (s32)SceneEffect_AdvancePositionByAxisMode;
    *(s32 *)(obj + 48) = a3;
    *(s32 *)(obj + 52) = a4;
    *(u16 *)(obj + 102) = *(u16 *)&layer;
    sel = (u32)layer >> 16;
    switch (sel) {
    case 0:
        spr->layer = (*(struct Sprite389 **)(leader + 80))->layer;
        break;
    case 1:
    case 2:
    case 3:
        obj[35] &= 0xfe;
        spr->layer = sel;
        break;
    }
}
