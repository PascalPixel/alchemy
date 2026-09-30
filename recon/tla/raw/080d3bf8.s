.syntax unified
	.thumb
	.global ObjectTable_ReadActiveValue
	.thumb_func
ObjectTable_ReadActiveValue:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r3, r0
	lsls r3, r3, #2
	adds r3, #20
	ldr r2, [r2, r3]
	movs r1, #1
	negs r1, r1
	cmp r2, #0
	beq .L_080d3c28
	adds r3, r2, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d3c28
	ldr r3, [r2, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
.L_080d3c28:
	adds r0, r1, #0
	pop {pc}
