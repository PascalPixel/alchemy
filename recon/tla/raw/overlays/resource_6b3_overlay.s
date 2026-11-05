.syntax unified
	.thumb
	.section .text.x02008098,"ax",%progbits
	.balign 4
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	sub sp, #4
	bl Heap_Alloc
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r8, r0
	adds r3, #212
	ldr r0, .L_02008130
	ldr r1, .L_02008134
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_02008138
	ldr r6, .L_0200813c
	bl Resource_GetTableEntry
	mov r1, r8
	bl Func_020007ac
	ldr r5, .L_02008140
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r8
	str r0, [r5]
	lsls r1, r1, #4
	ldr r5, .L_02008144
	bl VramCache_Load
	movs r3, #192
	str r0, [r5]
	movs r1, #0
	str r0, [sp, #0]
	movs r2, #0
	adds r0, r6, #0
	lsls r3, r3, #24
	bl Func_02000834
	movs r3, #240
	strh r3, [r6, #30]
	ldrb r3, [r6, #9]
	movs r2, #13
	ldrb r1, [r6, #5]
	negs r2, r2
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	movs r3, #224
	orrs r2, r3
	strb r2, [r6, #9]
	mov r0, r8
	bl Heap_Release
	ldr r5, .L_0200812c
	ldr r3, .L_02008148
	movs r1, #144
	strb r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200814c
	bl Task_Add
	add sp, #4
	b .L_02008150
.L_0200812c:
	.4byte 0x00000000
.L_02008130:
	.4byte Data_02000874
.L_02008134:
	.4byte 0x050003c0
.L_02008138:
	.4byte 0x000001f8
.L_0200813c:
	.4byte gData_02000a00
.L_02008140:
	.4byte Data_020008f0
.L_02008144:
	.4byte Data_02000a30
.L_02008148:
	.4byte gData_02000a34
.L_0200814c:
	.4byte Func_0200005c
.L_02008150:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
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
