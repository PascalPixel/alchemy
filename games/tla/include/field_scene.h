#ifndef TLA_FIELD_SCENE_H
#define TLA_FIELD_SCENE_H

#include "types.h"
#include "party_state.h"

struct SceneRecord {
    s16 resourceId;
    s8 group;
    s8 variant;
    u16 mapIndex;
    s16 attributes;
};

extern const struct SceneRecord Field_SceneTable[];

enum Facing
{
	FACING_EAST = 0x0000,
	FACING_SOUTHEAST = 0x2000,
	FACING_SOUTH = 0x4000,
	FACING_SOUTHWEST = 0x6000,
	FACING_WEST = 0x8000,
	FACING_NORTHWEST = 0xa000,
	FACING_NORTH = 0xc000,
	FACING_NORTHEAST = 0xe000
};

enum
{
	SCENE_TABLE_END = -1,
	CONDITION_ALWAYS = -1
};

// 出現位置
struct SceneEntrance
{
	s16 entrance;
	s16 requiredFlag;
	s16 x;
	s16 y;
	s16 z;
	u16 facing;
	s16 unused1;
	s16 cameraLeft;
	s16 cameraTop;
	s16 cameraRight;
	s16 cameraBottom;
	s16 unused2;
};

// 出口: 先頭はこのシーン番号、以下は出口ごとの行き先
#define SCENE_EXITS(scene) ((u32)(scene))
#define SCENE_EXIT(exit, scene, entrance) ((u32)(exit) << 20 | (u32)(entrance) << 12 | (u32)(scene))
#define SCENE_EXIT_IF(exit, scene, entrance, flag) \
	(SCENE_EXIT(exit, scene, entrance) | 0x10000000), (u32)(flag)
#define SCENE_EXITS_END 0x1ff

// 配置
struct ScenePlacement
{
	s16 sprite;
	s16 condition;
	s32 behavior;
	s32 x;
	s32 y;
	s32 z;
	u16 facing;
	u8	talkFacing;
	u8	flags;
};

#define PIXELS(value) ((value) << 16)

// イベント
struct SceneEvent
{
	u32 control;
	s16 trigger;
	s16 condition;
	u32 value;
};

#define FIND_MESSAGE(message) (0x00400000 | (message))
#define EVENT_SCRIPT(script) ((u32)(script))
#define SCENE_EVENTS_END 0xffffffff

struct SceneRegion;

#endif
