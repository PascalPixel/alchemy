.syntax unified
	.thumb
	.global Func_081a7518
	.thumb_func
Func_081a7518:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #110
	bl Audio_PlayCue
	ldr r2, .L_081a7580
	movs r3, #1
	strb r3, [r2]
	ldr r5, .L_081a7584
	bl Func_080144c0
	movs r0, #1
	bl Func_08013e70
	bl Func_08014b70
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_081a7578
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_081a757c
	subs r2, #12
	strh r3, [r2]
	ldr r3, .L_081a7588
	movs r6, #0
	strh r6, [r3, #10]
	adds r0, r5, #0
	bl Resource_GetTableEntry
	movs r7, #128
	movs r3, #128
	movs r2, #132
	lsls r7, r7, #1
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r7, #255
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #112
	b .L_081a758c
.L_081a7578:
	.4byte 0x00000681
.L_081a757c:
	.4byte 0x00001440
.L_081a7580:
	.4byte Data_0300120c
.L_081a7584:
	.4byte 0x00000020
.L_081a7588:
	.4byte Data_03001120
.L_081a758c:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_081a7638
	movs r3, #224
	lsls r3, r3, #1
	adds r4, r4, r3
	adds r1, r5, #0
	adds r0, r4, #0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_081a763c
	ldr r2, .L_081a7640
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_081a7644
	movs r3, #128
	lsls r3, r3, #1
	movs r5, #0
.L_081a75b8:
	movs r0, #0
.L_081a75ba:
	adds r2, r3, #0
	movs r4, #128
	lsls r3, r2, #16
	lsls r4, r4, #9
	adds r3, r3, r4
	adds r0, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r0, #29
	bls .L_081a75ba
	strh r7, [r1]
	adds r5, #1
	adds r1, #2
	strh r7, [r1]
	adds r1, #2
	cmp r5, #19
	bls .L_081a75b8
	ldr r3, .L_081a7648
	movs r5, #0
	movs r2, #0
.L_081a75e4:
	adds r5, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r5, #3
	bls .L_081a75e4
	movs r3, #128
	movs r1, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081a7648
	adds r1, #16
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_08014bac
	bl Func_08014b70
	ldr r3, .L_081a7634
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	mov r3, r8
	cmp r3, #0
	bne .L_081a766e
	movs r0, #1
	bl Func_08013eb4
	bl Func_08013fdc
	ldr r3, .L_081a764c
	movs r2, #9
	ldr r3, [r3, #4]
	movs r5, #0
	b .L_081a7662
	.2byte 0x0000
.L_081a7634:
	.4byte 0x00001540
.L_081a7638:
	.4byte gMapCellBuffer
.L_081a763c:
	.4byte 0x06004000
.L_081a7640:
	.4byte 0x84002580
.L_081a7644:
	.4byte 0x06003000
.L_081a7648:
	.4byte Data_03001120
.L_081a764c:
	.4byte gInput
.L_081a7650:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #119
	bhi .L_081a76ea
	ldr r3, .L_081a76f4
	movs r2, #9
	ldr r3, [r3, #4]
.L_081a7662:
	ands r3, r2
	cmp r3, #0
	beq .L_081a7650
	movs r6, #1
	negs r6, r6
	b .L_081a76ea
.L_081a766e:
	ldr r3, .L_081a76f4
	movs r2, #9
	ldr r3, [r3, #4]
	movs r5, #0
	b .L_081a768a
.L_081a7678:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bhi .L_081a7694
	ldr r3, .L_081a76f4
	movs r2, #9
	ldr r3, [r3, #4]
.L_081a768a:
	ands r3, r2
	cmp r3, #0
	beq .L_081a7678
	movs r6, #1
	negs r6, r6
.L_081a7694:
	cmp r6, #0
	beq .L_081a76a0
	movs r0, #8
	bl Func_08013eb4
	b .L_081a76a6
.L_081a76a0:
	movs r0, #60
	bl Func_08013eb4
.L_081a76a6:
	bl Func_08013fdc
	cmp r6, #0
	bne .L_081a76d8
	ldr r3, .L_081a76f4
	movs r2, #9
	ldr r3, [r3, #4]
	movs r5, #0
	b .L_081a76ca
.L_081a76b8:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #179
	bhi .L_081a76d4
	ldr r3, .L_081a76f4
	movs r2, #9
	ldr r3, [r3, #4]
.L_081a76ca:
	ands r3, r2
	cmp r3, #0
	beq .L_081a76b8
	movs r6, #1
	negs r6, r6
.L_081a76d4:
	cmp r6, #0
	beq .L_081a76e0
.L_081a76d8:
	movs r0, #8
	bl Func_08013e70
	b .L_081a76e6
.L_081a76e0:
	movs r0, #60
	bl Func_08013e70
.L_081a76e6:
	bl Func_08013fdc
.L_081a76ea:
	adds r0, r6, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a76f4:
	.4byte gInput
