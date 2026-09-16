	.irp EntryTarget, 0x02008729, 0x02008209, 0x02008215, 0x0200821d, 0x020084b1, 0x02008211
	overlay_veneer \EntryTarget
	.endr
