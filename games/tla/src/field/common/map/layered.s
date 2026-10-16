	.section .rodata
@ Scene overlay maps: map, palette, four tiles; indices relative to ResourceId_Map000.
@ The Broken Seal: gMap_LayeredScenes
	.global gData_0802f380
gData_0802f380:
	.incbin "games/tla/src/field/common/map/layered.table"
