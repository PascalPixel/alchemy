#include "ENTRANCE.H"

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Engine_ObjectSetScript and
 * Engine_ObjectSetAnimation are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */
void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *record = **(s32 ***)&gWork.scene;
    s32 *target;

    if (record[0] == 0) {
        return;
    }

    record[0] = 0;
    GameFlag_Clear(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Engine_ObjectSetScript(target, (s32)Makyuri_ScaleCounterScript);
        Object_SetAnimation(target, 7);
        record[5] = 0;
    }
}
