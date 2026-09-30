.syntax unified
	.thumb
	.global Func_080dfb88
	.thumb_func
Func_080dfb88:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	bl Random16
	ldrh r6, [r7, #6]
	movs r1, #128
	lsls r1, r1, #10
	adds r5, r0, #0
	adds r0, r6, #0
	adds r5, r5, r1
	bl Trig_Cos
	ldr r2, .L_080dfc34
	adds r1, r0, #0
	mov r8, r2
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	mov r10, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7, #8]
	movs r1, #255
	add r3, r10
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	lsls r1, r1, #8
	adds r3, r3, r0
	str r3, [r7, #16]
	ldrh r3, [r7, #6]
	adds r1, #240
	adds r3, r3, r1
	strh r3, [r7, #6]
	adds r5, r7, #0
	adds r5, #102
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #0
	beq .L_080dfbf8
	subs r3, r2, #1
	strh r3, [r5]
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	strh r3, [r7, #6]
	b .L_080dfc10
.L_080dfbf8:
	bl Random16
	lsls r0, r0, #5
	lsrs r0, r0, #16
	cmp r0, #0
	bne .L_080dfc10
	bl Random16
	lsls r0, r0, #4
	lsrs r0, r0, #16
	adds r0, #8
	strh r0, [r5]
.L_080dfc10:
	adds r2, r7, #0
	adds r2, #100
	ldrh r3, [r2]
	movs r1, #202
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #15
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_080dfc2c
	ldr r1, .L_080dfc38
	adds r0, r7, #0
	bl Object_SetCallback
.L_080dfc2c:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080dfc34:
	.4byte IwramMulQ16
.L_080dfc38:
	.4byte Data_080f0e54
