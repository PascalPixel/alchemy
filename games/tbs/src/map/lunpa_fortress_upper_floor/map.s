	.include "games/tbs/include/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_LunpaFortressUpperFloor
Field_LunpaFortressUpperFloor:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_CELL_PICTURES
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_SHAPES
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_GRID
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_ANIMATION
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_BLEND
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_MARKERS
.LField_LunpaFortressUpperFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa_fortress_upper_floor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_GRID:
	.incbin "games/tbs/src/map/lunpa_fortress_upper_floor/map_ja.grid.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_BLEND:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/blend.script.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_CELL_PICTURES
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_SHAPES
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_GRID
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_ANIMATION
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_BLEND
	map_offset Field_LunpaFortressUpperFloor, .LField_LunpaFortressUpperFloor_MARKERS
.LField_LunpaFortressUpperFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa_fortress_upper_floor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_GRID:
	.incbin "games/tbs/src/map/lunpa_fortress_upper_floor/map.grid.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_BLEND:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/blend.script.lz"
	.balign 4, 0
.LField_LunpaFortressUpperFloor_MARKERS:
	treasure_spots_end
	.endif
	.balign 4, 0
