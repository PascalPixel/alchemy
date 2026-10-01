.syntax unified
	.thumb
	.global Func_08011bf4
	.thumb_func
Func_08011bf4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_08011cd4
	ldr r3, [r3]
	mov r10, r3
	adds r3, #176
	ldrh r3, [r3]
	movs r2, #3
	movs r1, #0
	mov lr, r1
	ands r2, r3
	sub sp, #32
	cmp lr, r2
	bcs .L_08011cc2
	mov r9, r2
.L_08011c18:
	movs r3, #44
	mov r2, lr
	muls r2, r3
	mov r1, r10
	adds r3, r2, #0
	adds r5, r1, r3
	ldrh r2, [r5, #6]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_08011cae
	movs r2, #4
	ldrsh r7, [r5, r2]
	movs r3, #10
	ldrsh r2, [r5, r3]
	subs r3, r2, r7
	lsls r3, r3, #24
	ldr r1, [r5]
	lsls r6, r2, #16
	adds r4, r5, #0
	lsrs r0, r3, #24
	lsrs r3, r6, #16
	mov r8, r1
	adds r4, #12
	cmp r0, r3
	bcs .L_08011c60
	mov r1, sp
	mov r12, r3
.L_08011c4e:
	ldrh r3, [r4]
	lsls r2, r0, #1
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	lsrs r0, r3, #24
	adds r4, #2
	cmp r0, r12
	bcc .L_08011c4e
.L_08011c60:
	lsls r1, r7, #16
	adds r7, r1, #0
	lsrs r2, r6, #16
	lsrs r3, r7, #16
	movs r0, #0
	subs r2, r2, r3
	cmp r0, r2
	bge .L_08011c86
	mov r1, sp
	mov r12, r2
.L_08011c74:
	ldrh r3, [r4]
	lsls r2, r0, #1
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	lsrs r0, r3, #24
	adds r4, #2
	cmp r0, r12
	blt .L_08011c74
.L_08011c86:
	movs r2, #128
	lsrs r4, r6, #16
	lsls r2, r2, #24
	ldr r3, .L_08011cd8
	mov r0, sp
	mov r1, r8
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #9
	adds r1, r7, r2
	lsrs r3, r1, #16
	cmp r3, r4
	bcc .L_08011ca6
	movs r1, #0
.L_08011ca6:
	lsrs r3, r1, #16
	strh r3, [r5, #4]
	ldrh r3, [r5, #8]
	b .L_08011cb2
.L_08011cae:
	ldr r1, .L_08011cdc
	adds r3, r2, r1
.L_08011cb2:
	strh r3, [r5, #6]
	mov r3, lr
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov lr, r3
	cmp lr, r9
	bcc .L_08011c18
.L_08011cc2:
	add sp, #32
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_08011cd4:
	.4byte gPaletteWork
.L_08011cd8:
	.4byte 0x040000d4
.L_08011cdc:
	.4byte 0x0000ffff
