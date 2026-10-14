@ alchemy camelotize constants: assembler id positions, as scnrule.s,
@ locrule.s and scripts.s write them, and look-alikes that are none.
#include "flags.h"
#define TOWN_GROUP 2

	.macro scene_cue scene:req, entrance:req, flag:req, cue:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro group_cue group:req, entrance:req, flag:req, cue:req
	.2byte \group, \entrance & 0x7fff, \flag, \cue
	.endm
	.macro location scene:req, entrance:req, flag:req, shift:req, zone:req
	.2byte SceneId_\scene, \entrance, \flag, \shift
	.4byte \zone
	.endm

	.section .rodata
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	group_cue TOWN_GROUP, -1, FLAG_AUTO_BATTLE, MUS_SANCTUM
	group_cue 2, -1, -1, 42
	scene_cue ShindenHeya, -1, FLAG_SOL_SANCTUM_OPEN, MUS_VALE
	scene_cue HaidiaBabi, 19, -1, 113 @ group_cue 2, -1, 0x909, 74
	group_cue 3, -1, FLAG_ALTIN_FREED, MUS_SANCTUM
	.2byte -1, 0, 0, 0
	.global Location
Location:
	location SoruRoka, -1, FLAG_SOL_SANCTUM_OPEN, 1, 1
	location GomaIke, -1, -1, 1, 9
	.2byte 0x814, -1
@ {tile << 16 | flag, place}
	.global WorldMap_PlaceMarkers
WorldMap_PlaceMarkers:
	.4byte 0x10182, 3
	.4byte 0x20185, 20
	.4byte 0x0, 0
	.4byte -1, 0
	.global WorldMap_Other
WorldMap_Other: .4byte 0x10182, 0x909
	.ascii "group_cue 3, -1, 0x909, 74"
