.syntax unified
	.thumb
	.global DisplayTransition_UpdateScanlineTable
	.thumb_func
DisplayTransition_UpdateScanlineTable:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0808f7a0
	ldr r6, [r3]
	ldr r3, [r3, #92]
	sub sp, #24
	ldr r0, .L_0808f7a4
	str r3, [sp, #20]
	adds r4, r3, r0
	movs r2, #0
	ldrsb r2, [r4, r2]
	cmp r2, #0
	beq .L_0808f5fc
	ldr r5, .L_0808f7a8
	adds r1, r3, r5
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r0, [r1]
	cmp r3, r2
	blt .L_0808f5c4
	movs r3, #0
	strb r3, [r4]
	ldr r7, [sp, #20]
	ldr r0, .L_0808f7ac
	adds r3, r7, r0
	movs r2, #0
	ldrsb r2, [r3, r2]
	cmp r2, #0
	bne .L_0808f5b8
	ldr r1, .L_0808f7b0
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #64
	bne .L_0808f588
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	ldr r3, .L_0808f7b4
	b .L_0808f590
.L_0808f588:
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	ldr r3, .L_0808f7b8
.L_0808f590:
	ands r3, r2
	strh r3, [r1]
	ldr r0, .L_0808f7bc
	bl Scheduler_RemoveCallback
	ldr r0, .L_0808f7c0
	bl Scheduler_RemoveCallback
	ldr r2, .L_0808f7c4
	ldr r3, .L_0808f7c8
	ldrh r1, [r2, #10]
	ands r3, r1
	strh r3, [r2, #10]
	ldr r3, .L_0808f7cc
	ldrh r1, [r2, #10]
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	bl .L_0808fe1e
.L_0808f5b8:
	ldr r5, [sp, #20]
	movs r7, #165
	lsls r7, r7, #3
	adds r3, r5, r7
	strh r2, [r3]
	b .L_0808f5fc
.L_0808f5c4:
	ldr r2, [sp, #20]
	ldr r5, .L_0808f7b0
	adds r3, r2, r5
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r7, [sp, #20]
	ldr r3, .L_0808f7d0
	adds r5, r7, r3
	movs r3, #0
	ldrsb r3, [r5, r3]
	subs r2, r2, r3
	adds r3, r0, #1
	strb r3, [r1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r0, r3, #0
	muls r0, r2
	movs r1, #0
	ldrsb r1, [r4, r1]
	ldr r3, .L_0808f7d4
	bl _call_via_r3
	movs r3, #0
	ldrsb r3, [r5, r3]
	ldr r5, .L_0808f7d8
	adds r3, r3, r0
	adds r2, r7, r5
	strh r3, [r2]
.L_0808f5fc:
	ldr r7, [sp, #20]
	ldr r0, .L_0808f7dc
	adds r3, r7, r0
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #5
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r4, r7, r3
	adds r0, r4, #4
	str r4, [sp, #0]
	bl UiWindow_FillFromSceneFar
	movs r1, #165
	lsls r1, r1, #3
	adds r3, r7, r1
	ldrh r3, [r3]
	ldr r4, [sp, #0]
	cmp r3, #77
	bls .L_0808f62e
	bl .L_0808fe10
.L_0808f62e:
	ldr r2, .L_0808f7e0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0808f638:
	.4byte .L_0808f770
	.4byte .L_0808f818
	.4byte .L_0808f8e4
	.4byte .L_0808f958
	.4byte .L_0808f9ea
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fa52
	.4byte .L_0808fb3c
	.4byte .L_0808fc32
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fe10
	.4byte .L_0808fd0e
.L_0808f770:
	ldr r3, .L_0808f798
	strh r3, [r4]
	ldr r3, .L_0808f79c
	adds r4, #2
	strh r3, [r4]
	ldr r2, [sp, #20]
	ldr r5, .L_0808f7d8
	adds r3, r2, r5
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_0808f7e4
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_0808f7e8
	.2byte 0x0000
.L_0808f798:
	.4byte 0x00007f7f
.L_0808f79c:
	.4byte 0x00000001
.L_0808f7a0:
	.4byte gMapWork
.L_0808f7a4:
	.4byte 0x0000053c
.L_0808f7a8:
	.4byte 0x0000053d
.L_0808f7ac:
	.4byte 0x0000053e
.L_0808f7b0:
	.4byte 0x0000053b
.L_0808f7b4:
	.4byte 0x000081ff
.L_0808f7b8:
	.4byte 0x00009fff
.L_0808f7bc:
	.4byte BattleFx_StartWindowHBlankDma
.L_0808f7c0:
	.4byte DisplayTransition_UpdateScanlineTable
.L_0808f7c4:
	.4byte 0x040000b0
.L_0808f7c8:
	.4byte 0x0000c5ff
.L_0808f7cc:
	.4byte 0x00007fff
.L_0808f7d0:
	.4byte 0x0000053a
.L_0808f7d4:
	.4byte IwramSignedDivideArm
.L_0808f7d8:
	.4byte 0x0000052a
.L_0808f7dc:
	.4byte 0x00000539
.L_0808f7e0:
	.4byte .L_0808f638
.L_0808f7e4:
	movs r3, #31
	ands r5, r3
.L_0808f7e8:
	ldr r3, .L_0808f908
	ldrb r5, [r3, r5]
	movs r7, #0
	movs r3, #241
	mov r8, r7
	subs r6, r3, r5
.L_0808f7f4:
	str r4, [sp, #0]
	bl Random16
	adds r3, r6, #0
	muls r3, r0
	movs r0, #1
	lsrs r3, r3, #16
	lsls r2, r3, #8
	ldr r4, [sp, #0]
	adds r3, r3, r5
	add r8, r0
	orrs r2, r3
	mov r1, r8
	strh r2, [r4]
	adds r4, #4
	cmp r1, #159
	bls .L_0808f7f4
	b .L_0808fe10
.L_0808f818:
	ldr r2, [sp, #20]
	ldr r5, .L_0808f90c
	adds r3, r2, r5
	ldrh r5, [r3]
	movs r3, #31
	ands r3, r5
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r3, .L_0808f910
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_0808f83e
	ldrh r3, [r2]
	strh r3, [r4]
	ldrh r3, [r2, #2]
	b .L_0808f844
.L_0808f83e:
	ldrh r3, [r2, #2]
	strh r3, [r4]
	ldrh r3, [r2]
.L_0808f844:
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	movs r7, #0
	adds r2, #4
	mov r8, r7
	mov r10, r2
.L_0808f852:
	mov r2, r10
	ldrh r7, [r2]
	ldrh r0, [r2, #2]
	cmp r7, #0
	beq .L_0808f8d4
	cmp r0, #0
	beq .L_0808f876
	movs r1, #0
	mov r9, r1
	cmp r9, r7
	bge .L_0808f8d4
.L_0808f868:
	movs r2, #1
	add r9, r2
	strh r0, [r4]
	adds r4, #4
	cmp r9, r7
	blt .L_0808f868
	b .L_0808f8d4
.L_0808f876:
	ldrb r3, [r2, #4]
	ldrb r6, [r2, #6]
	mov r11, r3
	ldrb r0, [r2, #7]
	ldrb r3, [r2, #5]
	cmp r7, #0
	beq .L_0808f8d4
	mov r5, r11
	subs r5, r3, r5
	subs r0, r0, r6
	str r5, [sp, #16]
	str r0, [sp, #12]
	movs r2, #0
	movs r3, #0
	mov r9, r7
.L_0808f894:
	adds r0, r3, #0
	adds r1, r7, #0
	str r2, [sp, #8]
	str r3, [sp, #4]
	str r4, [sp, #0]
	bl __divsi3
	ldr r2, [sp, #8]
	adds r5, r0, #0
	adds r1, r7, #0
	adds r0, r2, #0
	bl __divsi3
	add r5, r11
	adds r0, r6, r0
	ldr r4, [sp, #0]
	lsls r5, r5, #8
	adds r5, r5, r0
	strh r5, [r4]
	movs r5, #1
	ldr r0, [sp, #12]
	ldr r2, [sp, #8]
	negs r5, r5
	ldr r3, [sp, #4]
	ldr r1, [sp, #16]
	add r9, r5
	adds r2, r2, r0
	mov r0, r9
	adds r4, #4
	adds r3, r3, r1
	cmp r0, #0
	bne .L_0808f894
.L_0808f8d4:
	movs r2, #1
	add r8, r2
	movs r1, #8
	mov r3, r8
	add r10, r1
	cmp r3, #2
	bls .L_0808f852
	b .L_0808fe10
.L_0808f8e4:
	ldr r5, [sp, #20]
	ldr r7, .L_0808f90c
	adds r3, r5, r7
	ldrh r3, [r3]
	subs r5, r3, #1
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_0808f914
	ldr r3, .L_0808f900
	strh r3, [r4]
	ldr r3, .L_0808f904
	b .L_0808f91a
	.2byte 0x0000
.L_0808f900:
	.4byte 0x00000001
.L_0808f904:
	.4byte 0x00007f7f
.L_0808f908:
	.4byte Data_0809e8ac
.L_0808f90c:
	.4byte 0x0000052a
.L_0808f910:
	.4byte Data_0809f840
.L_0808f914:
	ldr r3, .L_0808f940
	strh r3, [r4]
	ldr r3, .L_0808f944
.L_0808f91a:
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	movs r3, #31
	ands r5, r3
	movs r0, #0
	mov r8, r0
	lsls r5, r5, #4
.L_0808f92a:
	str r4, [sp, #0]
	bl Random16
	lsls r0, r0, #4
	lsrs r0, r0, #16
	adds r0, r5, r0
	ldr r4, [sp, #0]
	cmp r0, #255
	bls .L_0808f948
	movs r0, #255
	b .L_0808f948
.L_0808f940:
	.4byte 0x00007f7f
.L_0808f944:
	.4byte 0x00000001
.L_0808f948:
	movs r1, #1
	add r8, r1
	mov r2, r8
	strh r0, [r4]
	adds r4, #4
	cmp r2, #159
	bls .L_0808f92a
	b .L_0808fe10
.L_0808f958:
	ldr r5, [sp, #20]
	ldr r7, .L_0808f998
	ldr r2, .L_0808f990
	adds r3, r5, r7
	ldrh r5, [r3]
	ldr r3, .L_0808f994
	strh r2, [r4]
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	cmp r5, #32
	bls .L_0808f97c
	strh r3, [r4]
	adds r4, #2
	movs r3, #64
	strh r2, [r4]
	subs r5, r3, r5
	adds r4, #2
.L_0808f97c:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	movs r0, #0
	lsls r3, r3, #16
	ldr r7, .L_0808f99c
	mov r11, r3
	mov r8, r0
	b .L_0808f9a0
.L_0808f990:
	.4byte 0x00007f7f
.L_0808f994:
	.4byte 0x00000001
.L_0808f998:
	.4byte 0x0000052a
.L_0808f99c:
	.4byte IwramSqrt
.L_0808f9a0:
	mov r5, r8
	subs r5, #80
	adds r0, r5, #0
	muls r0, r5
	mov r1, r11
	lsls r0, r0, #16
	str r4, [sp, #0]
	subs r0, r1, r0
	bl _call_via_r7
	movs r3, #120
	asrs r0, r0, #8
	subs r6, r3, r0
	ldr r4, [sp, #0]
	adds r0, #120
	cmp r6, #0
	bge .L_0808f9c4
	movs r6, #0
.L_0808f9c4:
	cmp r0, #0
	bge .L_0808f9ca
	movs r0, #0
.L_0808f9ca:
	cmp r6, #240
	ble .L_0808f9d0
	movs r6, #240
.L_0808f9d0:
	cmp r0, #240
	ble .L_0808f9d6
	movs r0, #240
.L_0808f9d6:
	lsls r3, r6, #8
	movs r2, #1
	adds r3, r3, r0
	add r8, r2
	strh r3, [r4]
	mov r3, r8
	adds r4, #4
	cmp r3, #159
	bls .L_0808f9a0
	b .L_0808fe10
.L_0808f9ea:
	ldr r5, [sp, #20]
	ldr r7, .L_0808fa0c
	adds r3, r5, r7
	ldrh r5, [r3]
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_0808fa10
	ldr r3, .L_0808fa04
	strh r3, [r4]
	ldr r3, .L_0808fa08
	b .L_0808fa16
	.2byte 0x0000
.L_0808fa04:
	.4byte 0x00000001
.L_0808fa08:
	.4byte 0x00007f7f
.L_0808fa0c:
	.4byte 0x0000052a
.L_0808fa10:
	ldr r3, .L_0808fa38
	strh r3, [r4]
	ldr r3, .L_0808fa3c
.L_0808fa16:
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	movs r3, #31
	ands r3, r5
	lsls r2, r3, #4
	subs r2, r2, r3
	lsls r2, r2, #4
	lsrs r5, r2, #5
	movs r3, #240
	subs r3, r3, r5
	movs r0, #0
	lsls r3, r3, #8
	mov r8, r0
	adds r3, #240
	b .L_0808fa40
	.2byte 0x0000
.L_0808fa38:
	.4byte 0x00007f7f
.L_0808fa3c:
	.4byte 0x00000001
.L_0808fa40:
	movs r1, #2
	add r8, r1
	mov r2, r8
	strh r5, [r4]
	strh r3, [r4, #4]
	adds r4, #8
	cmp r2, #159
	bls .L_0808fa40
	b .L_0808fe10
.L_0808fa52:
	adds r2, r6, #0
	adds r2, #228
	ldr r3, .L_0808fd84
	ldr r5, [r2]
	ldr r6, [r2, #4]
	ands r5, r3
	ands r6, r3
	movs r7, #250
	ldr r3, .L_0808fd88
	lsls r7, r7, #1
	adds r3, r3, r7
	ldr r0, [r3]
	str r4, [sp, #0]
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	subs r3, r3, r5
	ldr r4, [sp, #0]
	cmp r3, #0
	bge .L_0808fa7e
	ldr r1, .L_0808fd8c
	adds r3, r3, r1
.L_0808fa7e:
	ldr r2, [r0, #12]
	asrs r7, r3, #16
	ldr r3, [r0, #16]
	subs r3, r3, r2
	subs r0, r3, r6
	cmp r0, #0
	bge .L_0808fa90
	ldr r2, .L_0808fd8c
	adds r0, r0, r2
.L_0808fa90:
	asrs r3, r0, #16
	subs r3, #16
	ldr r5, [sp, #20]
	mov r10, r3
	ldr r0, .L_0808fd90
	ldr r3, .L_0808fd94
	ldr r3, [r3]
	adds r3, r5, r0
	ldrh r3, [r3]
	ldr r1, .L_0808fd98
	strh r3, [r4]
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r4, #2
	ldr r2, .L_0808fd9c
	strh r3, [r4]
	adds r3, r5, r2
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	bne .L_0808fac8
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_0808facc
.L_0808fac8:
	movs r3, #31
	ands r5, r3
.L_0808facc:
	ldr r3, .L_0808fd94
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0808fada
	movs r5, #0
.L_0808fada:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	ldr r5, .L_0808fda0
	lsls r3, r3, #16
	mov r11, r3
	movs r3, #0
	mov r8, r3
	mov r9, r5
.L_0808faee:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #15
	str r4, [sp, #0]
	subs r0, r2, r0
	bl _call_via_r9
	asrs r0, r0, #8
	subs r6, r7, r0
	ldr r4, [sp, #0]
	adds r0, r7, r0
	cmp r6, #0
	bge .L_0808fb16
	movs r6, #0
.L_0808fb16:
	cmp r0, #0
	bge .L_0808fb1c
	movs r0, #0
.L_0808fb1c:
	cmp r6, #240
	ble .L_0808fb22
	movs r6, #240
.L_0808fb22:
	cmp r0, #240
	ble .L_0808fb28
	movs r0, #240
.L_0808fb28:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	mov r5, r8
	adds r4, #4
	cmp r5, #159
	bls .L_0808faee
	b .L_0808fe10
.L_0808fb3c:
	adds r1, r6, #0
	adds r1, #228
	ldr r3, .L_0808fd84
	ldr r2, [r1]
	ldr r7, [sp, #20]
	ldr r1, [r1, #4]
	ldr r0, .L_0808fda4
	ands r2, r3
	ands r1, r3
	adds r3, r7, r0
	ldr r3, [r3]
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0808fb5c
	ldr r2, .L_0808fd8c
	adds r3, r3, r2
.L_0808fb5c:
	ldr r5, [sp, #20]
	movs r0, #166
	lsls r0, r0, #3
	asrs r7, r3, #16
	adds r3, r5, r0
	ldr r3, [r3]
	subs r1, r3, r1
	cmp r1, #0
	bge .L_0808fb72
	ldr r2, .L_0808fd8c
	adds r1, r1, r2
.L_0808fb72:
	asrs r3, r1, #16
	subs r3, #16
	lsls r2, r3, #1
	mov r10, r3
	ldr r3, .L_0808fd94
	ldr r3, [r3]
	ldr r5, [sp, #20]
	ldr r0, .L_0808fd98
	subs r3, r3, r2
	mov r9, r3
	adds r3, r5, r0
	ldrh r3, [r3]
	ldr r1, .L_0808fd90
	strh r3, [r4]
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r4, #2
	ldr r2, .L_0808fd9c
	strh r3, [r4]
	adds r3, r5, r2
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_0808fbb0
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_0808fbb4
.L_0808fbb0:
	movs r3, #31
	ands r5, r3
.L_0808fbb4:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	lsls r3, r3, #16
	mov r11, r3
	movs r3, #0
	mov r8, r3
.L_0808fbc4:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #15
	str r4, [sp, #0]
	subs r0, r2, r0
	ldr r3, .L_0808fda0
	bl _call_via_r3
	asrs r0, r0, #8
	subs r6, r7, r0
	adds r0, r7, r0
	ldr r4, [sp, #0]
	cmp r6, r0
	bge .L_0808fc02
	movs r3, #31
	mov r5, r9
	ldr r1, .L_0808fda8
	ands r3, r5
	ldrsb r3, [r1, r3]
	subs r6, r6, r3
	adds r0, r0, r3
	cmp r6, r0
	blt .L_0808fc02
	movs r6, #240
	movs r0, #240
.L_0808fc02:
	cmp r6, #0
	bge .L_0808fc08
	movs r6, #0
.L_0808fc08:
	cmp r0, #0
	bge .L_0808fc0e
	movs r0, #0
.L_0808fc0e:
	cmp r6, #240
	ble .L_0808fc14
	movs r6, #240
.L_0808fc14:
	cmp r0, #240
	ble .L_0808fc1a
	movs r0, #240
.L_0808fc1a:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	movs r2, #2
	mov r5, r8
	adds r4, #4
	add r9, r2
	cmp r5, #159
	bls .L_0808fbc4
	b .L_0808fe10
.L_0808fc32:
	adds r2, r6, #0
	adds r2, #228
	ldr r3, .L_0808fd84
	ldr r5, [r2]
	ldr r6, [r2, #4]
	ands r5, r3
	ands r6, r3
	movs r7, #250
	ldr r3, .L_0808fd88
	lsls r7, r7, #1
	adds r3, r3, r7
	ldr r0, [r3]
	str r4, [sp, #0]
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	subs r3, r3, r5
	ldr r4, [sp, #0]
	cmp r3, #0
	bge .L_0808fc5e
	ldr r1, .L_0808fd8c
	adds r3, r3, r1
.L_0808fc5e:
	ldr r2, [r0, #12]
	asrs r7, r3, #16
	ldr r3, [r0, #16]
	subs r3, r3, r2
	subs r0, r3, r6
	cmp r0, #0
	bge .L_0808fc70
	ldr r2, .L_0808fd8c
	adds r0, r0, r2
.L_0808fc70:
	asrs r3, r0, #16
	subs r3, #16
	ldr r5, [sp, #20]
	mov r10, r3
	ldr r0, .L_0808fd98
	ldr r3, .L_0808fd94
	ldr r3, [r3]
	adds r3, r5, r0
	ldrh r3, [r3]
	ldr r1, .L_0808fd90
	strh r3, [r4]
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r4, #2
	ldr r2, .L_0808fd9c
	strh r3, [r4]
	adds r3, r5, r2
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_0808fca8
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_0808fcac
.L_0808fca8:
	movs r3, #31
	ands r5, r3
.L_0808fcac:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	ldr r5, .L_0808fda0
	lsls r3, r3, #16
	mov r11, r3
	movs r3, #0
	mov r8, r3
	mov r9, r5
.L_0808fcc0:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #15
	str r4, [sp, #0]
	subs r0, r2, r0
	bl _call_via_r9
	asrs r0, r0, #8
	subs r6, r7, r0
	ldr r4, [sp, #0]
	adds r0, r7, r0
	cmp r6, #0
	bge .L_0808fce8
	movs r6, #0
.L_0808fce8:
	cmp r0, #0
	bge .L_0808fcee
	movs r0, #0
.L_0808fcee:
	cmp r6, #240
	ble .L_0808fcf4
	movs r6, #240
.L_0808fcf4:
	cmp r0, #240
	ble .L_0808fcfa
	movs r0, #240
.L_0808fcfa:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	mov r5, r8
	adds r4, #4
	cmp r5, #159
	bls .L_0808fcc0
	b .L_0808fe10
.L_0808fd0e:
	adds r2, r6, #0
	adds r2, #228
	ldr r3, .L_0808fd84
	ldr r5, [r2]
	ldr r6, [r2, #4]
	ands r5, r3
	ands r6, r3
	movs r7, #250
	ldr r3, .L_0808fd88
	lsls r7, r7, #1
	adds r3, r3, r7
	ldr r0, [r3]
	str r4, [sp, #0]
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	subs r3, r3, r5
	ldr r4, [sp, #0]
	cmp r3, #0
	bge .L_0808fd3a
	ldr r1, .L_0808fd8c
	adds r3, r3, r1
.L_0808fd3a:
	ldr r2, [r0, #12]
	asrs r7, r3, #16
	ldr r3, [r0, #16]
	subs r3, r3, r2
	subs r0, r3, r6
	cmp r0, #0
	bge .L_0808fd4c
	ldr r2, .L_0808fd8c
	adds r0, r0, r2
.L_0808fd4c:
	asrs r3, r0, #16
	subs r3, #8
	ldr r5, [sp, #20]
	mov r10, r3
	ldr r0, .L_0808fd98
	ldr r3, .L_0808fd94
	ldr r3, [r3]
	adds r3, r5, r0
	ldrh r3, [r3]
	ldr r1, .L_0808fd90
	strh r3, [r4]
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r4, #2
	ldr r2, .L_0808fd9c
	strh r3, [r4]
	adds r3, r5, r2
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_0808fdac
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_0808fdb0
.L_0808fd84:
	.4byte 0xffff0000
.L_0808fd88:
	.4byte gCell
.L_0808fd8c:
	.4byte 0x0000ffff
.L_0808fd90:
	.4byte 0x00000536
.L_0808fd94:
	.4byte gFrameCount
.L_0808fd98:
	.4byte 0x00000534
.L_0808fd9c:
	.4byte 0x0000052a
.L_0808fda0:
	.4byte IwramSqrt
.L_0808fda4:
	.4byte 0x0000052c
.L_0808fda8:
	.4byte Data_0809e8ce
.L_0808fdac:
	movs r3, #31
	ands r5, r3
.L_0808fdb0:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	ldr r5, .L_0808fe30
	lsls r3, r3, #16
	mov r11, r3
	movs r3, #0
	mov r8, r3
	mov r9, r5
.L_0808fdc4:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #14
	str r4, [sp, #0]
	subs r0, r2, r0
	bl _call_via_r9
	asrs r0, r0, #8
	subs r6, r7, r0
	ldr r4, [sp, #0]
	adds r0, r7, r0
	cmp r6, #0
	bge .L_0808fdec
	movs r6, #0
.L_0808fdec:
	cmp r0, #0
	bge .L_0808fdf2
	movs r0, #0
.L_0808fdf2:
	cmp r6, #240
	ble .L_0808fdf8
	movs r6, #240
.L_0808fdf8:
	cmp r0, #240
	ble .L_0808fdfe
	movs r0, #240
.L_0808fdfe:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	mov r5, r8
	adds r4, #4
	cmp r5, #159
	bls .L_0808fdc4
.L_0808fe10:
	ldr r7, [sp, #20]
	ldr r0, .L_0808fe34
	adds r3, r7, r0
	ldrb r2, [r3]
	movs r1, #1
	eors r2, r1
	strb r2, [r3]
.L_0808fe1e:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0808fe30:
	.4byte IwramSqrt
.L_0808fe34:
	.4byte 0x00000539
