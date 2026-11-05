.syntax unified
	.thumb
	.section .text.x020081f8,"ax",%progbits
	.balign 4
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	ldr r1, .L_020082a8
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02008232
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02008232
	ldr r0, .L_020082ac
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02008232
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02008232:
	cmp r4, #0
	bne .L_02008238
	b .L_02008324
.L_02008238:
	ldr r3, .L_020082b0
	ldr r6, .L_020082b4
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008286
	ldr r3, .L_020082b8
	ldr r2, .L_020082bc
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02008250:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02008250
.L_02008286:
	ldr r3, .L_020082b4
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020082c0
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020082c4
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020082cc
	ldr r3, .L_020082c8
	b .L_020082ce
.L_020082a8:
	.4byte gData_02001a1c
.L_020082ac:
	.4byte gData_02001a20
.L_020082b0:
	.4byte Data_02001a2c
.L_020082b4:
	.4byte gData_02001a30
.L_020082b8:
	.4byte gData_02001a18
.L_020082bc:
	.4byte gData_02001a34
.L_020082c0:
	.4byte gData_02001a24
.L_020082c4:
	.4byte gData_02001a14
.L_020082c8:
	.4byte gData_02001a28
.L_020082cc:
	ldr r3, .L_02008314
.L_020082ce:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_02008318
	ldr r2, .L_0200830c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200831c
	ldr r2, .L_02008320
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008324
	ldr r3, .L_02008310
	strh r3, [r1]
	b .L_02008324
	.2byte 0x0000
.L_0200830c:
	.4byte 0x00000001
.L_02008310:
	.4byte 0x00000000
.L_02008314:
	.4byte Data_02001a2c
.L_02008318:
	.4byte gData_02001a14
.L_0200831c:
	.4byte gData_02001a34
.L_02008320:
	.4byte gData_02001a30
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008580,"ax",%progbits
	.balign 4
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	mov r12, r0
	ldr r0, [r3]
	movs r3, #224
	lsls r3, r3, #4
	movs r4, #224
	adds r1, r0, r3
	mov lr, r4
	movs r7, #0
.L_0200859c:
	ldrh r3, [r0]
	movs r5, #31
	lsrs r2, r3, #10
	ldr r6, .L_020085f8
	ands r2, r5
	movs r4, #248
	lsls r2, r2, #10
	lsls r4, r4, #7
	adds r0, #2
	mov r8, r6
	cmp r2, r4
	bls .L_020085b6
	adds r2, r4, #0
.L_020085b6:
	strh r2, [r1]
	lsrs r2, r3, #5
	ands r2, r5
	mov r6, r12
	muls r6, r2
	adds r2, r6, #0
	mov r6, r8
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085ce
	adds r2, r4, #0
.L_020085ce:
	strh r2, [r1]
	adds r2, r5, #0
	ands r2, r3
	mov r3, r12
	muls r3, r2
	mov r6, r8
	adds r2, r3, #0
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085e6
	adds r2, r4, #0
.L_020085e6:
	adds r7, #1
	strh r2, [r1]
	adds r1, #2
	cmp r7, lr
	bcc .L_0200859c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020085f8:
	.4byte 0xfffffc00
	.section .text.x0200869c,"ax",%progbits
	.global Func_0200069c
	.thumb_func
Func_0200069c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	movs r0, #0
	bl Func_0200131c
	ldr r5, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087b8
	cmp r2, r3
	bne .L_020086dc
	bl Func_020008c8
	ldr r0, .L_020087bc
	ldr r1, .L_020087c0
	ldr r2, .L_020087c4
	ldr r3, .L_020087c8
	bl Func_02000038
	b .L_02008754
.L_020086dc:
	ldr r3, .L_020087cc
	cmp r2, r3
	bne .L_02008712
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_02008754
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200129c
	movs r0, #141
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008754
.L_02008712:
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_02008766
	movs r0, #85
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200872c
	movs r0, #165
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008736
.L_0200872c:
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008754
.L_02008736:
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008754
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200129c
	ldr r1, .L_020087d4
	movs r0, #8
	bl Actor_SetMovementTypeById
.L_02008754:
	ldr r3, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087d0
	cmp r2, r3
	beq .L_0200877e
.L_02008766:
	ldr r3, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087cc
	cmp r2, r3
	bne .L_020087b0
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_02008794
.L_0200877e:
	ldr r3, .L_020087b4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	subs r3, #3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	b .L_02008796
.L_02008794:
	movs r3, #3
.L_02008796:
	movs r0, #128
	lsls r3, r3, #7
	lsls r0, r0, #3
	subs r0, r0, r3
	bl Func_02000580
	bl Func_0200133c
	movs r0, #1
	bl Func_020012fc
	bl Event_WaitTransitionFrames
.L_020087b0:
	movs r0, #0
	pop {r5, pc}
.L_020087b4:
	.4byte gMainRoutineWork
.L_020087b8:
	.4byte 0x00000088
.L_020087bc:
	.4byte gData_0200134c
.L_020087c0:
	.4byte gData_0200135c
.L_020087c4:
	.4byte gData_02001388
.L_020087c8:
	.4byte gData_020013b4
.L_020087cc:
	.4byte 0x0000008a
.L_020087d0:
	.4byte 0x00000089
.L_020087d4:
	.4byte gData_02001454
	.section .bss,"aw",%nobits
	.global gData_02001a14
gData_02001a14:
	.space 0x00000004
	.global gData_02001a18
gData_02001a18:
	.space 0x00000004
	.global gData_02001a1c
gData_02001a1c:
	.space 0x00000004
	.global gData_02001a20
gData_02001a20:
	.space 0x00000004
	.global gData_02001a24
gData_02001a24:
	.space 0x00000004
	.global gData_02001a28
gData_02001a28:
	.space 0x00000004
	.global Data_02001a2c
Data_02001a2c:
	.space 0x00000004
	.global gData_02001a30
gData_02001a30:
	.space 0x00000004
	.global gData_02001a34
gData_02001a34:
