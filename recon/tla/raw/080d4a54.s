.syntax unified
	.thumb
	.global Func_080d4a54
	.thumb_func
Func_080d4a54:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_080d4a9e
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r1, #16]
	ldr r3, [r5, #16]
	ldr r1, [r1, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl ArcTan2
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_080d4a9e
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_080d4a92
	adds r0, r2, #0
.L_080d4a92:
	ldr r2, .L_080d4aa4
	cmp r0, r2
	bge .L_080d4a9a
	adds r0, r2, #0
.L_080d4a9a:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_080d4a9e:
	movs r0, #1
	pop {r5, pc}
	.2byte 0x0000
.L_080d4aa4:
	.4byte 0xfffff000
