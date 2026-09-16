	.irp EntryTarget, 0x020093d5, 0x02008939, 0x02008949, 0x02008951, 0x020089fd, 0x02008941
	overlay_veneer \EntryTarget
	.endr
