.syntax unified
	.thumb
	.global BattleFx_HasReachedTarget
	.thumb_func
BattleFx_HasReachedTarget:
	push {lr}
	adds r3, r0, #0
	adds r3, #65
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080ebe84
	movs r0, #0
	b .L_080ebe92
.L_080ebe84:
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #24
	eors r3, r2
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
.L_080ebe92:
	pop {pc}
