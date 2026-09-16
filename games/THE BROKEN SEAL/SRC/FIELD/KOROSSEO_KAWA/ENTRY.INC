	.irp EntryTarget, 0x02008a3d, 0x02008031, 0x0200803d, 0x02008045, 0x0200896d, 0x02008039
	overlay_veneer \EntryTarget
	.endr
