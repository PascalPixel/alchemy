	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_LunpaFortressGroundFloor
Field_LunpaFortressGroundFloor:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LunpaFortressGroundFloor, .LField_LunpaFortressGroundFloor_CELL_PICTURES
	map_offset Field_LunpaFortressGroundFloor, .LField_LunpaFortressGroundFloor_SHAPES
	map_offset Field_LunpaFortressGroundFloor, .LField_LunpaFortressGroundFloor_GRID
	map_offset Field_LunpaFortressGroundFloor, .LField_LunpaFortressGroundFloor_ANIMATION
	map_offset Field_LunpaFortressGroundFloor, .LField_LunpaFortressGroundFloor_BLEND
	map_offset Field_LunpaFortressGroundFloor, .LField_LunpaFortressGroundFloor_MARKERS
.LField_LunpaFortressGroundFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LunpaFortressGroundFloor_SHAPES:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/shapes.table.lz"
	.balign 4, 0
.LField_LunpaFortressGroundFloor_GRID:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/map.grid.lz"
	.balign 4, 0
.LField_LunpaFortressGroundFloor_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_LunpaFortressGroundFloor_BLEND:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/blend.script.lz"
	.balign 4, 0
.LField_LunpaFortressGroundFloor_MARKERS:
	treasure_spot 29, 24, 104
	treasure_spot 47, 52, 100
	treasure_spot 49, 53, 101
	treasure_spot 46, 54, 103
	treasure_spot 48, 55, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteLunpaFortress
Tileset_PaletteLunpaFortress:
	.incbin "games/tbs/src/graphics/tileset/palettes/lunpa_fortress.gbapal.lz"
