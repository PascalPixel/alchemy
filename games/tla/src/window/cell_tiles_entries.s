@ Why assembly: the Thumb entry of the cell tiles builder: callers enter in
@ Thumb and bx pc switches to the ARM code of Tile_BuildCellTiles
@ (cell_tiles.c) that follows, a mode switch inside a function.
@ The halfword after it is never run.
.syntax unified
	.thumb
	.global Tile_BuildCellTilesCode
Tile_BuildCellTilesCode:
	bx	pc
AlchemyUncredited_080388fe:
	.2byte	0x0200
AlchemyUncreditedEnd_080388fe:
