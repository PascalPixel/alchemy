.syntax unified
	.thumb
	.global Func_08018a04
	.thumb_func
Func_08018a04:
	lsrs r4, r4, #26
	movs r0, r0
	mov r1, r9
	ldrh r3, [r5, r1]
	subs r0, r7, r3
	subs r0, #4
	cmp r0, #0
	bge .L_08018a16
	movs r0, #0
.L_08018a16:
	mov r3, r8
	ldrh r1, [r5, r3]
	lsls r0, r0, #8
	subs r1, #1
	str r2, [sp, #4]
	bl FixedPoint_Ratio
	movs r1, #192
	lsls r1, r1, #4
	ldr r2, [sp, #4]
	cmp r0, r1
	bls .L_08018a32
	movs r0, #128
	lsls r0, r0, #2
.L_08018a32:
	strh r0, [r6]
	adds r6, #2
	adds r2, #1
	adds r5, #2
	cmp r2, r10
	bls Func_080189de
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
