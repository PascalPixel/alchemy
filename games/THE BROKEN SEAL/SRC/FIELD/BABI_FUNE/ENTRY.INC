	.irp EntryTarget, 0x02008da5, 0x02008031, 0x0200803d, 0x02008045, 0x020080a9, 0x02008039
	overlay_veneer \EntryTarget
	.endr
