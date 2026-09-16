	.irp EntryTarget, 0x020088f5, 0x02008031, 0x02008075, 0x0200807d, 0x020080ed, 0x02008071
	overlay_veneer \EntryTarget
	.endr
