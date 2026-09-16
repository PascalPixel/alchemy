	.irp EntryTarget, 0x02009a0d, 0x02008031, 0x0200803d, 0x02008045, 0x02008075, 0x02008039
	overlay_veneer \EntryTarget
	.endr
