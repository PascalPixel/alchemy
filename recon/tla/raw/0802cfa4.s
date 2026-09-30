.syntax unified
	.thumb
	.global Func_0802cfa4
	.thumb_func
Func_0802cfa4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #112]
	movs r2, #3
	mov r10, r3
	adds r3, #176
	ldrh r3, [r3]
	movs r1, #0
	mov lr, r1
	ands r2, r3
	sub sp, #32
	cmp lr, r2
	bcs .L_0802d07c
	mov r9, r2
.L_0802cfca:
	movs r3, #44
	mov r2, lr
	muls r2, r3
	mov r1, r10
	adds r3, r2, #0
	adds r5, r1, r3
	ldrh r2, [r5, #6]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0802d064
	movs r2, #4
	ldrsh r7, [r5, r2]
	movs r3, #10
	ldrsh r2, [r5, r3]
	ldr r1, [r5]
	subs r3, r2, r7
	lsls r3, r3, #24
	lsls r6, r2, #16
	adds r4, r5, #0
	lsrs r0, r3, #24
	lsrs r3, r6, #16
	mov r8, r1
	adds r4, #12
	cmp r0, r3
	bcs .L_0802d012
	mov r1, sp
	mov r12, r3
.L_0802d000:
	ldrh r3, [r4]
	lsls r2, r0, #1
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	lsrs r0, r3, #24
	adds r4, #2
	cmp r0, r12
	bcc .L_0802d000
.L_0802d012:
	lsls r1, r7, #16
	adds r7, r1, #0
	lsrs r2, r6, #16
	lsrs r3, r7, #16
	movs r0, #0
	subs r2, r2, r3
	cmp r0, r2
	bge .L_0802d038
	mov r1, sp
	mov r12, r2
.L_0802d026:
	ldrh r3, [r4]
	lsls r2, r0, #1
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	lsrs r0, r3, #24
	adds r4, #2
	cmp r0, r12
	blt .L_0802d026
.L_0802d038:
	movs r2, #128
	movs r3, #128
	lsrs r4, r6, #16
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
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
	bcc .L_0802d05c
	movs r1, #0
.L_0802d05c:
	lsrs r3, r1, #16
	strh r3, [r5, #4]
	ldrh r3, [r5, #8]
	b .L_0802d06c
.L_0802d064:
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
.L_0802d06c:
	strh r3, [r5, #6]
	mov r3, lr
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov lr, r3
	cmp lr, r9
	bcc .L_0802cfca
.L_0802d07c:
	add sp, #32
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
