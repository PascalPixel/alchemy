#include "TYPES.H"
#include "FIELD_SCENE.H"

u32 Clear_CodeSequence[] = {
	0x40004, 4,
};

u32 Clear_ExtraCodeSequence[] = { 0 };

struct SceneEntrance Clear_ScriptTable[] = {
	{ 0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Clear_MessageTable[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement Clear_ActorTable[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent Clear_EffectTable[] = {
	{ SCENE_EVENTS_END },
};

s16 Clear_BlendFrame = 0;

s16 Clear_CodeUnlocked = 0;

s16 Clear_ExtraCodeUnlocked = 0;

s16 Clear_CodeProgress = 0;

s16 Clear_ExtraCodeProgress = 0;

s16 Clear_CodeHeld = 0;

s32 Clear_ExtraCodeHeld = 0;

u32 Clear_PasswordMembers[] = {
	0, 1, 2, 3,
};

u32 Clear_PasswordFlags[] = {
	0x9510941, 0x8d108b3, 0x868081e,
};

u32 Clear_PasswordKeyItems[] = {
	0xc900c8, 0xcb00ca, 0xcd00cc, 0xcf00ce,
};

u16 Clear_PasswordStackItems[] = {
	180, 181, 182, 183, 186, 187, 188, 189,
	191, 192, 193, 194, 195, 196, 226, 227,
	228, 229, 236, 238, 239, 240, 241,
};
