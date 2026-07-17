@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_0800447c, 0x0800447c
	.set sub_080044d0, 0x080044d0
	.set sub_0800c300, 0x0800c300
	.set sub_0800d14c, 0x0800d14c
	.set sub_0800d924, 0x0800d924
	.set sub_0800eba0, 0x0800eba0
	.set sub_0800f1fa, 0x0800f1fa
	.set sub_080120dc, 0x080120dc
	.set sub_08077008, 0x08077008
	.global Region_0800ee14
Region_0800ee14:
	ldr r0, [sp, #4]
	add r3, sp, #24
	movs r1, #128
	mov r10, r3
	lsls r1, r1, #5
	lsrs r3, r0, #16
	adds r2, r3, r1
	ldr r1, [pc, #868]
	mov r0, r10
	strh r2, [r0, #0]
	adds r2, r3, r1
	movs r1, #128
	lsls r1, r1, #6
	strh r2, [r0, #2]
	adds r2, r3, r1
	ldr r1, [pc, #856]
	strh r2, [r0, #4]
	adds r2, r3, r1
	movs r1, #192
	lsls r1, r1, #6
	strh r2, [r0, #6]
	adds r2, r3, r1
	ldr r1, [pc, #844]
	strh r2, [r0, #8]
	adds r3, r3, r1
	mov r2, r10
	strh r3, [r2, #10]
	movs r3, #0
	mov r9, r3
	mov r8, r11
.L_0800ee50:
	mov r0, r9
	lsls r3, r0, #1
	mov r1, r10
	ldrsh r1, [r1, r3]
	str r1, [sp, #12]
	ldr r3, [r7, #8]
	mov r0, r8
	str r3, [r0, #0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #16
	str r3, [r0, #8]
	lsrs r6, r1, #16
	movs r0, #128
	str r1, [sp, #0]
	lsls r0, r0, #12
	adds r1, r6, #0
	mov r2, r8
	bl sub_0800447c
	adds r0, r7, #0
	mov r1, r8
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	add r5, sp, #80
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #5
	movs r0, #128
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r3, [pc, #712]
	movs r0, #128
	adds r1, r6, r3
	lsls r0, r0, #12
	adds r2, r5, #0
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #6
	adds r1, r6, r0
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r2, [pc, #640]
	movs r0, #128
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	beq .L_0800ef44
.L_0800ef24:
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #6
	blt .L_0800ee50
	ldr r3, [r7, #8]
	mov r1, r11
	str r3, [r1, #0]
	ldr r3, [r7, #12]
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	str r3, [r1, #8]
	ldr r2, [sp, #20]
	movs r3, #1
	orrs r2, r3
	str r2, [sp, #20]
.L_0800ef44:
	add r3, sp, #68
	mov r11, r3
	ldr r3, [r7, #8]
	mov r0, r11
	str r3, [r0, #0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	ldr r2, [sp, #0]
	movs r0, #128
	lsrs r1, r2, #16
	lsls r0, r0, #11
	mov r2, r11
	bl sub_0800447c
	ldr r3, [pc, #556]
	ldr r3, [r3, #0]
	mov r8, r3
	mov r6, r8
	movs r3, #63
	mov r9, r3
	adds r6, #8
.L_0800ef72:
	ldrh r3, [r7, #32]
	mov r0, r8
	subs r1, r3, #2
	ldr r3, [r0, #0]
	cmp r3, #0
	bne .L_0800ef80
	b .L_0800f09a
.L_0800ef80:
	mov r3, r8
	adds r3, #89
	ldrb r2, [r3, #0]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0800ef90
	b .L_0800f09a
.L_0800ef90:
	cmp r8, r7
	bne .L_0800ef96
	b .L_0800f09a
.L_0800ef96:
	ldrh r3, [r6, #24]
	adds r0, r6, #0
	subs r3, #2
	mov r2, r11
	bl sub_0800eba0
	cmp r0, #0
	blt .L_0800f09a
	ldr r3, [r6, #80]
	ldr r2, [pc, #492]
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	cmp r3, r1
	bne .L_0800f092
	ldr r3, [r7, #16]
	ldr r0, [r6, #8]
	ldr r1, [r6, #0]
	subs r0, r0, r3
	ldr r3, [r7, #8]
	subs r1, r1, r3
	bl sub_080044d0
	ldr r3, [r6, #0]
	add r5, sp, #80
	str r3, [r5, #0]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	lsls r0, r0, #16
	ldr r3, [r6, #8]
	asrs r2, r0, #16
	lsrs r0, r0, #16
	mov r10, r0
	movs r0, #128
	str r3, [r5, #8]
	lsls r0, r0, #7
	str r2, [sp, #12]
	mov r1, r10
	adds r2, r5, #0
	bl sub_0800447c
	mov r0, r8
	adds r1, r5, #0
	bl sub_0800d924
	cmp r0, #0
	bne .L_0800f092
	ldr r3, [r6, #0]
	str r3, [r5, #0]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	ldr r3, [r6, #8]
	movs r0, #160
	lsls r0, r0, #12
	mov r1, r10
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	mov r0, r8
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800f092
	ldr r3, [r6, #0]
	str r3, [r5, #0]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	movs r1, #128
	ldr r3, [r6, #8]
	lsls r1, r1, #5
	movs r0, #160
	add r1, r10
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	mov r0, r8
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800f092
	mov r0, r8
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800f092
	ldr r3, [r6, #0]
	str r3, [r5, #0]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	ldr r1, [pc, #304]
	ldr r3, [r6, #8]
	movs r0, #160
	add r1, r10
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	mov r0, r8
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne .L_0800f092
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	adds r2, r6, #0
	bl sub_0800447c
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r3, [r6, #56]
	ldr r3, [sp, #16]
	movs r0, #1
	orrs r3, r0
	str r3, [sp, #16]
	b .L_0800f09a
.L_0800f092:
	ldr r1, [sp, #20]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #20]
.L_0800f09a:
	movs r2, #1
	negs r2, r2
	add r9, r2
	movs r3, #112
	mov r0, r9
	adds r6, #112
	add r8, r3
	cmp r0, #0
	blt .L_0800f0ae
	b .L_0800ef72
.L_0800f0ae:
	ldr r1, [sp, #20]
	cmp r1, #0
	bne .L_0800f0c6
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_0800f0c6
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
.L_0800f0c6:
	ldr r3, [pc, #212]
	ldr r3, [r3, #0]
	cmp r3, #0
	beq .L_0800f0ee
	ldr r0, [sp, #20]
	movs r2, #3
	ands r2, r0
	cmp r2, #0
	beq .L_0800f0e6
	movs r1, #206
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2, #0]
	adds r3, #1
	strh r3, [r2, #0]
	b .L_0800f0ee
.L_0800f0e6:
	movs r0, #206
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r2, [r3, #0]
.L_0800f0ee:
	ldr r1, [sp, #16]
	cmp r1, #0
	beq .L_0800f0fe
	adds r0, r7, #0
	movs r1, #8
	bl sub_0800c300
	b .L_0800f130
.L_0800f0fe:
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_0800f128
	ldr r3, [pc, #152]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3, #0]
	bl sub_08077008
	movs r1, #56
	ldrsh r3, [r0, r1]
	movs r5, #9
	cmp r3, #0
	bne .L_0800f11e
	movs r5, #22
.L_0800f11e:
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_0800c300
	b .L_0800f130
.L_0800f128:
	adds r0, r7, #0
	ldr r1, [sp, #8]
	bl sub_0800c300
.L_0800f130:
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_0800f1a4
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_0800f170
	ldr r0, [sp, #4]
	ldrh r1, [r7, #6]
	lsrs r3, r0, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0800f164
	adds r3, r2, #0
.L_0800f164:
	ldr r2, [pc, #32]
	cmp r3, r2
	bge .L_0800f16c
	adds r3, r2, #0
.L_0800f16c:
	adds r3, r1, r3
	strh r3, [r7, #6]
.L_0800f170:
	movs r1, #100
	adds r1, r1, r7
	mov r10, r1
	movs r3, #0
	mov r2, r10
	strh r3, [r2, #0]
	adds r2, r7, #0
	adds r2, #102
	movs r3, #2
	strh r3, [r2, #0]
	b sub_0800f1fa
	.2byte 0x0000
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0x03001e64
	.4byte 0xff000200
	.4byte 0x03001ebc
	.4byte 0x02000240
.L_0800f1a4:
	add r3, sp, #92
	ldr r1, [r3, #0]
	ldr r2, [r3, #4]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl sub_0800d14c
	ldr r1, [r7, #36]
	ldr r4, [pc, #252]
	adds r0, r1, #0
	mov r12, pc
	bx r4
