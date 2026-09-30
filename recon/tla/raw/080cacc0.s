.syntax unified
	.thumb
	.global ObjectTable_FindLastActiveId
	.thumb_func
ObjectTable_FindLastActiveId:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #7
	adds r1, r3, #0
	movs r2, #8
	adds r1, #52
.L_080cacd0:
	ldmia r1!, {r3}
	cmp r3, #0
	beq .L_080cacd8
	adds r0, r2, #0
.L_080cacd8:
	adds r2, #1
	cmp r2, #63
	ble .L_080cacd0
	adds r0, #1
	cmp r0, #64
	bne .L_080cace8
	movs r0, #1
	negs r0, r0
.L_080cace8:
	pop {pc}
	.2byte 0x0000
