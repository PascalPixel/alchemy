	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_SolSanctumEruption
Field_SolSanctumEruption:
	map_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumEruption, .LField_SolSanctumEruption_CELL_PICTURES
	map_offset Field_SolSanctumEruption, .LField_SolSanctumEruption_SHAPES
	map_offset Field_SolSanctumEruption, .LField_SolSanctumEruption_GRID
	map_offset Field_SolSanctumEruption, .LField_SolSanctumEruption_ANIMATION
	map_offset Field_SolSanctumEruption, .LField_SolSanctumEruption_BLEND
	map_offset Field_SolSanctumEruption, .LField_SolSanctumEruption_MARKERS
.LField_SolSanctumEruption_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_eruption/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumEruption_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_eruption/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumEruption_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_eruption/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumEruption_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_eruption/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumEruption_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_elemental_star_room/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumEruption_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_SolSanctumEruptionPalette
Tileset_SolSanctumEruptionPalette:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_eruption/palette.gbapal.lz"
	.byte 0x27, 0x50	@ PACKER: left in the buffer by Field_SolSanctumEruption+0x10a
	.global Tileset_SolSanctumEruptionTilesA
Tileset_SolSanctumEruptionTilesA:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_eruption/tiles_a.4bpp.mtf"
	.byte 0xfe, 0x4c	@ PACKER: left in the buffer by Field_SolSanctumEruption+0x1fe2
	.global Tileset_SolSanctumEruptionTilesB
Tileset_SolSanctumEruptionTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_eruption/tiles_b.4bpp.mtf"
	.byte 0xa5	@ PACKER: left in the buffer by Tileset_SolSanctumEruptionTilesA+0x1723
	.global Tileset_SolSanctumEruptionTilesC
Tileset_SolSanctumEruptionTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_eruption/tiles_c.4bpp.mtf"
	.global Tileset_SolSanctumEruptionTilesD
Tileset_SolSanctumEruptionTilesD:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_eruption/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_SolSanctumEruptionTilesC+0x13f
