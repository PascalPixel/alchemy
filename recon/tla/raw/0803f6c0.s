.syntax unified
	.thumb
	.global UiTextResource_Release
	.thumb_func
UiTextResource_Release:
	push {lr}
	bl Resource_ResetEntry
	pop {pc}
