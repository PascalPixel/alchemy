.syntax unified
	.thumb
	.global Func_080e6400
	.thumb_func
Func_080e6400:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #92]
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #48
	adds r3, r7, r0
	ldr r3, [r3]
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #56
	mov r8, r3
	adds r3, r7, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	sub sp, #4
	cmp r0, #0
	beq .L_080e642c
	b .L_080e65b6
.L_080e642c:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #40
	adds r6, r7, r3
	movs r4, #0
	ldrsh r3, [r6, r4]
	cmp r3, #1
	beq .L_080e6486
	cmp r3, #1
	bgt .L_080e6446
	cmp r3, #0
	beq .L_080e644c
	b .L_080e6508
.L_080e6446:
	cmp r3, #2
	beq .L_080e64d6
	b .L_080e6508
.L_080e644c:
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #42
	adds r1, r7, r0
	movs r4, #0
	ldrsh r3, [r1, r4]
	ldrh r2, [r1]
	cmp r3, #1
	bne .L_080e646c
	adds r0, #10
	adds r3, r7, r0
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #24]
	ldrh r2, [r1]
.L_080e646c:
	lsls r3, r2, #16
	movs r2, #128
	lsls r2, r2, #11
	cmp r3, r2
	bne .L_080e6508
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_080e6508
.L_080e6486:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #42
	adds r5, r7, r3
	movs r4, #0
	ldrsh r3, [r5, r4]
	ldrh r2, [r5]
	cmp r3, #8
	bne .L_080e64a2
	mov r0, r8
	movs r1, #3
	bl Object_SetMode
	ldrh r2, [r5]
.L_080e64a2:
	movs r0, #240
	lsls r3, r2, #16
	lsls r0, r0, #12
	cmp r3, r0
	bgt .L_080e64bc
	mov r1, r8
	ldr r3, [r1, #12]
	movs r2, #147
	lsls r2, r2, #10
	adds r2, #204
	adds r3, r3, r2
	str r3, [r1, #12]
	ldrh r2, [r5]
.L_080e64bc:
	movs r4, #224
	lsls r3, r2, #16
	lsls r4, r4, #13
	cmp r3, r4
	bne .L_080e6508
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e6508
.L_080e64d6:
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #42
	adds r2, r7, r1
	movs r4, #0
	ldrsh r3, [r2, r4]
	ldrh r1, [r2]
	cmp r3, #25
	bne .L_080e64f6
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #52
	adds r3, r7, r1
	ldr r3, [r3]
	str r0, [r3, #24]
	ldrh r1, [r2]
.L_080e64f6:
	movs r2, #200
	lsls r3, r1, #16
	lsls r2, r2, #14
	cmp r3, r2
	bne .L_080e6508
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	strh r3, [r6]
.L_080e6508:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #40
	adds r3, r3, r7
	mov r8, r3
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #2
	beq .L_080e651c
	b .L_080e68b0
.L_080e651c:
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #46
	adds r3, r7, r1
	movs r2, #0
	ldrsh r5, [r3, r2]
	ldr r3, .L_080e6854
	movs r4, #200
	lsls r4, r4, #5
	adds r4, #42
	movs r0, #128
	lsls r5, r5, #5
	lsls r0, r0, #3
	adds r5, r5, r3
	adds r3, r7, r4
	adds r6, r7, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r1, r6, #0
	adds r2, #14
	adds r0, r7, #0
	bl Func_080e62f4
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #84]
	movs r2, #32
	adds r3, r5, #0
	movs r1, #32
	adds r0, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r1, #200
	lsls r1, r1, #5
	adds r5, r7, r1
	adds r0, r5, #0
	bl Func_080eb01c
	mov r4, r8
	movs r2, #0
	ldrsh r3, [r4, r2]
	cmp r3, #2
	beq .L_080e6574
	b .L_080e68b0
.L_080e6574:
	movs r0, #144
	movs r1, #128
	lsls r0, r0, #5
	lsls r1, r1, #4
	adds r6, r7, r0
	movs r4, #0
	adds r0, r7, r1
.L_080e6582:
	ldr r3, [r6, #24]
	adds r5, r0, #0
	cmp r3, #31
	bhi .L_080e65a4
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #44
	adds r3, r7, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r0, r5, #0
	adds r1, r6, #0
	str r4, [sp, #0]
	bl Func_080e6388
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
.L_080e65a4:
	adds r3, #1
	adds r0, r5, #0
	adds r4, #1
	str r3, [r6, #24]
	adds r0, #40
	adds r6, #28
	cmp r4, #17
	ble .L_080e6582
	b .L_080e68b0
.L_080e65b6:
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #56
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	beq .L_080e65c8
	b .L_080e671a
.L_080e65c8:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #40
	adds r6, r7, r3
	movs r4, #0
	ldrsh r3, [r6, r4]
	cmp r3, #1
	beq .L_080e6622
	cmp r3, #1
	bgt .L_080e65e2
	cmp r3, #0
	beq .L_080e65e8
	b .L_080e66ae
.L_080e65e2:
	cmp r3, #2
	beq .L_080e667a
	b .L_080e66ae
.L_080e65e8:
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #42
	adds r1, r7, r0
	movs r4, #0
	ldrsh r3, [r1, r4]
	ldrh r2, [r1]
	cmp r3, #1
	bne .L_080e6608
	adds r0, #10
	adds r3, r7, r0
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #24]
	ldrh r2, [r1]
.L_080e6608:
	lsls r3, r2, #16
	movs r2, #128
	lsls r2, r2, #11
	cmp r3, r2
	bne .L_080e66ae
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_080e66ae
.L_080e6622:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #42
	adds r5, r7, r3
	movs r4, #0
	ldrsh r3, [r5, r4]
	ldrh r2, [r5]
	cmp r3, #8
	bne .L_080e6646
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #48
	adds r3, r7, r0
	ldr r0, [r3]
	movs r1, #3
	bl Object_SetMode
	ldrh r2, [r5]
.L_080e6646:
	movs r1, #240
	lsls r3, r2, #16
	lsls r1, r1, #12
	cmp r3, r1
	bgt .L_080e6660
	mov r2, r8
	ldr r3, [r2, #12]
	movs r4, #147
	lsls r4, r4, #10
	adds r4, #204
	adds r3, r3, r4
	str r3, [r2, #12]
	ldrh r2, [r5]
.L_080e6660:
	movs r0, #224
	lsls r3, r2, #16
	lsls r0, r0, #13
	cmp r3, r0
	bne .L_080e66ae
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e66ae
.L_080e667a:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #42
	adds r1, r7, r2
	movs r4, #0
	ldrsh r3, [r1, r4]
	ldrh r2, [r1]
	cmp r3, #70
	bne .L_080e669c
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #52
	adds r3, r7, r0
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #24]
	ldrh r2, [r1]
.L_080e669c:
	movs r1, #180
	lsls r3, r2, #16
	lsls r1, r1, #15
	cmp r3, r1
	bne .L_080e66ae
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	strh r3, [r6]
.L_080e66ae:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #40
	adds r3, r7, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #2
	beq .L_080e66c0
	b .L_080e68b0
.L_080e66c0:
	movs r1, #128
	movs r0, #144
	lsls r1, r1, #4
	lsls r0, r0, #5
	adds r1, r1, r7
	movs r4, #0
	adds r6, r7, r0
	mov r8, r1
.L_080e66d0:
	ldr r3, [r6, #24]
	cmp r3, #44
	bhi .L_080e6708
	mov r5, r8
	adds r0, r5, #0
	adds r1, r6, #0
	str r4, [sp, #0]
	bl Func_080eb298
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
	cmp r3, #41
	bgt .L_080e6708
	ldr r3, [r6, #4]
	ldr r2, .L_080e6858
	adds r3, r3, r2
	str r3, [r6, #4]
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r3, [r6]
	subs r5, r5, r0
	adds r3, r3, r5
	str r3, [r6]
	ldr r4, [sp, #0]
	ldr r3, [r6, #24]
.L_080e6708:
	adds r3, #1
	str r3, [r6, #24]
	adds r4, #1
	movs r3, #40
	add r8, r3
	adds r6, #28
	cmp r4, #63
	ble .L_080e66d0
	b .L_080e68b0
.L_080e671a:
	movs r4, #200
	lsls r4, r4, #5
	adds r4, #56
	adds r3, r7, r4
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	bne .L_080e67ce
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #40
	adds r6, r7, r1
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldrh r1, [r6]
	cmp r3, #1
	beq .L_080e676c
	cmp r3, #1
	bgt .L_080e6746
	cmp r3, #0
	beq .L_080e674c
	b .L_080e68b0
.L_080e6746:
	cmp r3, #2
	beq .L_080e67b4
	b .L_080e68b0
.L_080e674c:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #42
	adds r2, r7, r3
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #8
	beq .L_080e675e
	b .L_080e68b0
.L_080e675e:
	adds r3, r1, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r2]
	b .L_080e68b0
.L_080e676c:
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #42
	adds r5, r7, r0
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_080e678c
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #48
	adds r3, r7, r2
	ldr r0, [r3]
	movs r1, #4
	bl Object_SetMode
.L_080e678c:
	mov r4, r8
	ldr r3, [r4, #12]
	movs r0, #147
	lsls r0, r0, #10
	adds r0, #204
	adds r3, r3, r0
	str r3, [r4, #12]
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #15
	beq .L_080e67a4
	b .L_080e68b0
.L_080e67a4:
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e68b0
.L_080e67b4:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #42
	adds r3, r7, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #20
	bne .L_080e68b0
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	strh r3, [r6]
	b .L_080e68b0
.L_080e67ce:
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #40
	adds r6, r7, r0
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldrh r1, [r6]
	cmp r3, #1
	beq .L_080e680e
	cmp r3, #1
	bgt .L_080e67ea
	cmp r3, #0
	beq .L_080e67f0
	b .L_080e6874
.L_080e67ea:
	cmp r3, #2
	beq .L_080e685c
	b .L_080e6874
.L_080e67f0:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #42
	adds r2, r7, r3
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #8
	bne .L_080e6874
	adds r3, r1, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r2]
	b .L_080e6874
.L_080e680e:
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #42
	adds r5, r7, r0
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_080e682e
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #48
	adds r3, r7, r2
	ldr r0, [r3]
	movs r1, #4
	bl Object_SetMode
.L_080e682e:
	mov r4, r8
	ldr r3, [r4, #12]
	movs r0, #147
	lsls r0, r0, #10
	adds r0, #204
	adds r3, r3, r0
	str r3, [r4, #12]
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #15
	bne .L_080e6874
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e6874
.L_080e6854:
	.4byte 0x06010000
.L_080e6858:
	.4byte 0xffff0000
.L_080e685c:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #42
	adds r3, r7, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #30
	bne .L_080e6874
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	strh r3, [r6]
.L_080e6874:
	movs r0, #144
	movs r1, #128
	lsls r0, r0, #5
	lsls r1, r1, #4
	adds r6, r7, r0
	adds r5, r7, r1
	movs r4, #19
.L_080e6882:
	ldr r3, [r6, #24]
	cmp r3, #31
	bhi .L_080e68a2
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #44
	adds r3, r7, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r0, r5, #0
	adds r1, r6, #0
	str r4, [sp, #0]
	bl Func_080e6388
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
.L_080e68a2:
	adds r3, #1
	subs r4, #1
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r4, #0
	bge .L_080e6882
.L_080e68b0:
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #42
	adds r2, r7, r1
	ldrh r3, [r2]
	add sp, #4
	adds r3, #1
	strh r3, [r2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
