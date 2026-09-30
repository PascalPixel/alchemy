.syntax unified
	.thumb
	.global Func_080dfc3c
	.thumb_func
Func_080dfc3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	ldr r0, [r7, #104]
	ldrh r5, [r7, #6]
	movs r2, #128
	lsls r2, r2, #12
	mov r9, r0
	adds r0, r5, #0
	mov r8, r2
	bl Trig_Cos
	ldr r6, .L_080dfccc
	adds r1, r0, #0
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	mov r10, r0
	adds r0, r5, #0
	bl Trig_Sin
	adds r1, r0, #0
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	mov r2, r9
	ldr r3, [r2, #8]
	movs r1, #0
	add r3, r10
	str r3, [r7, #8]
	ldr r3, [r2, #16]
	adds r2, r7, #0
	adds r3, r3, r0
	str r3, [r7, #16]
	ldrh r3, [r7, #6]
	movs r0, #128
	lsls r0, r0, #4
	adds r3, r3, r0
	strh r3, [r7, #6]
	adds r2, #100
	ldrh r3, [r2]
	movs r0, #242
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #15
	lsls r3, r3, #16
	cmp r3, r0
	bne .L_080dfcc2
	ldr r3, .L_080dfcd0
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #102
	strh r1, [r2]
	strh r1, [r3]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #72]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #40]
	bl Random16
	strh r0, [r7, #6]
.L_080dfcc2:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080dfccc:
	.4byte IwramMulQ16
.L_080dfcd0:
	.4byte Func_080dfb88
