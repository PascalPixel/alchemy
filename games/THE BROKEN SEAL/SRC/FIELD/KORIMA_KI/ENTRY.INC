	.irp EntryTarget, 0x02009071, 0x02008031, 0x02008041, 0x02008049, 0x02008081, 0x02008039
	overlay_veneer \EntryTarget
	.endr
