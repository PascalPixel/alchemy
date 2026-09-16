	.irp EntryTarget, 0x020092e1, 0x02008031, 0x02008059, 0x02008061, 0x020091b5, 0x02008039
	overlay_veneer \EntryTarget
	.endr
