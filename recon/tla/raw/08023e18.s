.syntax unified
	.thumb
	.global Func_08023e18
	.thumb_func
Func_08023e18:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #48]
	ldr r3, [r3, #24]
	sub sp, #12
	mov r8, r3
	ldr r1, .L_08023ee4
	movs r0, #80
	bl Runtime_AllocateHeapBlock
	ldr r2, .L_08023ee8
	adds r1, r0, #0
	ldr r0, .L_08023eec
	movs r4, #132
	subs r2, r2, r0
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #2
	lsls r3, r3, #19
	adds r3, #212
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, r7, #0
	ldr r3, [r5, #24]
	adds r6, r5, #0
	adds r6, #12
	cmp r3, #0
	beq .L_08023e5a
	adds r5, r3, #0
.L_08023e5a:
	ldr r3, [r7, #28]
	cmp r3, #0
	beq .L_08023e62
	adds r6, r3, #0
.L_08023e62:
	ldr r3, [r6]
	ldr r0, [r5]
	ldr r1, [r5, #8]
	subs r0, r0, r3
	ldr r3, [r6, #8]
	asrs r0, r0, #16
	subs r1, r1, r3
	asrs r1, r1, #16
	bl ArcTan2
	lsls r0, r0, #16
	asrs r7, r0, #16
	movs r3, #0
	mov r2, r8
	movs r0, #108
	strh r3, [r2]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08023ea6
	bl Func_08014de4
	ldr r3, .L_08023ef0
	ldr r0, .L_08023ef4
	adds r7, r7, r3
	ldr r3, .L_08023ef8
	mov lr, r3
	.2byte 0xf800
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_080156f8
	b .L_08023eb2
.L_08023ea6:
	bl Func_08014e1c
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_080156e8
.L_08023eb2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r2, #252
	lsls r2, r2, #5
	adds r5, r5, r2
	movs r6, #63
.L_08023ec0:
	ldrh r3, [r5, #2]
	cmp r3, #0
	beq .L_08023ece
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_08023d80
.L_08023ece:
	subs r6, #1
	subs r5, #128
	cmp r6, #0
	bge .L_08023ec0
	movs r0, #80
	bl Runtime_ReleaseHeapBlock
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08023ee4:
	.4byte 0x000002c8
.L_08023ee8:
	.4byte Render_DecodeFrameCopyEnd
.L_08023eec:
	.4byte Render_DecodeFrameCode
.L_08023ef0:
	.4byte 0xffffe000
.L_08023ef4:
	.4byte Data_0802eb98
.L_08023ef8:
	.4byte IwramTransformMatrix
