.syntax unified
	.thumb
	.section .text.x0200cd48,"ax",%progbits
	.balign 4
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r2, r11
	sub sp, #8
	adds r2, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Actor_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	adds r3, r7, #0
	adds r3, #100
	mov r2, r9
	strh r2, [r3]
	movs r2, #7
	mov r10, r2
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	movs r2, #1
	mov r8, r2
	subs r3, #4
	movs r5, #2
	mov r2, r8
	strb r5, [r3]
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	ldr r0, [sp, #0]
	ldr r6, [sp, #0]
	bl Actor_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #8
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #2
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #10
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005624
	movs r2, #0
	mov r10, r2
.L_0200ce40:
	movs r3, #1
	mov r9, r3
	movs r5, #0
.L_0200ce46:
	movs r2, #4
	add r2, r11
	mov r8, r2
	mov r6, r11
	cmp r11, r8
	bge .L_0200ce7c
.L_0200ce52:
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200ce6c
	cmp r3, #7
	beq .L_0200ce6c
	movs r3, #0
	mov r9, r3
.L_0200ce6c:
	adds r2, r6, #0
	adds r0, r7, #0
	adds r1, r5, #0
	adds r6, #1
	bl Func_02004938
	cmp r6, r8
	blt .L_0200ce52
.L_0200ce7c:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #15
	ble .L_0200ce46
	mov r2, r9
	cmp r2, #0
	bne .L_0200ce98
	movs r3, #1
	add r10, r3
	mov r2, r10
	cmp r2, #127
	ble .L_0200ce40
.L_0200ce98:
	movs r0, #20
	bl wait
	ldr r3, [sp, #4]
	movs r0, #3
	cmp r3, #0
	beq .L_0200cf32
	mov r0, r11
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r6, #0
	movs r5, #0
	cmp r3, #0
	beq .L_0200ceb8
	ldr r6, [r0, #76]
.L_0200ceb8:
	ldr r0, [sp, #0]
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200ced0
	ldr r0, [r0, #76]
	cmp r6, #0
	beq .L_0200cece
	cmp r6, r0
	bls .L_0200ced0
.L_0200cece:
	adds r6, r0, #0
.L_0200ced0:
	mov r0, r11
	adds r0, #2
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cee0
	ldr r5, [r0, #76]
.L_0200cee0:
	mov r0, r11
	adds r0, #3
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cefa
	ldr r0, [r0, #76]
	cmp r5, #0
	beq .L_0200cef8
	cmp r5, r0
	bls .L_0200cefa
.L_0200cef8:
	adds r5, r0, #0
.L_0200cefa:
	cmp r6, #0
	beq .L_0200cf2a
	movs r0, #1
	cmp r5, #0
	beq .L_0200cf32
	movs r0, #0
	cmp r6, r5
	beq .L_0200cf32
	cmp r6, r5
	bcs .L_0200cf1c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl GameFlag_SetBit
	movs r0, #1
	b .L_0200cf32
.L_0200cf1c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl GameFlag_SetBit
	movs r0, #2
	b .L_0200cf32
.L_0200cf2a:
	movs r0, #2
	cmp r5, #0
	bne .L_0200cf32
	movs r0, #3
.L_0200cf32:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200d090,"ax",%progbits
	.balign 4
	.global Func_02005090
	.thumb_func
Func_02005090:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200d1f0
	movs r0, #24
	adds r0, r0, r5
	mov r9, r0
	ldr r0, .L_0200d1f4
	ldr r2, .L_0200d1f8
	ldr r3, [r0]
	sub sp, #20
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	ldr r1, [r5]
	lsrs r3, r3, #5
	ldr r7, [r5, #4]
	str r3, [sp, #8]
	movs r2, #132
	adds r2, r2, r5
	ldr r0, [r2]
	mov r10, r1
	movs r1, #32
	adds r1, r1, r5
	mov r11, r2
	adds r0, #50
	mov r8, r1
	mov r1, r9
	bl Func_02004f84
	mov r3, r11
	ldr r0, [r3]
	ldr r1, .L_0200d1fc
	adds r0, #60
	bl Func_02004f84
	mov r1, r11
	ldr r0, [r1]
	ldr r1, .L_0200d200
	bl Func_02004f84
	movs r6, #4
.L_0200d0ec:
	mov r2, r9
	ldrb r1, [r2]
	ldr r3, .L_0200d204
	lsls r1, r1, #6
	add r1, r10
	adds r1, r1, r3
	adds r0, r7, #0
	ldr r3, .L_0200d208
	movs r2, #64
	mov lr, r3
	.2byte 0xf800
	subs r6, #1
	movs r0, #1
	adds r7, #64
	add r9, r0
	cmp r6, #0
	bge .L_0200d0ec
	ldr r1, .L_0200d1f4
	ldr r2, [r5, #4]
	ldr r0, [r1]
	movs r1, #160
	lsls r1, r1, #1
	bl VramCache_Load
	add r5, sp, #12
	ldr r3, [r5, #4]
	ldr r2, .L_0200d20c
	adds r0, r5, #0
	ands r3, r2
	str r3, [r5, #4]
	ldr r3, [sp, #12]
	movs r6, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #18
	orrs r3, r2
	str r3, [sp, #12]
	bl AffineMatrix_BuildForEffect
	str r0, [sp, #4]
	mov r2, r11
	ldr r0, [r2]
	bl Func_02005020
	movs r3, #0
	strh r3, [r5, #4]
	strh r0, [r5]
	strh r0, [r5, #2]
	adds r0, r5, #0
	bl AffineMatrix_BuildForEffect
	ldr r3, .L_0200d1fc
	str r0, [sp, #0]
	ldr r7, .L_0200d200
	mov r10, r3
.L_0200d168:
	mov r1, r8
	movs r3, #0
	stmia r1!, {r3}
	movs r2, #128
	lsls r2, r2, #8
	lsls r3, r6, #20
	orrs r3, r2
	stmia r1!, {r3}
	ldr r0, [sp, #8]
	movs r3, #128
	lsls r3, r3, #3
	orrs r3, r0
	str r3, [r1]
	mov r1, r8
	ldrb r3, [r1, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r1, #5]
	mov r0, r10
	ldrb r3, [r0]
	ldrb r2, [r7]
	mov r12, r3
	ldr r5, [sp, #8]
	adds r3, r6, #1
	mov r11, r3
	movs r3, #12
	movs r1, #1
	mov r0, r8
	add r3, r8
	add r10, r1
	adds r7, #1
	ldrb r1, [r0, #7]
	adds r5, #2
	mov r9, r3
	cmp r12, r2
	bne .L_0200d1b4
	cmp r6, #2
	bne .L_0200d1b8
.L_0200d1b4:
	ldr r2, [sp, #0]
	b .L_0200d1ba
.L_0200d1b8:
	ldr r2, [sp, #4]
.L_0200d1ba:
	movs r0, #63
	movs r3, #31
	negs r0, r0
	ands r2, r3
	adds r3, r0, #0
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #7]
	mov r0, r8
	movs r1, #236
	bl Func_020055ec
	mov r6, r11
	str r5, [sp, #8]
	mov r8, r9
	cmp r6, #4
	ble .L_0200d168
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d1f0:
	.4byte gData_020023c4 + 0x288
.L_0200d1f4:
	.4byte gData_020023c4 + 0x308
.L_0200d1f8:
	.4byte gResourceTableEntries
.L_0200d1fc:
	.4byte gData_020023c4 + 0x290
.L_0200d200:
	.4byte gData_020023c4 + 0x298
.L_0200d204:
	.4byte 0xfffff400
.L_0200d208:
	.4byte IwramCopyWords
.L_0200d20c:
	.4byte 0xffff0000
	.section .bss,"aw",%nobits
	.global Data_02006fc8
Data_02006fc8:
	.space 0x00000020
	.global Data_02006fe8
Data_02006fe8:
	.space 0x00000020
	.global Data_02007008
Data_02007008:
	.space 0x00000020
	.global Data_02007028
Data_02007028:
	.space 0x00000020
	.global Data_02007048
Data_02007048:
	.space 0x00000020
	.global gData_02007068
gData_02007068:
