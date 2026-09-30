.syntax unified
	.thumb
	.global Func_08043534
	.thumb_func
Func_08043534:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #192
	movs r2, #0
	lsls r0, r0, #6
	mov r10, r2
	bl Runtime_BumpAllocateAlternatePool
	ldr r3, .L_080435e4
	adds r7, r0, #0
	movs r2, #0
	ldrsh r0, [r3, r2]
	mov r8, r3
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080435da
	bl Func_0801596c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_08043570
	ldr r0, .L_080435e8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r2, #9
	b .L_080435ca
.L_08043570:
	mov r2, r8
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r1, r7, #0
	bl Func_08015e44
	adds r6, r0, #0
	cmp r6, #0
	beq .L_08043590
	ldr r0, .L_080435ec
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #2
	negs r3, r3
	mov r10, r3
.L_08043590:
	ldr r1, .L_080435f0
	ldr r3, .L_080435f4
	adds r0, r7, r1
	movs r2, #16
	subs r0, r0, r3
	ldr r3, .L_080435f8
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	bl Func_08042e28
	mov r3, r8
	adds r5, r0, #0
	adds r1, r7, #0
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_08042f10
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_08042eec
	cmp r6, #0
	beq .L_080435ce
	ldr r0, .L_080435ec
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r2, #3
.L_080435ca:
	negs r2, r2
	mov r10, r2
.L_080435ce:
	bl Func_0801613c
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r10
.L_080435da:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080435e4:
	.4byte Data_020036d0
.L_080435e8:
	.4byte 0x0000000b
.L_080435ec:
	.4byte 0x0000000c
.L_080435f0:
	.4byte Data_02000504
.L_080435f4:
	.4byte Data_02000000
.L_080435f8:
	.4byte IwramCopyWords
