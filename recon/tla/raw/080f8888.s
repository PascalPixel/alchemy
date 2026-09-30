.syntax unified
	.thumb
	.global UiIcon_PrepareObject
	.thumb_func
UiIcon_PrepareObject:
	push {lr}
	cmp r0, #0
	beq .L_080f88be
	movs r3, #1
	strb r3, [r0, #5]
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	ldrh r1, [r0, #22]
	ldr r3, .L_080f88c0
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #22]
	ldrh r3, [r0, #8]
	ldrb r2, [r0, #23]
	strb r3, [r0, #20]
	movs r3, #63
	negs r3, r3
	ands r3, r2
	ldrb r2, [r0, #21]
	strb r3, [r0, #23]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	strb r3, [r0, #21]
.L_080f88be:
	pop {pc}
.L_080f88c0:
	.4byte 0xfffffe00
