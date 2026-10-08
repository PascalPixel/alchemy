.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleActor_CommitPlacementFar
	.thumb_func
BattleActor_CommitPlacementFar:
	overlay_veneer BtlObjPut
	.global ReleaseBattleObjectRecordsFar
	.thumb_func
ReleaseBattleObjectRecordsFar:
	overlay_veneer ReleaseBattleObjectRecords
	.global Func_08118128
	.thumb_func
Func_08118128:
	overlay_veneer Func_08120060
	.global Func_08118130
	.thumb_func
Func_08118130:
	overlay_veneer Func_08123574
	.global BattleParty_PrepareReserveOwnersFar
	.thumb_func
BattleParty_PrepareReserveOwnersFar:
	overlay_veneer BattleParty_PrepareReserveOwners
	.global Func_08118140
	.thumb_func
Func_08118140:
	overlay_veneer Func_0811a39c
	.global Func_08118148
	.thumb_func
Func_08118148:
	overlay_veneer Func_0811b75c
	.global Func_08118150
	.thumb_func
Func_08118150:
	overlay_veneer Func_08119734
	.global Func_08118158
	.thumb_func
Func_08118158:
	overlay_veneer Func_08118f6c
