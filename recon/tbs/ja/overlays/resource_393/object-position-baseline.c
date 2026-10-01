/* NONMATCHING: Japanese Korima river's selected-actor position callback.
 * 2026-10-01, approved TBS flags: 76 bytes against the Japanese 56-byte
 * leader callback. The complete source overlay is 4308 against 4288.
 * The later-edition rounding and selected actor are preserved here.
 */
#include "TYPES.H"
#include "RESOURCE_393.H"

u8 *Object_GetById();
s32 StagedActor_RunStepEffect(struct Resource393Position *request);

void SceneActor_ApplyOffsetObjectPosition(void)
{
    struct Resource393Position pos;
    struct Resource393Object *obj = Object_GetById(Data_02000240.object_id);
    u32 xb = obj->position_x & 0xfff00000;

    pos.x = xb + 0x80000;
    pos.y = obj->position_y;
    pos.z = (obj->position_z & 0xfff00000) + 0x80000;
    pos.x = xb + 0x280000;
    StagedActor_RunStepEffect(&pos);
}
