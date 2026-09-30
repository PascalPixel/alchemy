.syntax unified
	.thumb
	.global Owner_RefreshClassActions
	.thumb_func
Owner_RefreshClassActions:
	push {lr}
	bl Owner_RefreshDerivedData
	pop {pc}
