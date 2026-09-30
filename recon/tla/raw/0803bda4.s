.syntax unified
	.thumb
	.global Func_0803bda4
	.thumb_func
Func_0803bda4:
	ldr r2, [sp, #12]
	ldrh r3, [r5, r2]
	mov r2, r9
	subs r0, r2, r3
	subs r0, #4
	cmp r0, #0
	bge .L_0803bdb4
	movs r0, #0
.L_0803bdb4:
	mov r3, r11
	ldrh r1, [r5, r3]
	lsls r0, r0, #8
	subs r1, #1
	bl __divsi3
	mov r2, r8
	strh r0, [r2]
	movs r3, #2
	add r8, r3
	ldr r2, [sp, #16]
	adds r6, #1
	adds r5, #2
	cmp r6, r2
	bls Func_0803bd86
	bl Func_0803a404
	add sp, #108
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
