	.irp EntryTarget, 0x0200920d, 0x02008041, 0x0200804d, 0x02008055, 0x020091e9, 0x02008049
	overlay_veneer \EntryTarget
	.endr
