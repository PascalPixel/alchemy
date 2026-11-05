.syntax unified
	.thumb
	.section .text.x02008244,"ax",%progbits
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #28
	movs r4, #0
	movs r3, #0
	str r4, [sp, #24]
	str r4, [sp, #20]
	mov r9, r3
	ldr r3, .L_02008314
	movs r5, #128
	lsls r5, r5, #8
	add r4, sp, #12
	mov r8, r0
	adds r6, r1, #0
	mov r10, r2
	ands r5, r0
	add r2, sp, #20
	ldr r0, [r3]
	add r1, sp, #24
	add r3, sp, #16
	str r4, [sp, #0]
	bl Text_GetMessageSize
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r2, #24
	bgt .L_020082a4
	cmp r5, #0
	bne .L_02008292
	movs r3, #25
	subs r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r3, #5
	b .L_0200829c
.L_02008292:
	movs r3, #30
	subs r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
.L_0200829c:
	str r3, [sp, #24]
	ldr r3, [sp, #24]
	subs r4, r3, #5
	b .L_020082b2
.L_020082a4:
	movs r3, #30
	subs r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #24]
	adds r4, r3, #0
.L_020082b2:
	ldr r3, [sp, #12]
	subs r3, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r2, r3, #1
	cmp r6, #0
	bne .L_020082c8
	movs r3, #3
	str r3, [sp, #20]
	adds r7, r2, #1
	b .L_020082f2
.L_020082c8:
	cmp r6, #1
	bne .L_020082d8
	movs r3, #14
	subs r3, r3, r2
	str r3, [sp, #20]
	adds r3, r3, r2
	subs r7, r3, #2
	b .L_020082f2
.L_020082d8:
	cmp r6, #2
	bne .L_020082f2
	cmp r2, #3
	bne .L_020082ec
	movs r3, #13
	str r3, [sp, #20]
	movs r3, #2
	movs r7, #15
	mov r9, r3
	b .L_020082f2
.L_020082ec:
	movs r3, #16
	str r3, [sp, #20]
	movs r7, #15
.L_020082f2:
	mov r3, r10
	str r3, [sp, #4]
	mov r3, r9
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	str r3, [sp, #8]
	mov r0, r8
	adds r3, r4, #0
	str r7, [sp, #0]
	bl Func_02000158
	add sp, #28
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008314:
	.4byte gData_02000a28
	.section .bss,"aw",%nobits
	.balign 16
	.global Data_020008f0
Data_020008f0:
	.space 0x00000110
	.global gData_02000a00
gData_02000a00:
	.space 0x00000028
	.global gData_02000a28
gData_02000a28:
	.space 0x00000004
	.global gData_02000a2c
gData_02000a2c:
	.space 0x00000004
	.global Data_02000a30
Data_02000a30:
	.space 0x00000004
	.global gData_02000a34
gData_02000a34:
	.space 0x00000004
	.global gData_02000a38
gData_02000a38:
