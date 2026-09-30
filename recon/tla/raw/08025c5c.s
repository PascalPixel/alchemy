.syntax unified
	.thumb
	.global Script_SetOrCompareHalfword20
	.thumb_func
Script_SetOrCompareHalfword20:
	push {lr}
	adds r4, r2, #0
	cmp r1, #0
	bne .L_08025c68
	strh r4, [r0, #32]
	b .L_08025c88
.L_08025c68:
	cmp r1, #1
	bne .L_08025c74
	ldrh r3, [r0, #32]
	adds r3, r3, r4
	strh r3, [r0, #32]
	b .L_08025c88
.L_08025c74:
	ldrh r2, [r0, #32]
	lsls r3, r4, #16
	asrs r3, r3, #16
	movs r1, #0
	cmp r2, r3
	bne .L_08025c82
	movs r1, #1
.L_08025c82:
	adds r3, r0, #0
	adds r3, #87
	strb r1, [r3]
.L_08025c88:
	pop {pc}
	.2byte 0x0000
