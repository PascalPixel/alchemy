.syntax unified
	.thumb
	.global WaitFrames
	.thumb_func
WaitFrames:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	movs r1, #0
	str r0, [sp, #4]
	str r1, [sp, #0]
	cmp r1, r0
	bcc .L_0801357c
	b .L_08013880
.L_0801357c:
	ldr r2, .L_08013660
	ldr r3, .L_08013664
	mov r11, r2
	mov r10, r1
	mov r9, r3
.L_08013586:
	ldr r5, .L_08013668
	movs r1, #1
	movs r0, #144
	strb r1, [r5]
	lsls r0, r0, #3
	bl Func_080147d8
	movs r3, #0
	strb r3, [r5]
	ldr r3, .L_0801366c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080135b0
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #80
	bl Runtime_AllocateHeapBlock
	bl Render_BuildOamList
	b .L_080135ba
.L_080135b0:
	ldr r2, .L_08013670
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #1
	strh r3, [r2, #2]
.L_080135ba:
	ldr r3, .L_08013674
	movs r2, #1
	strb r2, [r3]
	ldr r3, .L_08013678
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08013606
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #6
	ldrh r1, [r3]
	cmp r1, #159
	bls .L_080135d8
	subs r1, #160
	b .L_080135da
.L_080135d8:
	adds r1, #68
.L_080135da:
	ldr r3, .L_0801367c
	ldr r0, .L_08013680
	ldrh r3, [r3]
	ldr r2, [r0]
	subs r3, #1
	lsls r3, r3, #8
	adds r1, r1, r3
	cmp r2, #0
	bne .L_080135f2
	ldr r3, .L_08013684
	str r2, [r3]
	b .L_080135f6
.L_080135f2:
	subs r3, r2, #1
	str r3, [r0]
.L_080135f6:
	ldr r2, .L_08013684
	ldr r3, [r2]
	cmp r3, r1
	bcs .L_08013606
	str r1, [r2]
	ldr r2, .L_08013680
	movs r3, #30
	str r3, [r2]
.L_08013606:
	ldr r3, .L_08013688
	ldrb r3, [r3]
	adds r2, r3, #0
	cmp r2, #0
	bne .L_08013654
	ldr r3, .L_0801368c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08013642
	mov r1, r11
	ldr r3, [r1]
	cmp r3, #0
	beq .L_08013626
	mov r3, r9
	strh r2, [r3]
	b .L_08013642
.L_08013626:
	mov r1, r9
	ldrh r3, [r1]
	mov r2, r9
	adds r3, #1
	strh r3, [r2]
	movs r3, #168
	ldrh r2, [r2]
	lsls r3, r3, #6
	adds r3, #48
	cmp r2, r3
	bls .L_08013642
	ldr r3, .L_08013690
	movs r1, #1
	strb r1, [r3]
.L_08013642:
	mov r2, r11
	ldr r3, [r2]
	movs r1, #193
	lsls r1, r1, #2
	cmp r3, r1
	bne .L_08013654
	ldr r3, .L_08013690
	movs r2, #1
	strb r2, [r3]
.L_08013654:
	ldr r3, .L_08013694
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080136fa
	ldr r5, .L_08013698
	b .L_080136d2
.L_08013660:
	.4byte gInput
.L_08013664:
	.4byte Data_03001218
.L_08013668:
	.4byte Data_03001108
.L_0801366c:
	.4byte Data_0300120c
.L_08013670:
	.4byte gOamUsage
.L_08013674:
	.4byte Data_03001230
.L_08013678:
	.4byte Data_0300123c
.L_0801367c:
	.4byte Data_030011d4
.L_08013680:
	.4byte Data_03001140
.L_08013684:
	.4byte Data_03001184
.L_08013688:
	.4byte Data_03001180
.L_0801368c:
	.4byte Data_03001200
.L_08013690:
	.4byte Data_030011d0
.L_08013694:
	.4byte Data_03001238
.L_08013698:
	.4byte Data_03001214
.L_0801369c:
	mov r1, r11
	ldr r3, [r1]
	cmp r3, #12
	bne .L_080136fa
	movs r2, #1
	strb r2, [r5]
.L_080136a8:
	bl Func_080134b0
	bl Func_080138b4
	ldr r2, .L_080137cc
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_080136d2
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_080137d0
	ldr r3, .L_080137d4
	str r3, [r2]
	bl Func_081c0080
	movs r4, #128
	lsls r4, r4, #20
	ldr r3, .L_080137d8
	mov r1, r10
	strh r1, [r3]
	bx r4
.L_080136d2:
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0801369c
	ldr r0, .L_080137dc
	movs r2, #7
	ldr r3, [r0, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_080136fa
	ldr r1, [r0]
	movs r3, #240
	ands r1, r3
	cmp r1, #0
	bne .L_080136fa
	ldr r3, [r0, #12]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080136a8
	strb r1, [r5]
.L_080136fa:
	ldr r2, .L_080137e0
	ldr r1, .L_080137e4
	ldrh r3, [r2]
	strh r3, [r1]
	mov r3, r10
	strh r3, [r2]
	bl Func_080134b0
	movs r0, #80
	bl Runtime_ReleaseHeapBlock
	bl Func_08013ffc
	ldr r2, .L_080137e8
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	ldr r2, .L_080137ec
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	bl Func_080138b4
	ldr r3, .L_080137f0
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_08013740
	bl Func_08016430
	ldr r2, .L_080137f4
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08013740
	movs r3, #1
	strb r3, [r2, #8]
.L_08013740:
	ldr r1, .L_080137f8
	ldrb r3, [r1]
	cmp r3, #0
	bne .L_0801374a
	b .L_08013850
.L_0801374a:
	ldr r3, .L_080137fc
	ldrb r3, [r3]
	adds r2, r3, #0
	cmp r2, #0
	beq .L_08013756
	b .L_08013850
.L_08013756:
	movs r4, #128
	lsls r4, r4, #19
	ldrh r3, [r4]
	movs r0, #160
	lsls r3, r3, #16
	lsls r0, r0, #19
	asrs r7, r3, #16
	ldrh r3, [r0]
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r8, r3
	ldrb r3, [r1]
	cmp r3, #1
	bne .L_0801384a
	strh r2, [r4]
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	strh r3, [r0]
	movs r5, #9
.L_0801377e:
	subs r5, #1
	bl Func_080134b0
	cmp r5, #0
	bge .L_0801377e
	mov r1, r11
	ldr r3, [r1]
	cmp r3, #0
	beq .L_0801379c
	ldr r5, .L_080137dc
.L_08013792:
	bl Func_080134b0
	ldr r3, [r5]
	cmp r3, #0
	bne .L_08013792
.L_0801379c:
	ldr r6, .L_08013800
	ldr r3, .L_080137c8
	strh r3, [r6]
	movs r3, #195
	ldr r5, .L_08013804
	lsls r3, r3, #8
	adds r3, #4
	strh r3, [r5]
	bl Bios_SoundBiasOff
	swi #3
	bl Bios_SoundBiasOn
	movs r3, #192
	lsls r3, r3, #8
	adds r3, #15
	strh r3, [r5]
	mov r2, r10
	strh r2, [r6]
	movs r5, #9
	b .L_08013808
	.2byte 0x0000
.L_080137c8:
	.4byte 0x00000001
.L_080137cc:
	.4byte Data_030011c0
.L_080137d0:
	.4byte Data_03007800
.L_080137d4:
	.4byte 0x19670704
.L_080137d8:
	.4byte 0x04000208
.L_080137dc:
	.4byte gInput
.L_080137e0:
	.4byte Data_030011d4
.L_080137e4:
	.4byte Data_030011d8
.L_080137e8:
	.4byte Data_0300122c
.L_080137ec:
	.4byte Data_0300117c
.L_080137f0:
	.4byte Data_030011b8
.L_080137f4:
	.4byte Data_02005360
.L_080137f8:
	.4byte Data_030011d0
.L_080137fc:
	.4byte Data_03001180
.L_08013800:
	.4byte Data_02003000
.L_08013804:
	.4byte 0x04000132
.L_08013808:
	subs r5, #1
	bl Func_080134b0
	cmp r5, #0
	bge .L_08013808
	mov r1, r11
	ldr r3, [r1]
	cmp r3, #0
	beq .L_08013826
	ldr r5, .L_08013890
.L_0801381c:
	bl Func_080134b0
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0801381c
.L_08013826:
	lsls r3, r7, #16
	movs r2, #128
	lsrs r3, r3, #16
	lsls r2, r2, #19
	strh r3, [r2]
	mov r2, r8
	lsls r3, r2, #16
	movs r2, #160
	lsrs r3, r3, #16
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r2, .L_08013894
	movs r3, #0
	strb r3, [r2]
	mov r1, r9
	mov r3, r10
	strh r3, [r1]
	b .L_08013850
.L_0801384a:
	ldrb r3, [r1]
	adds r3, #255
	strb r3, [r1]
.L_08013850:
	ldr r2, .L_08013898
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08013872
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_0801389c
	ldr r3, .L_080138a0
	str r3, [r2]
	bl Func_081c0080
	movs r4, #128
	lsls r4, r4, #20
	ldr r3, .L_080138a4
	mov r2, r10
	strh r2, [r3]
	bx r4
.L_08013872:
	ldr r3, [sp, #0]
	ldr r1, [sp, #4]
	adds r3, #1
	str r3, [sp, #0]
	cmp r3, r1
	bcs .L_08013880
	b .L_08013586
.L_08013880:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08013890:
	.4byte gInput
.L_08013894:
	.4byte Data_030011d0
.L_08013898:
	.4byte Data_030011c0
.L_0801389c:
	.4byte Data_03007800
.L_080138a0:
	.4byte 0x19670704
.L_080138a4:
	.4byte 0x04000208
