	.section .rodata
@ 場面ごとの重ねマップ (ResourceId_Map000 からの番号): マップ、パレット、タイル四枚
@ The Broken Seal: gMap_LayeredScenes
	.global gData_0802f380
gData_0802f380:
	.incbin "games/tla/src/field/common/map/layered.table"
