.syntax unified
	.thumb
	.global Func_080d36c8
	.thumb_func
Func_080d36c8:
	push {r5, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080d36ec
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_080d36e4
	ldr r3, .L_080d36f0
	str r3, [r0, #108]
	b .L_080d36ec
.L_080d36e4:
	str r3, [r0, #108]
	adds r1, r5, #0
	bl Object_SetPartAttribute
.L_080d36ec:
	pop {r5, pc}
	.2byte 0x0000
.L_080d36f0:
	.4byte Func_080d36f4
