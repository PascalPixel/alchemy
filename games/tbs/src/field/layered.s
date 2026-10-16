	.section .rodata
@ Layered maps by scene, indexed from ResourceId_Map001
	.global gMap_LayeredScenes
gMap_LayeredScenes:
	.incbin "games/tbs/src/field/layered.table"

	.global Map_LayeredScenesTail
Map_LayeredScenesTail:
	.4byte 0, 3, 7, 9
