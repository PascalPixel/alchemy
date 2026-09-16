	.irp EntryTarget, 0x020096f1, 0x02008d21, 0x02008d79, 0x02008d81, 0x0200969d, 0x02008d75
	overlay_veneer \EntryTarget
	.endr
