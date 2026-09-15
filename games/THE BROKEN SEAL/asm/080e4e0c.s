@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080d4604, 0x080d4604
	.set sub_080d52a4, 0x080d52a4
	.set sub_080dea70, 0x080dea70
	.set sub_080e0524, 0x080e0524
	.set sub_080e396c, 0x080e396c
	.set sub_080e551a, 0x080e551a
	.set sub_080e657c, 0x080e657c
	.set sub_080e65f8, 0x080e65f8
	.global Region_080e4e0c
Region_080e4e0c:
	ldr r1, [sp, #96]
	cmp r1, #32
	bne .L_080e4e58
	ldr r2, [pc, #36]
	ldr r3, [pc, #28]
	strh r3, [r2, #0]
	ldr r2, [sp, #92]
	ldr r5, [pc, #32]
	adds r3, r2, r5
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4e44
	ldr r0, [pc, #24]
	movs r1, #192
	lsls r1, r1, #12
	str r0, [sp, #72]
	str r1, [sp, #68]
	b .L_080e4e4e
	.2byte 0x0000
	.4byte 0x00000100
	.4byte 0x04000020
	.4byte 0x00007828
	.4byte 0xff800000
.L_080e4e44:
	movs r2, #128
	ldr r3, [pc, #88]
	lsls r2, r2, #12
	str r2, [sp, #72]
	str r3, [sp, #68]
.L_080e4e4e:
	ldr r5, [sp, #72]
	ldr r2, [pc, #80]
	asrs r3, r5, #16
	lsls r3, r3, #8
	str r3, [r2, #0]
.L_080e4e58:
	ldr r0, [sp, #96]
	cmp r0, #8
	bne .L_080e4ec0
	ldr r2, [pc, #72]
	ldr r3, [pc, #56]
	strh r3, [r2, #0]
	ldr r3, [sp, #60]
	ldr r2, [r3, #0]
	movs r3, #64
	ldr r1, [pc, #56]
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1, #0]
	ldr r5, [sp, #92]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, [pc, #48]
	adds r2, r5, r0
	movs r3, #1
	str r3, [r2, #0]
	movs r6, #0
	adds r3, r5, r1
	movs r1, #128
	str r6, [r3, #0]
	ldr r5, [pc, #36]
	lsls r1, r1, #7
	ldr r0, [pc, #36]
	bl sub_080072f8
	movs r1, #128
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	b .L_080e4eb8
	.2byte 0x0000
	.4byte 0x00000100
	.4byte 0xfff40000
	.4byte 0x04000028
	.4byte 0x04000020
	.4byte 0x00007784
	.4byte 0x03000164
	.4byte 0x06004000
.L_080e4eb8:
	bl sub_080072f8
	ldr r3, [pc, #40]
	strh r6, [r3, #0]
.L_080e4ec0:
	ldr r2, [sp, #96]
	cmp r2, #31
	bne .L_080e4f02
	ldr r3, [pc, #28]
	ldr r2, [pc, #32]
	strh r3, [r2, #0]
	ldr r5, [sp, #92]
	ldr r0, [pc, #32]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4ef4
	ldr r3, [sp, #60]
	ldr r2, [r3, #0]
	movs r3, #32
	b .L_080e4efa
	.2byte 0x0000
	.4byte 0x00000100
	.4byte 0x04000050
	.4byte 0x04000020
	.4byte 0x00007828
.L_080e4ef4:
	ldr r5, [sp, #60]
	ldr r2, [r5, #0]
	movs r3, #96
.L_080e4efa:
	ldr r1, [pc, #448]
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1, #0]
.L_080e4f02:
	ldr r0, [sp, #96]
	cmp r0, #15
	beq .L_080e4f14
	cmp r0, #17
	beq .L_080e4f14
	cmp r0, #24
	beq .L_080e4f14
	cmp r0, #26
	bne .L_080e4f86
.L_080e4f14:
	movs r1, #128
	ldr r5, [pc, #424]
	lsls r1, r1, #7
	ldr r0, [pc, #424]
	bl sub_080072f8
	movs r1, #128
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	bl sub_080072f8
	ldr r2, [pc, #412]
	ldr r1, [sp, #92]
	adds r3, r1, r2
	ldr r2, [r3, #0]
	movs r3, #0
	str r3, [r2, #28]
	ldr r0, [pc, #404]
	bl sub_08004278
	ldr r0, [pc, #400]
	bl sub_08004278
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	adds r0, r7, #0
	movs r1, #3
	bl sub_08009080
	ldr r3, [sp, #96]
	cmp r3, #15
	bne .L_080e4f64
	ldr r0, [sp, #100]
	movs r1, #9
	bl sub_080dea70
.L_080e4f64:
	ldr r5, [sp, #96]
	cmp r5, #24
	bne .L_080e4f70
	ldr r0, [sp, #100]
	bl sub_080d52a4
.L_080e4f70:
	ldr r0, [sp, #96]
	cmp r0, #26
	beq .L_080e4f7a
	bl sub_080e65f8
.L_080e4f7a:
	ldr r0, [sp, #100]
	movs r1, #8
	bl sub_080dea70
	bl sub_080e65f8
.L_080e4f86:
	adds r0, r7, #0
	movs r1, #16
	bl sub_08009088
	ldr r1, [sp, #48]
	str r1, [r7, #36]
	ldr r2, [sp, #44]
	str r2, [r7, #40]
	ldr r3, [sp, #40]
	str r3, [r7, #44]
	ldr r5, [sp, #36]
	str r5, [r7, #52]
	ldr r0, [sp, #32]
	str r0, [r7, #72]
	ldr r1, [sp, #96]
	cmp r1, #35
	bne .L_080e4ff4
	movs r1, #128
	ldr r5, [pc, #276]
	lsls r1, r1, #7
	ldr r0, [pc, #276]
	bl sub_080072f8
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [sp, #88]
	bl sub_080072f8
	ldr r5, [pc, #264]
	ldr r2, [sp, #92]
	adds r3, r2, r5
	ldr r2, [r3, #0]
	movs r3, #0
	str r3, [r2, #28]
	ldr r0, [pc, #256]
	bl sub_08004278
	ldr r0, [pc, #252]
	bl sub_08004278
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	ldr r0, [sp, #100]
	movs r3, #3
	str r3, [r0, #24]
	ldr r0, [sp, #100]
	movs r1, #2
	bl sub_080d4604
	bl sub_080e65f8
.L_080e4ff4:
	ldr r1, [sp, #92]
	ldr r2, [pc, #208]
	adds r3, r1, r2
	ldr r3, [r3, #0]
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl sub_080b5098
	ldr r0, [r0, #0]
	str r0, [sp, #28]
	mov r0, sp
	adds r0, #136
	ldr r1, [sp, #28]
	ldr r2, [sp, #8]
	str r0, [sp, #24]
	ldr r3, [r2, #0]
	ldr r0, [r1, #8]
	movs r1, #6
	subs r0, r0, r3
	bl sub_080022ec
	ldr r3, [sp, #24]
	str r0, [r3, #0]
	ldr r5, [sp, #28]
	ldr r1, [sp, #8]
	ldr r0, [r5, #12]
	ldr r3, [r1, #4]
	movs r2, #240
	lsls r2, r2, #13
	subs r0, r0, r3
	adds r0, r0, r2
	movs r1, #6
	bl sub_080022ec
	ldr r3, [sp, #24]
	str r0, [r3, #4]
	ldr r0, [r5, #16]
	ldr r5, [sp, #8]
	ldr r3, [r5, #8]
	movs r1, #6
	subs r0, r0, r3
	bl sub_080022ec
	ldr r1, [sp, #24]
	str r0, [r1, #8]
	ldr r5, [sp, #92]
	ldr r0, [pc, #128]
	movs r2, #0
	mov r10, r2
	adds r3, r5, r0
.L_080e5058:
	movs r1, #1
	add r10, r1
	mov r5, r10
	str r2, [r3, #0]
	adds r3, #28
	cmp r5, #64
	bne .L_080e5058
	ldr r0, [sp, #96]
	cmp r0, #14
	beq .L_080e5114
	ldr r1, [sp, #92]
	ldr r2, [pc, #88]
	adds r3, r1, r2
	ldr r3, [r3, #0]
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl sub_080b5070
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r1, [sp, #92]
	movs r2, #225
	asrs r7, r0, #1
	lsls r2, r2, #7
	movs r0, #0
	mov r10, r0
	movs r6, #255
	adds r5, r1, r2
.L_080e5090:
	ldr r0, [sp, #28]
	ldr r3, [r0, #8]
	str r7, [r5, #4]
	str r3, [r5, #0]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #96]
	cmp r1, #31
	bne .L_080e50d8
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #10
	b .L_080e50ee
	.2byte 0x0000
	.4byte 0x04000028
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x00007828
	.4byte 0x080cd4b5
	.4byte 0x080cd261
	.4byte 0x00007098
.L_080e50d8:
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #12
.L_080e50ee:
	str r0, [r5, #16]
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #32
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #28
	cmp r0, #32
	bne .L_080e5090
.L_080e5114:
	ldr r1, [sp, #96]
	cmp r1, #11
	bne .L_080e5138
	ldr r1, [sp, #92]
	ldr r0, [pc, #68]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	movs r2, #1
	movs r3, #0
	ldr r0, [pc, #60]
	ldr r1, [pc, #60]
	bl sub_080e0524
	ldr r2, [pc, #60]
	ldr r3, [pc, #40]
	strh r3, [r2, #0]
.L_080e5138:
	ldr r2, [sp, #96]
	cmp r2, #32
	bne .L_080e517c
	ldr r1, [sp, #92]
	ldr r0, [pc, #48]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	movs r2, #1
	movs r3, #0
	ldr r0, [pc, #40]
	ldr r1, [pc, #24]
	bl sub_080e0524
	ldr r2, [pc, #24]
	ldr r3, [pc, #4]
	strh r3, [r2, #0]
	b .L_080e517c
	.2byte 0x0000
	.4byte 0x00000e10
	.4byte 0x000000ab
	.4byte 0x000000ac
	.4byte 0x02010000
	.4byte 0x04000052
	.4byte 0x000000ad
	.4byte 0x000000ae
.L_080e517c:
	ldr r3, [sp, #96]
	cmp r3, #7
	beq .L_080e523e
	cmp r3, #13
	beq .L_080e523e
	cmp r3, #18
	beq .L_080e523e
	cmp r3, #11
	beq .L_080e523e
	cmp r3, #32
	beq .L_080e523e
	cmp r3, #19
	beq .L_080e523e
	movs r7, #0
	cmp r3, #12
	beq .L_080e51a0
	movs r7, #160
	lsls r7, r7, #13
.L_080e51a0:
	movs r5, #0
	mov r10, r5
	ldr r5, [pc, #804]
	movs r6, #255
.L_080e51a8:
	ldr r0, [sp, #28]
	ldr r3, [r0, #8]
	str r7, [r5, #4]
	str r3, [r5, #0]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #96]
	cmp r1, #5
	beq .L_080e51be
	cmp r1, #23
	bne .L_080e51e0
.L_080e51be:
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	b .L_080e522c
.L_080e51e0:
	ldr r2, [sp, #96]
	cmp r2, #25
	bne .L_080e520a
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	b .L_080e522c
.L_080e520a:
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #10
.L_080e522c:
	str r0, [r5, #20]
	movs r3, #0
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_080e51a8
.L_080e523e:
	ldr r1, [sp, #96]
	subs r1, #2
	str r1, [sp, #20]
	cmp r1, #1
	bls .L_080e525a
	ldr r2, [sp, #96]
	cmp r2, #12
	beq .L_080e525a
	cmp r2, #22
	beq .L_080e525a
	cmp r2, #29
	beq .L_080e525a
	cmp r2, #28
	bne .L_080e5264
.L_080e525a:
	movs r1, #144
	ldr r0, [pc, #624]
	lsls r1, r1, #3
	bl sub_080041d8
.L_080e5264:
	ldr r3, [sp, #96]
	subs r3, #4
	str r3, [sp, #16]
	cmp r3, #2
	bls .L_080e5288
	ldr r5, [sp, #96]
	cmp r5, #23
	beq .L_080e5288
	cmp r5, #30
	beq .L_080e5288
	cmp r5, #27
	beq .L_080e5288
	cmp r5, #33
	beq .L_080e5288
	cmp r5, #34
	beq .L_080e5288
	cmp r5, #100
	bne .L_080e528e
.L_080e5288:
	movs r0, #32
	str r0, [sp, #64]
	b .L_080e52ea
.L_080e528e:
	ldr r1, [sp, #96]
	cmp r1, #3
	bls .L_080e52b4
	cmp r1, #8
	beq .L_080e52b4
	cmp r1, #9
	beq .L_080e52b4
	cmp r1, #10
	beq .L_080e52b4
	cmp r1, #22
	beq .L_080e52b4
	cmp r1, #25
	beq .L_080e52b4
	cmp r1, #29
	beq .L_080e52b4
	cmp r1, #31
	beq .L_080e52b4
	cmp r1, #14
	bne .L_080e52ba
.L_080e52b4:
	movs r2, #48
	str r2, [sp, #64]
	b .L_080e52ea
.L_080e52ba:
	ldr r5, [sp, #96]
	movs r3, #20
	str r3, [sp, #64]
	cmp r5, #21
	beq .L_080e52ea
	ldr r0, [sp, #96]
	cmp r0, #11
	beq .L_080e52d2
	cmp r0, #32
	beq .L_080e52d2
	cmp r0, #20
	bne .L_080e52d8
.L_080e52d2:
	movs r1, #40
	str r1, [sp, #64]
	b .L_080e52ea
.L_080e52d8:
	ldr r2, [sp, #96]
	cmp r2, #28
	beq .L_080e52e6
	movs r3, #80
	str r3, [sp, #64]
	cmp r2, #12
	bne .L_080e52ea
.L_080e52e6:
	movs r5, #64
	str r5, [sp, #64]
.L_080e52ea:
	ldr r1, [sp, #64]
	movs r0, #0
	str r0, [sp, #84]
	cmp r1, #0
	bne .L_080e52f8
	bl sub_080e657c
.L_080e52f8:
	ldr r2, [sp, #96]
	cmp r2, #11
	beq .L_080e5334
	cmp r2, #32
	beq .L_080e5334
	ldr r3, [sp, #92]
	movs r5, #211
	ldr r1, [sp, #84]
	lsls r5, r5, #7
	movs r0, #0
	movs r7, #128
	adds r6, r3, r5
	mov r10, r0
	lsls r7, r7, #11
	lsls r5, r1, #12
.L_080e5316:
	adds r0, r5, #0
	bl sub_08002322
	lsls r0, r0, #2
	subs r0, r7, r0
	movs r3, #1
	asrs r0, r0, #10
	movs r2, #128
	add r10, r3
	stmia r6!, {r0}
	lsls r2, r2, #4
	mov r0, r10
	adds r5, r5, r2
	cmp r0, #160
	bne .L_080e5316
.L_080e5334:
	ldr r1, [sp, #84]
	cmp r1, #2
	bgt .L_080e535c
	ldr r2, [sp, #92]
	ldr r5, [pc, #404]
	adds r3, r2, r5
	ldr r3, [r3, #0]
	ldr r1, [sp, #56]
	ldr r0, [r3, #8]
	bl sub_080e396c
	ldr r0, [sp, #56]
	ldr r3, [r0, #0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r0, #0]
	ldr r3, [r0, #4]
	adds r3, #16
	str r3, [r0, #4]
.L_080e535c:
	ldr r1, [sp, #96]
	cmp r1, #11
	beq .L_080e53e2
	cmp r1, #8
	beq .L_080e53e2
	cmp r1, #32
	beq .L_080e53e2
	cmp r1, #33
	beq .L_080e53ea
	cmp r1, #34
	beq .L_080e53e2
	ldr r2, [sp, #84]
	cmp r2, #11
	bgt .L_080e53e2
	ldr r5, [sp, #92]
	ldr r0, [pc, #344]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e53b4
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #56]
	movs r0, #48
	ldr r2, [r3, #0]
	ldr r3, [r3, #4]
	lsls r1, r1, #7
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	adds r1, r5, r1
	subs r2, #32
	subs r3, #40
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
	b .L_080e53e2
.L_080e53b4:
	ldr r5, [sp, #84]
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r0, [sp, #92]
	ldr r3, [sp, #56]
	lsls r1, r1, #7
	ldr r2, [r3, #0]
	adds r1, r0, r1
	ldr r3, [r3, #4]
	movs r0, #48
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	subs r3, #40
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
.L_080e53e2:
	ldr r5, [sp, #96]
	cmp r5, #33
	bls .L_080e53ea
	b sub_080e551a
.L_080e53ea:
	ldr r0, [sp, #96]
	ldr r2, [pc, #232]
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
