.syntax unified
	.thumb
	.global Func_080d5930
	.thumb_func
Func_080d5930:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	movs r2, #0
	str r2, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	mov r10, r0
	movs r0, #0
	cmp r3, #0
	beq .L_080d59ca
	adds r7, r3, #0
	ldr r2, .L_080d59d8
	movs r3, #128
	lsls r3, r3, #21
	str r3, [sp, #0]
	movs r3, #127
	adds r7, #236
	mov r11, r2
	mov r9, r3
.L_080d5966:
	movs r3, #16
	ldrsb r3, [r7, r3]
	cmp r3, #1
	bne .L_080d59ba
	mov r2, r10
	ldr r1, [r2]
	ldr r3, [r7, #4]
	ldr r5, [r2, #4]
	subs r1, r1, r3
	ldr r3, [r7, #8]
	ldr r6, [r2, #8]
	subs r5, r5, r3
	ldr r3, [r7, #12]
	asrs r1, r1, #8
	adds r0, r1, #0
	subs r6, r6, r3
	mov lr, r11
	.2byte 0xf800
	asrs r5, r5, #8
	adds r1, r5, #0
	mov r8, r0
	adds r0, r5, #0
	mov lr, r11
	.2byte 0xf800
	asrs r6, r6, #8
	adds r5, r0, #0
	adds r1, r6, #0
	adds r0, r6, #0
	mov lr, r11
	.2byte 0xf800
	add r8, r5
	mov r2, r8
	adds r3, r2, r0
	movs r2, #128
	lsls r2, r2, #3
	cmp r3, r2
	bgt .L_080d59ba
	ldr r2, [sp, #0]
	cmp r2, r3
	ble .L_080d59ba
	str r3, [sp, #0]
	str r7, [sp, #4]
.L_080d59ba:
	movs r3, #1
	negs r3, r3
	add r9, r3
	mov r2, r9
	adds r7, #32
	cmp r2, #0
	bge .L_080d5966
	ldr r0, [sp, #4]
.L_080d59ca:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d59d8:
	.4byte IwramMulQ16
