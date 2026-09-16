	.irp EntryTarget, 0x020081d9, 0x02008031, 0x0200803d, 0x02008045, 0x020081a9, 0x02008039
	overlay_veneer \EntryTarget
	.endr
