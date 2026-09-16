	.irp EntryTarget, 0x0200a59d, 0x02009271, 0x020092d9, 0x020092e1, 0x0200a371, 0x020092b1
	overlay_veneer \EntryTarget
	.endr
