	.irp EntryTarget, 0x0200871d, 0x02008465, 0x02008471, 0x02008479, 0x02008715, 0x0200846d
	overlay_veneer \EntryTarget
	.endr
