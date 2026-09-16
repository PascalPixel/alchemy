	.irp EntryTarget, 0x0200964d, 0x020080ed, 0x02008179, 0x02008181, 0x02008241, 0x02008175
	overlay_veneer \EntryTarget
	.endr
