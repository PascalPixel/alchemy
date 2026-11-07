.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008708,"ax",%progbits
	.balign 4
	.global Func_02000708
	.thumb_func
Func_02000708:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	add r7, sp, #20
	movs r3, #1
	str r1, [sp, #16]
	str r3, [r7]
	movs r3, #24
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #20]
	str r3, [r7, #16]
	ldr r3, .L_0200878c
	str r2, [r7, #12]
	str r2, [r7, #8]
	str r3, [r7, #36]
	ldr r3, [sp, #16]
	movs r2, #0
	mov r11, r0
	mov r9, r2
	cmp r3, #0
	beq .L_020087cc
.L_02008744:
	bl Random16Far
	ldr r3, .L_02008788
	ands r0, r3
	lsls r0, r0, #12
	strh r0, [r7, #32]
	bl Random16Far
	mov r8, r0
	movs r2, #15
	mov r3, r8
	ands r3, r2
	mov r10, r2
	mov r8, r3
	subs r2, #23
	add r8, r2
	mov r3, r8
	lsls r3, r3, #14
	mov r8, r3
	bl Random16Far
	mov r2, r10
	adds r5, r0, #0
	ands r5, r2
	bl Random16Far
	mov r2, r11
	ldr r6, [r2]
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	subs r6, r6, r3
	b .L_02008790
	.2byte 0x0000
.L_02008788:
	.4byte 0x0000000f
.L_0200878c:
	.4byte Func_020006d4
.L_02008790:
	movs r3, #240
	lsls r3, r3, #12
	adds r6, r6, r3
	bl Random16Far
	mov r3, r11
	mov r2, r10
	ldr r1, [r3, #4]
	ands r0, r2
	ldr r2, [r3, #8]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #175
	lsls r0, r0, #16
	lsls r3, r3, #17
	adds r5, #8
	adds r1, r1, r0
	str r3, [sp, #8]
	lsls r5, r5, #14
	mov r3, r8
	adds r0, r6, #0
	str r5, [sp, #0]
	str r7, [sp, #12]
	bl Func_020000b8
	ldr r3, [sp, #16]
	movs r2, #1
	add r9, r2
	cmp r9, r3
	bne .L_02008744
.L_020087cc:
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020091de,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_02003ccc
Data_02003ccc:
	.space 0x00000004
	.global gData_02003cd0
gData_02003cd0:
