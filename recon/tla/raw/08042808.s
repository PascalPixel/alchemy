.syntax unified
	.thumb
	.global Func_08042808
	.thumb_func
Func_08042808:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r1, [sp, #16]
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r6, r0, #0
	mov r0, r9
	str r3, [sp, #8]
	str r0, [sp, #4]
	adds r5, r2, #0
	ldrb r3, [r3, #5]
	cmp r3, #0
	bne .L_08042850
	bl Func_080149f0
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_08042960
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08042964
	ldr r2, .L_08042968
	ldrh r3, [r3]
	strh r3, [r2]
.L_08042850:
	movs r1, #12
	ldrsh r3, [r6, r1]
	ldr r2, [sp, #16]
	movs r1, #4
	adds r2, r2, r3
	str r2, [sp, #16]
	movs r0, #14
	ldrsh r3, [r6, r0]
	str r1, [sp, #12]
	adds r5, r5, r3
	lsls r5, r5, #5
	str r5, [sp, #0]
.L_08042868:
	ldr r2, [sp, #0]
	ldr r0, [sp, #16]
	ldr r1, [sp, #8]
	adds r3, r2, r0
	lsls r3, r3, #1
	adds r3, #8
	ldrh r3, [r1, r3]
	ldr r2, .L_0804296c
	mov r10, r3
	ldr r3, .L_08042970
	mov r0, r10
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r0, r3
	mov r1, r9
	mov r8, r2
	mov r10, r0
	cmp r1, #7
	ble .L_0804289c
	ldr r2, .L_08042974
	ldr r3, .L_08042978
	mov r8, r2
	mov r12, r3
	b .L_080428c8
.L_0804289c:
	mov r0, r9
	cmp r0, #0
	blt .L_080428c8
	lsls r1, r0, #2
	mov r2, r8
	lsls r2, r1
	ldr r3, .L_08042974
	mov r8, r2
	movs r2, #32
	subs r2, r2, r1
	lsrs r3, r2
	mov r0, r8
	orrs r0, r3
	mov r3, r12
	lsls r3, r1
	mov r12, r3
	ldr r3, .L_08042978
	mov r8, r0
	lsrs r3, r2
	mov r0, r12
	orrs r0, r3
	mov r12, r0
.L_080428c8:
	movs r2, #192
	lsls r2, r2, #19
	movs r1, #0
	adds r2, #28
	mov lr, r1
	mov r11, r2
	movs r7, #0
	b .L_08042926
.L_080428d8:
	mov r3, r10
	lsls r6, r3, #5
	mov r0, r11
	subs r3, r6, r7
	ldr r4, [r3, r0]
	movs r1, #0
	movs r0, #0
	movs r5, #15
.L_080428e8:
	adds r2, r4, #0
	ands r2, r5
	cmp r2, #14
	bne .L_080428fa
	lsls r2, r1, #2
	adds r3, r5, #0
	lsls r3, r2
	mov r2, r8
	b .L_08042906
.L_080428fa:
	cmp r2, #1
	bne .L_0804290c
	lsls r2, r1, #2
	adds r3, r5, #0
	lsls r3, r2
	mov r2, r12
.L_08042906:
	ands r3, r2
	orrs r0, r3
	b .L_08042912
.L_0804290c:
	lsls r3, r1, #2
	lsls r2, r3
	orrs r0, r2
.L_08042912:
	adds r1, #1
	lsrs r4, r4, #4
	cmp r1, #7
	ble .L_080428e8
	subs r3, r6, r7
	mov r1, r11
	str r0, [r3, r1]
	movs r2, #1
	adds r7, #4
	add lr, r2
.L_08042926:
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_08042934
	mov r0, lr
	cmp r0, #2
	ble .L_080428d8
	b .L_0804293a
.L_08042934:
	mov r1, lr
	cmp r1, #0
	ble .L_080428d8
.L_0804293a:
	ldr r3, [sp, #12]
	ldr r0, [sp, #16]
	movs r2, #8
	negs r2, r2
	subs r3, #1
	adds r0, #1
	add r9, r2
	str r3, [sp, #12]
	str r0, [sp, #16]
	cmp r3, #0
	bge .L_08042868
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08042960:
	.4byte 0x050001c0
.L_08042964:
	.4byte 0x050001e8
.L_08042968:
	.4byte 0x050001dc
.L_0804296c:
	.4byte 0x22222222
.L_08042970:
	.4byte 0xcccccccc
.L_08042974:
	.4byte 0x88888888
.L_08042978:
	.4byte 0xdddddddd
