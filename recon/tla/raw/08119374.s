.syntax unified
	.thumb
	.global Func_08119374
	.thumb_func
Func_08119374:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	sub sp, #20
	adds r3, r1, #0
	adds r3, #68
	ldrb r3, [r3]
	movs r7, #0
	cmp r3, #0
	bne .L_0811938c
	b .L_0811959a
.L_0811938c:
	adds r3, r1, #0
	adds r3, #80
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_081193d8
	lsls r2, r2, #3
	adds r5, r2, r3
	adds r3, r1, #0
	adds r3, #82
	ldrb r3, [r3]
	ldr r6, .L_081193dc
	cmp r3, #0
	beq .L_081193ae
	b .L_08119578
.L_081193ae:
	ldr r3, .L_081193c8
	movs r0, #1
	strh r3, [r6]
	ldr r3, .L_081193cc
	strh r3, [r6, #2]
	ldr r3, .L_081193d0
	strh r3, [r6, #8]
	ldr r3, .L_081193d4
	strh r3, [r6, #10]
	bl WaitFrames
	b .L_081193e6
	.2byte 0x0000
.L_081193c8:
	.4byte 0x00000065
.L_081193cc:
	.4byte 0x00000078
.L_081193d0:
	.4byte 0x00000054
.L_081193d4:
	.4byte 0x00000055
.L_081193d8:
	.4byte Data_02003874
.L_081193dc:
	.4byte Data_02003a74
.L_081193e0:
	movs r0, #1
	bl WaitFrames
.L_081193e6:
	ldr r3, .L_0811945c
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_081193fa
	adds r7, #1
	cmp r7, #24
	ble .L_081193e0
	b .L_08119578
.L_081193fa:
	ldrh r2, [r6, #4]
	ldrh r3, [r5, #4]
	movs r7, #0
	cmp r2, r3
	beq .L_08119406
	b .L_08119578
.L_08119406:
	ldrh r2, [r6, #6]
	ldrh r3, [r5, #6]
	cmp r2, r3
	beq .L_08119410
	b .L_08119578
.L_08119410:
	ldrh r2, [r6]
	ldrh r3, [r5]
	cmp r2, r3
	bne .L_081193e0
	ldrh r2, [r6, #2]
	ldrh r3, [r5, #2]
	cmp r2, r3
	bne .L_081193e0
	ldrh r2, [r6, #8]
	ldrh r3, [r5, #8]
	cmp r2, r3
	bne .L_081193e0
	ldrh r2, [r6, #10]
	ldrh r3, [r5, #10]
	cmp r2, r3
	bne .L_081193e0
	ldr r3, .L_08119454
	strh r3, [r6, #12]
	ldr r3, .L_08119458
	strh r3, [r6, #14]
	b .L_08119440
.L_0811943a:
	movs r0, #1
	bl WaitFrames
.L_08119440:
	ldr r3, .L_0811945c
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_08119460
	adds r7, #1
	cmp r7, #24
	ble .L_0811943a
	b .L_08119578
.L_08119454:
	.4byte 0x00000072
.L_08119458:
	.4byte 0x0000006e
.L_0811945c:
	.4byte gLinkStatus
.L_08119460:
	ldrh r2, [r6, #8]
	ldrh r3, [r5, #8]
	movs r7, #0
	cmp r2, r3
	beq .L_0811946c
	b .L_08119578
.L_0811946c:
	ldrh r2, [r6, #10]
	ldrh r3, [r5, #10]
	cmp r2, r3
	beq .L_08119476
	b .L_08119578
.L_08119476:
	ldrh r2, [r6, #12]
	ldrh r3, [r5, #12]
	cmp r2, r3
	bne .L_0811943a
	ldrh r2, [r6, #14]
	ldrh r3, [r5, #14]
	cmp r2, r3
	bne .L_0811943a
	ldr r3, .L_081194b0
	ldr r2, .L_081194b4
	strh r3, [r6]
	strh r3, [r6, #4]
	ldr r3, .L_081194b8
	strh r2, [r6, #2]
	strh r3, [r6, #6]
	b .L_0811949c
.L_08119496:
	movs r0, #1
	bl WaitFrames
.L_0811949c:
	ldr r3, .L_081194bc
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_081194c0
	adds r7, #1
	cmp r7, #24
	ble .L_08119496
	b .L_08119578
.L_081194b0:
	.4byte 0x00000045
.L_081194b4:
	.4byte 0x00000058
.L_081194b8:
	.4byte 0x00000043
.L_081194bc:
	.4byte gLinkStatus
.L_081194c0:
	ldrh r2, [r6, #12]
	ldrh r3, [r5, #12]
	movs r7, #0
	cmp r2, r3
	bne .L_08119578
	ldrh r2, [r6, #14]
	ldrh r3, [r5, #14]
	cmp r2, r3
	bne .L_08119578
	ldrh r2, [r6]
	ldrh r3, [r5]
	cmp r2, r3
	bne .L_08119496
	ldrh r2, [r6, #2]
	ldrh r3, [r5, #2]
	cmp r2, r3
	bne .L_08119496
	ldrh r2, [r6, #4]
	ldrh r3, [r5, #4]
	cmp r2, r3
	bne .L_08119496
	ldrh r2, [r6, #6]
	ldrh r3, [r5, #6]
	cmp r2, r3
	bne .L_08119496
	ldr r3, .L_08119518
	strh r3, [r6, #8]
	ldr r3, .L_0811951c
	strh r3, [r6, #10]
	b .L_08119502
.L_081194fc:
	movs r0, #1
	bl WaitFrames
.L_08119502:
	ldr r3, .L_08119520
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_08119524
	adds r7, #1
	cmp r7, #24
	ble .L_081194fc
	b .L_08119578
	.2byte 0x0000
.L_08119518:
	.4byte 0x00000074
.L_0811951c:
	.4byte 0x00000075
.L_08119520:
	.4byte gLinkStatus
.L_08119524:
	ldrh r2, [r6]
	ldrh r3, [r5]
	movs r7, #0
	cmp r2, r3
	bne .L_08119578
	ldrh r2, [r6, #2]
	ldrh r3, [r5, #2]
	cmp r2, r3
	bne .L_08119578
	ldrh r2, [r6, #4]
	ldrh r3, [r5, #4]
	cmp r2, r3
	bne .L_08119578
	ldrh r2, [r6, #6]
	ldrh r3, [r5, #6]
	cmp r2, r3
	bne .L_08119578
	ldrh r2, [r6, #8]
	ldrh r3, [r5, #8]
	cmp r2, r3
	bne .L_081194fc
	ldrh r2, [r6, #10]
	ldrh r3, [r5, #10]
	cmp r2, r3
	bne .L_081194fc
	ldr r3, .L_08119580
	strh r3, [r6, #12]
	ldr r3, .L_08119584
	strh r3, [r6, #14]
	b .L_08119566
.L_08119560:
	movs r0, #1
	bl WaitFrames
.L_08119566:
	ldr r3, .L_08119588
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0811958c
	adds r7, #1
	cmp r7, #24
	ble .L_08119560
.L_08119578:
	movs r0, #1
	negs r0, r0
	b .L_0811959c
	.2byte 0x0000
.L_08119580:
	.4byte 0x00000052
.L_08119584:
	.4byte 0x0000004e
.L_08119588:
	.4byte gLinkStatus
.L_0811958c:
	ldrh r3, [r5, #12]
	movs r7, #0
	cmp r3, #114
	bne .L_0811959a
	ldrh r3, [r5, #14]
	cmp r3, #110
	beq .L_08119560
.L_0811959a:
	movs r0, #0
.L_0811959c:
	add sp, #20
	pop {r5, r6, r7, pc}
