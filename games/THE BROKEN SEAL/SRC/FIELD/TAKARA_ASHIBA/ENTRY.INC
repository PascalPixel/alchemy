	.irp EntryTarget, 0x02009f79, 0x020089f1, 0x02008a49, 0x02008a51, 0x02008ee1, 0x02008a45
	overlay_veneer \EntryTarget
	.endr
