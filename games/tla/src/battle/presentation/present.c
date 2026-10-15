//--------------------------------------------------------------------------
// present.c
// バトル表示：ユニットの絵の半透明の切りかえ
//--------------------------------------------------------------------------

#include "types.h"
#include "battle_party.h"
#include "battle_display.h"
#include "battle_session.h"
#include "motobj.h"

struct ActorSpritePart
{
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u8 unknown_06[0x16];
};

struct ActorSpriteRecord
{
    struct ActorSpritePart part[2];
};

// Sets the OBJ mode of both sprite parts of each of the actor's records.
void BattlePres_SetActorRecordMode(s32 actor_id, s32 mode)
{
    struct BtlObj *slot = BtlObjGet(actor_id);
    struct MotionObject *object;
    struct ActorSpriteRecord *record;
    struct ActorSpriteRecord **records;
    s32 i;

    if (slot == 0)
        return;
    object = slot->object;
    if (object == 0)
        return;
    switch (object->record_storage_kind & 15)
    {
    case 1:
    {
        record = object->records;
        record->part[0].object_mode = mode;
        /* FAKEMATCH: the native stores the first part before it starts the second part's address; plainly the address comes first (one reordered row here and in the loop, differ 4). A one-pass block on the store moves only this one (differ 2). */
        __asm__ volatile("");
        record->part[1].object_mode = mode;
        break;
    }
    case 2:
    {
        records = object->records;
        i = 0;
        do
        {
            record = *records++;
            if (record == 0)
                break;
            record->part[0].object_mode = mode;
            /* FAKEMATCH: the barrier of the store above. */
            __asm__ volatile("");
            record->part[1].object_mode = mode;
            i++;
        } while (i <= 3);
        break;
    }
    }
}

//--------------------------------------------------------------------------
// except 以外の全ユニットの絵を半透明（OBJ モード 1）にする
// except : そのままにするユニット番号
//--------------------------------------------------------------------------
void BattlePres_SetOtherActorRecordModes(s32 except)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
	{
		if (list[i] != except)
			BattlePres_SetActorRecordMode(list[i], 1);
	}
}

//--------------------------------------------------------------------------
// 全ユニットの絵をふつう（OBJ モード 0）にもどす
//--------------------------------------------------------------------------
void BattlePres_ClearAllActorRecordModes(void)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
		BattlePres_SetActorRecordMode(list[i], 0);
}
