@ The sixteen headings an actor faces (enum Facing, field/scene_tables.h), in
@ turns from east, clockwise on the screen. The header computes them, so the
@ build cannot hand them to assembly; a table's file includes this.
	.equiv FACING_EAST, 0x0000
	.equiv FACING_EAST_SOUTHEAST, 0x1000
	.equiv FACING_SOUTHEAST, 0x2000
	.equiv FACING_SOUTH_SOUTHEAST, 0x3000
	.equiv FACING_SOUTH, 0x4000
	.equiv FACING_SOUTH_SOUTHWEST, 0x5000
	.equiv FACING_SOUTHWEST, 0x6000
	.equiv FACING_WEST_SOUTHWEST, 0x7000
	.equiv FACING_WEST, 0x8000
	.equiv FACING_WEST_NORTHWEST, 0x9000
	.equiv FACING_NORTHWEST, 0xa000
	.equiv FACING_NORTH_NORTHWEST, 0xb000
	.equiv FACING_NORTH, 0xc000
	.equiv FACING_NORTH_NORTHEAST, 0xd000
	.equiv FACING_NORTHEAST, 0xe000
	.equiv FACING_EAST_NORTHEAST, 0xf000
