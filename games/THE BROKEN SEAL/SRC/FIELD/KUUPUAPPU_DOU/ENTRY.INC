	.irp EntryTarget, 0x02008ca1, 0x02008031, 0x0200808d, 0x020080e1, 0x020082e1, 0x02008085
	overlay_veneer \EntryTarget
	.endr
