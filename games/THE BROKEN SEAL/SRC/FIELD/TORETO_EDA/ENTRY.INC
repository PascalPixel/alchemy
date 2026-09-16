	.irp EntryTarget, 0x02008201, 0x02008041, 0x0200804d, 0x02008055, 0x020081b5, 0x02008049
	overlay_veneer \EntryTarget
	.endr
