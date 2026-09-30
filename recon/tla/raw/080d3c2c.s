.syntax unified
	.thumb
	.global Func_080d3c2c
	.thumb_func
Func_080d3c2c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #108]
	movs r5, #1
	ldr r2, [r4, #52]
	negs r5, r5
	movs r1, #8
	cmp r2, #0
	beq .L_080d3c5a
	adds r3, r2, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d3c5a
	ldr r3, [r2, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_080d3c5a
	movs r5, #8
	b .L_080d3c82
.L_080d3c5a:
	adds r1, #1
	cmp r1, #79
	bgt .L_080d3c82
	lsls r3, r1, #2
	adds r3, #20
	ldr r2, [r4, r3]
	cmp r2, #0
	beq .L_080d3c5a
	adds r3, r2, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d3c5a
	ldr r3, [r2, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_080d3c5a
	adds r5, r1, #0
.L_080d3c82:
	adds r0, r5, #0
	pop {r5, pc}
	.2byte 0x0000
