	.irp EntryTarget, 0x02008599, 0x02008315, 0x02008321, 0x02008329, 0x02008571, 0x0200831d
	overlay_veneer \EntryTarget
	.endr
