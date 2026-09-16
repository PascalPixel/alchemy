	.irp EntryTarget, 0x0200c0b5, 0x02008031, 0x02008069, 0x02008071, 0x0200c035, 0x02008061
	overlay_veneer \EntryTarget
	.endr
