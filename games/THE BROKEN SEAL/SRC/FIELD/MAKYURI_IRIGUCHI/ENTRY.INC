	.irp EntryTarget, 0x020092e1, 0x02008e91, 0x02008e9d, 0x02008ea5, 0x02008fd1, 0x02008e99
	overlay_veneer \EntryTarget
	.endr
