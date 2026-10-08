//--------------------------------------------------------------------------
// RUN_MERCURY_DJINN_CAPTURE.C
// フィールド演出：水のジンが仲間になるとき、跳ねて残像を引いて昇り、粒を降らせる
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "FX_SCENE.H"
#include "GAME_STATE.H"
#include "PALBUF.H"

#define BattleFx_HalveDistanceToTarget ((void (*)(void))BattleFx_HalveDistanceToTarget)
#define Object_ReplaceResourceEntry                                                   \
	((struct CaptureResource * (*)(struct CaptureSprite *, struct CaptureResource *)) \
			Object_ReplaceResourceEntry)
#define Camera_WorldToScreen ((void (*)(s32 *))Camera_WorldToScreen)
#define EffectSlot_Initialize ((void (*)(void *, s32, s32, s32))EffectSlot_Initialize)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(s32, s32))ObjectGroup_SetChildValueUnlessFifteenFar)

extern struct BattleFxScene *gEffectWork;

struct CaptureSprite
{
	u8	 unknown_00[40];
	s16 *resource; // 先頭はオブジェクトの種類
};

struct CaptureObject
{
	u8					  unknown_00[6];
	u16					  facing;
	s32					  x;
	s32					  y;
	s32					  z;
	u8					  unknown_14[4];
	s32					  scale_x;
	s32					  scale_y;
	u8					  unknown_20[3];
	u8					  priority_flags;
	u8					  unknown_24[44];
	struct CaptureSprite *sprite;
	u8					  unknown_54;
	u8					  mode;
	u8					  unknown_56[4];
	u8					  collision_flags;
	u8					  visible;
	u8					  unknown_5c[12];
	struct CaptureObject *linked;	// 追いかける相手
	void				 *callback; // 毎フレームの処理
};

struct CaptureResource
{
	u8 unknown_00[28];
	u8 id;
};

struct CaptureObject *Object_GetById(s32 id);
void				  WaitFrames(s32 frames);
void				  Audio_PlayCue(s32 cue);
void				  ObjectMotion_Launch(s32 id, s32 height, s32 frames);
struct CaptureObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void				  Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);
void				  Object_SetMode(struct CaptureObject *object, s32 mode);
void ObjectDispatch_SetSingleChildFlagsFar(struct CaptureObject *object, s32 value);
void ObjectDispatch_ReleaseFar(struct CaptureObject *object);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

#define ZANZOU_MAX 8 // 残像の数
#define TSUBU_MAX 24 // 降らせる粒の数 (scene->slots)

//--------------------------------------------------------------------------
// 画面を青く染め、ジンを 3 回跳ねさせてから 8 つの残像を引いて 120 ドット
// 昇らせて消す。selected_actor を mode 28 にしたあと、そのまわりに粒を
// 1 フレームに 1 つずつ 24 個降らせ、最後に色を戻す
// id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnCapture(s32 id)
{
	struct CaptureObject   *jin;
	struct CaptureObject   *copy;
	struct CaptureObject   *previous;
	struct CaptureResource *resource;
	void				   *scene;
	struct CaptureObject  **cursor;
	struct CaptureObject  **list;
	u8					   *slot;
	s8					   *slot_state;
	s32						kind;
	s32						remaining;
	s32						resource_id;
	struct CaptureObject   *copies[ZANZOU_MAX];
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	} position;

	jin = Object_GetById(id);
	if (jin == NULL)
		return;

	// 画面の色を青 (R4 G4 B16) で染める
	BattleFx_InitializeSlots();
	scene = gEffectWork;
	Shop_RestoreSceneTilesFar(PALETTE_TRANSFORM_TINT | (16 << 10) | (4 << 5) | 4);
	WaitFrames(30);
	jin->visible = 0;
	Audio_PlayCue(173);
	Motion_SetVarCbAndRefresh(id, 1);
	Audio_PlayCue(175);
	Motion_SetVarCbAndRefresh(id, 1);
	WaitFrames(20);

	// 3・5・7 ドット/フレームで 3 回跳ねる
	Audio_PlayCue(152);
	ObjectMotion_Launch(id, 3, 14);
	Audio_PlayCue(152);
	ObjectMotion_Launch(id, 5, 16);
	Audio_PlayCue(152);
	ObjectMotion_Launch(id, 7, 18);
	WaitFrames(20);

	// 残像：同じ絵を 8 つ作り、それぞれ一つ前を半分ずつ追いかけさせる
	kind = *jin->sprite->resource;
	resource = NULL;
	previous = jin;
	cursor = copies;
	for (remaining = ZANZOU_MAX - 1; remaining >= 0; remaining--)
	{
		copy = Object_CreateFar(kind, jin->x, jin->y, jin->z);
		*cursor++ = copy;
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->mode = 0;
			copy->priority_flags = 2;
			copy->collision_flags |= 1;
			copy->callback = (void *)BattleFx_HalveDistanceToTarget;
			copy->facing = jin->facing;
			Animation_ApplyChildValuesFar(copy, 9);
			Object_SetMode(copy, 0);
			ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
			resource = Object_ReplaceResourceEntry(copy->sprite, resource);
			copy->linked = previous;
			previous = copy;
		}
	}

	// 8 ドットずつ 15 フレーム昇らせ、ジンと残像を消す
	resource_id = resource->id;
	Audio_PlayCue(153);
	jin->mode = 0;
	for (remaining = 14; remaining >= 0; remaining--)
	{
		jin->y += 8 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ReleaseFar(jin);
	list = copies;
	for (remaining = ZANZOU_MAX - 1; remaining >= 0; remaining--)
		ObjectDispatch_ReleaseFar(*list++);
	if (resource_id != 96)
		Resource_ResetEntry(resource_id);
	WaitFrames(10);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	Object_SetMode(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(20);

	// 粒 (種類 240) を 1 フレームに 1 つずつ出す。動きは BattleFx_UpdateRandomTargetParticle
	position.x = jin->x;
	position.y = jin->y;
	position.z = jin->z;
	Camera_WorldToScreen(&position.x);

	slot = (u8 *)scene + 88; // scene->slots[0]
	remaining = TSUBU_MAX - 1;
	do
	{
		EffectSlot_Initialize(slot, 240, position.x, position.z);
		EffectSlot_SetCallback((struct EffectSlot *)slot, BattleFx_UpdateRandomTargetParticle);
		EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)slot, 9);
		remaining--;
		WaitFrames(1);
		slot += 72; // sizeof(struct EffectSlot)
	} while (remaining >= 0);

	// 120 フレーム後、動いている粒 (active) の state を 2 にする
	WaitFrames(120);
	{
		s32 next_state;

		next_state = 2;
		slot_state = scene;
		slot_state += 152; // scene->slots[0].state
		remaining = TSUBU_MAX - 1;
		do
		{
			if (slot_state[5] != 0) // active
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	// 色を戻す
	WaitFrames(50);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
