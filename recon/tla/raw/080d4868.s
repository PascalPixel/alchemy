.syntax unified
	.thumb
	.global Func_080d4868
	.thumb_func
Func_080d4868:
	push {lr}
	cmp r0, #0
	beq .L_080d4884
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d4884
	ldr r0, [r0, #80]
	cmp r0, #0
	beq .L_080d4884
	ldr r0, [r0, #40]
	cmp r0, #0
	bne .L_080d4888
.L_080d4884:
	movs r0, #1
	b .L_080d4898
.L_080d4888:
	movs r2, #0
	ldrsh r3, [r0, r2]
	movs r2, #132
	lsls r2, r2, #1
	eors r3, r2
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
.L_080d4898:
	pop {pc}
	.2byte 0x0000
