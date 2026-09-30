.syntax unified
	.thumb
	.global Func_080afeb0
	.thumb_func
Func_080afeb0:
	push {lr}
	ldr r1, .L_080afecc
	ldr r2, .L_080afed0
	ldr r3, [r1, #16]
	adds r3, r3, r0
	cmp r3, r2
	ble .L_080afec0
	adds r3, r2, #0
.L_080afec0:
	cmp r3, #0
	bge .L_080afec6
	movs r3, #0
.L_080afec6:
	str r3, [r1, #16]
	adds r0, r3, #0
	pop {pc}
.L_080afecc:
	.4byte gPartyState
.L_080afed0:
	.4byte 0x000f423f
