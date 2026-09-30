.syntax unified
	.thumb
	.global Func_080d1e84
	.thumb_func
Func_080d1e84:
	push {lr}
	bl ObjectTable_Get
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d1ea0
	ldr r0, [r0, #80]
	cmp r0, #0
	beq .L_080d1ea0
	ldr r0, [r0, #40]
	cmp r0, #0
	bne .L_080d1ea4
.L_080d1ea0:
	movs r0, #0
	b .L_080d1ea8
.L_080d1ea4:
	movs r3, #0
	ldrsh r0, [r0, r3]
.L_080d1ea8:
	pop {pc}
	.2byte 0x0000
