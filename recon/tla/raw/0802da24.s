.syntax unified
	.thumb
	.global Func_0802da24
	.thumb_func
Func_0802da24:
	push {lr}
	ldr r3, [r0]
	cmp r3, #0
	bge .L_0802da34
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_0802da34:
	ldr r2, .L_0802da84
	asrs r1, r3, #16
	ldr r3, [r0, #8]
	ands r3, r2
	ldr r2, [r0, #4]
	subs r3, r3, r2
	lsrs r0, r3, #16
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	cmp r3, #0
	bne .L_0802da50
	movs r0, #0
	b .L_0802da80
.L_0802da50:
	movs r2, #212
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [r3]
	adds r3, r1, #0
	cmp r3, #0
	bge .L_0802da60
	adds r3, #15
.L_0802da60:
	asrs r1, r3, #4
	cmp r0, #0
	bge .L_0802da68
	adds r0, #15
.L_0802da68:
	asrs r3, r0, #4
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	movs r2, #255
	eors r3, r2
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	subs r0, #1
.L_0802da80:
	pop {pc}
	.2byte 0x0000
.L_0802da84:
	.4byte 0xfff00000
