.syntax unified
	.thumb
	.global Func_080b0144
	.thumb_func
Func_080b0144:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r2
	movs r2, #1
	adds r7, r0, #0
	negs r2, r2
	sub sp, #16
	mov r11, r2
	movs r0, #0
	cmp r7, #7
	ble .L_080b0166
	b .L_080b0270
.L_080b0166:
	mov r5, sp
	adds r0, r7, #0
	adds r2, r5, #0
	bl Owner_GetDigitValues
	mov r12, r11
	mov r0, r11
	movs r4, #0
	adds r2, r5, #0
.L_080b0178:
	ldmia r2!, {r3}
	cmp r12, r3
	bge .L_080b0182
	mov r12, r3
	adds r0, r4, #0
.L_080b0182:
	adds r4, #1
	cmp r4, #3
	ble .L_080b0178
	movs r1, #1
	negs r1, r1
	mov r12, r1
	movs r4, #0
	adds r2, r5, #0
.L_080b0192:
	cmp r4, r0
	beq .L_080b01a0
	ldr r3, [r2]
	cmp r12, r3
	bge .L_080b01a0
	mov r12, r3
	adds r1, r4, #0
.L_080b01a0:
	adds r4, #1
	adds r2, #4
	cmp r4, #3
	ble .L_080b0192
	lsls r3, r1, #2
	ldr r3, [r5, r3]
	cmp r3, #9
	bgt .L_080b01b2
	adds r1, r0, #0
.L_080b01b2:
	ldr r2, .L_080b0280
	lsls r3, r0, #2
	adds r3, r3, r1
	lsls r3, r3, #2
	ldr r6, [r2, r3]
	cmp r6, #2
	bne .L_080b01c8
	cmp r7, #5
	bne .L_080b01c8
	movs r6, #13
	b .L_080b01d2
.L_080b01c8:
	cmp r6, #6
	bne .L_080b01d2
	cmp r7, #7
	bne .L_080b01d2
	movs r6, #14
.L_080b01d2:
	movs r3, #222
	lsls r3, r3, #1
	cmp r8, r3
	bne .L_080b01de
	movs r6, #16
	b .L_080b01f8
.L_080b01de:
	movs r1, #188
	adds r1, #255
	cmp r8, r1
	bne .L_080b01ee
	movs r6, #15
	b .L_080b01f8
.L_080b01ea:
	mov r11, r4
	b .L_080b0262
.L_080b01ee:
	movs r2, #190
	adds r2, #255
	cmp r8, r2
	bne .L_080b01f8
	movs r6, #17
.L_080b01f8:
	ldr r3, .L_080b0284
	movs r1, #158
	mov r10, r3
	lsls r1, r1, #7
	mov r8, r10
	adds r1, #192
	movs r7, #158
	mov r12, r5
	add r1, r8
	lsls r7, r7, #7
	movs r4, #243
	mov r9, r12
	mov lr, r1
	adds r7, #188
.L_080b0214:
	mov r2, r10
	ldr r3, [r7, r2]
	cmp r3, r6
	bne .L_080b0254
	mov r1, lr
	ldrb r3, [r1]
	mov r1, r12
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, [r1]
	lsls r2, r2, #1
	movs r5, #0
	cmp r3, r2
	blt .L_080b0250
	mov r2, r8
	adds r3, r7, r2
	mov r0, r9
	adds r1, r3, #4
.L_080b0238:
	adds r5, #1
	cmp r5, #3
	bgt .L_080b0250
	adds r1, #1
	ldrb r3, [r1]
	adds r0, #4
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, [r0]
	lsls r2, r2, #1
	cmp r3, r2
	bge .L_080b0238
.L_080b0250:
	cmp r5, #4
	beq .L_080b01ea
.L_080b0254:
	movs r3, #84
	negs r3, r3
	subs r4, #1
	add lr, r3
	subs r7, #84
	cmp r4, #0
	bge .L_080b0214
.L_080b0262:
	movs r1, #1
	negs r1, r1
	cmp r11, r1
	bne .L_080b026e
	movs r2, #0
	mov r11, r2
.L_080b026e:
	mov r0, r11
.L_080b0270:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080b0280:
	.4byte Data_080c6604
.L_080b0284:
	.4byte Data_080c15f4
