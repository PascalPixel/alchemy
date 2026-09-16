	.irp EntryTarget, 0x0200904d, 0x02008045, 0x0200807d, 0x02008085, 0x020084b1, 0x0200804d
	overlay_veneer \EntryTarget
	.endr
