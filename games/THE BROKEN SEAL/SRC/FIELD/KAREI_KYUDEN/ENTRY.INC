	.irp EntryTarget, 0x02008231, 0x02008031, 0x02008065, 0x0200806d, 0x020080a5, 0x02008061
	overlay_veneer \EntryTarget
	.endr
