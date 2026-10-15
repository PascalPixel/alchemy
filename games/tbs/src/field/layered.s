	.section .rodata
@ 場面ごとの重ねマップ (ResourceId_Map001 からの番号)
	.global gMap_LayeredScenes
gMap_LayeredScenes:
	.incbin "games/tbs/src/field/layered.table"

	.global Map_LayeredScenesTail
Map_LayeredScenesTail:
	.4byte 0, 3, 7, 9
