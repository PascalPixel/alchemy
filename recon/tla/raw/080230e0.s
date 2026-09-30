.syntax unified
	.thumb
	.global Func_080230e0
	.thumb_func
Func_080230e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r1, #92
	movs r0, #24
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r1, #128
	lsls r1, r1, #6
	mov r8, r0
	movs r0, #20
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_08022bd8
	movs r3, #128
	mov r4, sp
	movs r5, #0
	lsls r3, r3, #19
	str r5, [r4]
	adds r3, #212
	adds r0, r4, #0
	adds r1, r6, #0
	ldr r2, .L_08023184
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #133
	lsls r2, r2, #24
	str r5, [r4]
	adds r0, r4, #0
	mov r1, r8
	adds r2, #23
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	cmp r7, #4
	bne .L_0802313e
	movs r1, #128
	lsls r1, r1, #3
	ldr r0, .L_08023188
	adds r1, #138
	bl Func_080145a8
	b .L_08023156
.L_0802313e:
	movs r1, #227
	lsls r1, r1, #2
	adds r1, #255
	ldr r0, .L_0802318c
	bl Func_080145a8
	movs r1, #128
	lsls r1, r1, #3
	ldr r0, .L_08023190
	adds r1, #138
	bl Func_080145a8
.L_08023156:
	subs r3, r7, #3
	cmp r3, #1
	bhi .L_08023168
	movs r1, #144
	ldr r0, .L_08023194
	lsls r1, r1, #3
	bl Func_080145a8
	b .L_08023172
.L_08023168:
	movs r1, #144
	ldr r0, .L_08023198
	lsls r1, r1, #3
	bl Func_080145a8
.L_08023172:
	mov r1, r8
	movs r2, #0
	movs r3, #15
	strb r3, [r1, #6]
	strb r2, [r1, #7]
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08023184:
	.4byte 0x85000800
.L_08023188:
	.4byte Func_0802493c
.L_0802318c:
	.4byte Func_08023f9c
.L_08023190:
	.4byte Func_080246b8
.L_08023194:
	.4byte Func_08023e18
.L_08023198:
	.4byte Func_0802386c
	.4byte 0x00004770
	.4byte 0x00004770
