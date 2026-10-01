.syntax unified
	.thumb
	.global Func_080e43a4
	.thumb_func
Func_080e43a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #1
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #80]
	adds r5, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	cmp r6, #0
	bne .L_080e43ee
	ldr r1, .L_080e4458
	movs r0, #80
	bl Runtime_AllocateHeapBlock
	ldr r2, .L_080e445c
	adds r1, r0, #0
	ldr r0, .L_080e4460
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
	mov r3, r10
	ldr r6, [r3, #80]
	movs r3, #0
	mov r9, r3
.L_080e43ee:
	ldr r3, .L_080e4464
	adds r0, r7, #0
	mov r1, r8
	mov lr, r3
	.2byte 0xf800
	ldrb r3, [r5, #7]
	cmp r3, #1
	bne .L_080e440e
	ldrb r3, [r5, #22]
	ldr r2, [r5, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	adds r1, r7, #0
	bl Resource_DecodeType01
	b .L_080e4440
.L_080e440e:
	cmp r3, #3
	bne .L_080e4430
	ldrb r3, [r5, #22]
	ldr r2, [r5, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	adds r1, r7, #0
	ldr r3, .L_080e4468
	mov lr, r3
	.2byte 0xf800
	cmp r0, #0
	beq .L_080e4440
	adds r1, r7, #0
	movs r2, #0
	mov lr, r6
	.2byte 0xf800
	b .L_080e4440
.L_080e4430:
	ldrb r3, [r5, #22]
	ldr r2, [r5, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	adds r1, r7, #0
	ldrb r2, [r5, #5]
	mov lr, r6
	.2byte 0xf800
.L_080e4440:
	mov r3, r9
	cmp r3, #0
	bne .L_080e444c
	movs r0, #80
	bl Runtime_ReleaseHeapBlock
.L_080e444c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e4458:
	.4byte 0x000002c8
.L_080e445c:
	.4byte BattleFx_DecodeFrameCopyEnd
.L_080e4460:
	.4byte BattleFx_DecodeFrameCode
.L_080e4464:
	.4byte IwramClearWords
.L_080e4468:
	.4byte IwramDecompress
