	.irp EntryTarget, 0x02008309, 0x02008031, 0x02008075, 0x0200807d, 0x020080e5, 0x02008071
	overlay_veneer \EntryTarget
	.endr
