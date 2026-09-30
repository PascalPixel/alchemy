.syntax unified
	.thumb
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	cmp r0, #191
	bls .L_080cad94
	movs r0, #0
	b .L_080cad9a
.L_080cad94:
	lsls r3, r0, #2
	adds r3, #20
	ldr r0, [r2, r3]
.L_080cad9a:
	pop {pc}
