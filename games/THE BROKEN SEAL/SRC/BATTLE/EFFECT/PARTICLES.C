//--------------------------------------------------------------------------
// PARTICLES.C
// バトル効果：持ち上げた物のまわりに落ちる粒
//--------------------------------------------------------------------------

#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_EFFECT.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"
#include "FIELD_EVENT.H"

struct FallPos
{
	s32 x;
	s32 y;
	s32 z;
};

struct FallSource
{
	u8	reserved_00[8];
	s32 x; // 16.16 ドット
	s32 y;
	s32 z;
};

struct FallScene
{
	u8				   reserved_00[20];
	struct FallSource *child; // 持ち上げている物
};

struct FallParticle
{
	u8	reserved_00[72];
	s32 gravity; // FieldActor と同じ位置
	u8	reserved_4c[9];
	u8	motion_flags; // FieldActor と同じ位置
	u8	reserved_56[8];
	u16 unknown_5e;
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct FallPos, 0x0c);
LAYOUT_OFFSET_GUARD(FallSource_X, struct FallSource, x, 0x08);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);
LAYOUT_OFFSET_GUARD(FallParticle_Gravity, struct FallParticle, gravity, 0x48);
LAYOUT_OFFSET_GUARD(FallParticle_MotionFlags, struct FallParticle, motion_flags, 0x55);
LAYOUT_OFFSET_GUARD(FallParticle_Unknown5e, struct FallParticle, unknown_5e, 0x5e);

extern struct FallScene *gEffectWork;

struct FallParticle *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void				 Vector_AddPolarOffset(s32, s32, struct FallPos *);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void)
{
	struct FallSource	*src;
	struct FallPos		 pos;
	struct FallParticle *obj;
	u32					 dist;

	src = gEffectWork->child;
	pos.x = src->x;
	pos.y = src->y - Random16() * 16 + (24 << 16);
	pos.z = src->z;

	dist = Random16() * 3;
	Vector_AddPolarOffset(dist * 16, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = ACTOR_FALLS;
		obj->gravity = 0x1999; // 重力 0.1 ドット
		Object_SetMode((struct FieldActor *)obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}
