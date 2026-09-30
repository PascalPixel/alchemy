.syntax unified
	.thumb
	.global UiText_DrawStringAtOffset
	.thumb_func
UiText_DrawStringAtOffset:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	mov r10, r2
	mov r8, r3
	adds r7, r1, #0
	bl Runtime_BumpAllocateAlternatePool
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r0, #0
	ldr r0, [r3, #60]
	ldrb r3, [r5]
	adds r2, r6, #0
	cmp r3, #0
	beq .L_08042132
.L_08042124:
	ldrb r3, [r5]
	adds r5, #1
	strh r3, [r2]
	adds r2, #2
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_08042124
.L_08042132:
	ldr r3, .L_0804216c
	mov r4, r8
	strh r3, [r2]
	lsrs r2, r4, #3
	movs r1, #14
	ldrsh r3, [r7, r1]
	mov r4, r10
	adds r3, r3, r2
	movs r1, #12
	ldrsh r2, [r7, r1]
	adds r3, #1
	lsrs r1, r4, #3
	adds r2, r2, r1
	lsls r3, r3, #5
	adds r3, r3, r2
	movs r2, #160
	adds r1, r3, #1
	lsls r2, r2, #2
	cmp r1, r2
	bcs .L_0804217e
	ldr r3, .L_08042170
	lsls r1, r1, #1
	adds r2, r1, r3
	adds r1, r0, r1
	movs r3, #7
	adds r1, #8
	ands r3, r4
	adds r0, r6, #0
	b .L_08042174
.L_0804216c:
	.4byte 0x00000000
.L_08042170:
	.4byte 0x06002000
.L_08042174:
	bl Func_080416cc
	adds r0, r6, #0
	bl Sys_Free
.L_0804217e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
