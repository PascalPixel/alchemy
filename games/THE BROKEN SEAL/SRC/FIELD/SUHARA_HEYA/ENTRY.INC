	.irp EntryTarget, 0x02008a79, 0x02008031, 0x02008041, 0x02008049, 0x02008a55, 0x02008039
	overlay_veneer \EntryTarget
	.endr
