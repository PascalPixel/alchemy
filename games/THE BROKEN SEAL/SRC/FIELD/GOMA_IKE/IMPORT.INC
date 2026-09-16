	.irp EntryTarget, 0x080091c1, 0x080091e1, 0x080770c1, 0x0808a081, 0x0808a0f1, 0x0808a101
	overlay_veneer \EntryTarget
	.endr
