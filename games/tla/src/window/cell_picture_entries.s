@ Why assembly: the Thumb entry of the cell picture builder: callers enter in
@ Thumb and bx pc switches to the ARM code of Tile_BuildCellPictures
@ (cell_picture.c) that follows, a mode switch inside a function.
@ The halfword after it is never run.
.syntax unified
	.thumb
	.global Tile_BuildCellPicturesCode
Tile_BuildCellPicturesCode:
	bx	pc
AlchemyUncredited_080388fe:
	.2byte	0x0200
AlchemyUncreditedEnd_080388fe:
