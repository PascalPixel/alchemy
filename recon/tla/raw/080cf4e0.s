.syntax unified
	.thumb
	.global Func_080cf4e0
	.thumb_func
Func_080cf4e0:
	push {r5, r6, lr}
	sub sp, #12
	adds r5, r0, #0
	bl Random16
	movs r3, #100
	muls r3, r0
	lsrs r3, r3, #16
	cmp r3, #9
	bhi .L_080cf54c
	ldr r3, [r5, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r5, r5, #4
	adds r1, r0, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl Func_0801489c
	movs r0, #209
	lsls r0, r0, #1
	adds r0, #255
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_080dc10c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080cf54c
	ldr r1, .L_080cf550
	bl Object_SetCallback
	movs r1, #0
	adds r0, r5, #0
	bl Object_SetMode
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
.L_080cf54c:
	add sp, #12
	pop {r5, r6, pc}
.L_080cf550:
	.4byte Data_080f0194
