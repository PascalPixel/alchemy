.syntax unified
	.thumb
	.global Func_08043774
	.thumb_func
Func_08043774:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r6, #0
	bl Func_0801596c
	cmp r0, #0
	beq .L_0804378e
	ldr r0, .L_080437ec
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	subs r6, #9
	b .L_080437e2
.L_0804378e:
	bl Func_08016054
	ldr r7, .L_080437f0
	movs r1, #0
	ldrsh r0, [r7, r1]
	adds r1, r5, #0
	bl Func_08043cd8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_080437ac
	adds r6, r5, #0
	b .L_080437e2
.L_080437ac:
	ldr r1, .L_080437f4
	adds r0, r5, #0
	bl Func_08015e44
	cmp r0, #0
	beq .L_080437c6
	ldr r0, .L_080437f8
	movs r1, #1
	movs r6, #2
	bl UiText_ShowPositionedMessageAndWait
	negs r6, r6
	b .L_080437e2
.L_080437c6:
	ldr r3, .L_080437fc
	ldr r1, .L_08043800
	ldr r2, [r3, #4]
	str r2, [r1]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #74
	adds r3, r3, r1
	ldrb r3, [r3]
	ldr r2, .L_08043804
	strb r3, [r2]
	ldr r3, .L_08043808
	strh r6, [r3]
	strh r5, [r7]
.L_080437e2:
	bl Func_0801613c
	adds r0, r6, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080437ec:
	.4byte 0x0000000b
.L_080437f0:
	.4byte Data_020036d0
.L_080437f4:
	.4byte Data_02000000
.L_080437f8:
	.4byte 0x0000000d
.L_080437fc:
	.4byte gPartyState
.L_08043800:
	.4byte Data_0300117c
.L_08043804:
	.4byte Data_03001200
.L_08043808:
	.4byte Data_03001218
