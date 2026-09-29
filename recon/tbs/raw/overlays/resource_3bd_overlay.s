.syntax unified
	.thumb
	.section .text.x0200909c,"ax",%progbits
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	sub	sp, #4
	mov	r9, r2
	adds	r7, r0, #0
	bl 0x0200bcb0
	bl 0x0200bde8
	ldr	r0, [pc, #792]
	bl 0x0200bd60
	movs	r0, #16
	movs	r1, #0
	bl 0x0200bd70
	movs	r6, #128
	bl 0x0200bcb8
	movs	r4, #0
	mov	r8, r4
	lsls	r6, r6, #9
.L_020010d4:
	adds	r0, r4, #0
	adds	r0, #11
	str	r4, [sp, #0]
	bl 0x0200bcc8
	ldr	r4, [sp, #0]
	adds	r5, r0, #0
	mov	r3, r8
	adds	r4, #1
	str	r3, [r5, #108]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	cmp	r4, #4
	ble.n	.L_020010d4
	ldr	r3, [pc, #740]
	ldrb	r2, [r3, #0]
	mov	r8, r2
	ldr	r2, [pc, #740]
	movs	r6, #1
	ldrsb	r6, [r3, r6]
	ldr	r5, [r2, #0]
	mov	sl, r2
	ldrb	r2, [r3, #1]
	lsls	r0, r6, #16
	movs	r1, #5
	mov	fp, r2
	bl 0x0200bbe8
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r2, r8
	adds	r0, r0, r3
	lsls	r3, r2, #24
	asrs	r3, r3, #24
	strh	r0, [r5, #6]
	cmp	r3, #0
	bne.n	.L_0200113c
	cmp	r7, #16
	bne.n	.L_0200112e
	movs	r3, #1
	movs	r0, #110
	mov	r8, r3
	bl 0x0200be70
	b.n	.L_02001134
.L_0200112e:
	movs	r0, #114
	bl 0x0200be70
.L_02001134:
	ldr	r2, [pc, #680]
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L_02001256
.L_0200113c:
	cmp	r3, #1
	bne.n	.L_020011da
	cmp	r7, #16
	bne.n	.L_0200114c
	movs	r0, #110
	bl 0x0200be70
	b.n	.L_02001256
.L_0200114c:
	cmp	r7, #20
	bne.n	.L_020011ce
	movs	r2, #2
	movs	r0, #110
	mov	r8, r2
	bl 0x0200be70
	movs	r0, #30
	bl 0x0200bc00
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r2, #6
	ldrsh	r7, [r3, r2]
	ldr	r3, [pc, #632]
	movs	r4, #0
	mov	sl, r3
.L_0200116e:
	adds	r5, r4, #0
	adds	r5, #11
	lsls	r7, r7, #16
	movs	r1, #192
	lsrs	r2, r7, #16
	adds	r0, r5, #0
	lsls	r1, r1, #13
	str	r4, [sp, #0]
	bl SceneActor_SetPositionFromTransformedBase
	movs	r0, #151
	bl 0x0200be70
	adds	r0, r5, #0
	bl 0x0200bcc8
	movs	r3, #0
	adds	r5, r0, #0
	str	r3, [r5, #24]
	ldr	r6, [pc, #592]
	ldr	r4, [sp, #0]
.L_02001198:
	str	r6, [r5, #28]
	str	r6, [r5, #24]
	movs	r0, #1
	str	r4, [sp, #0]
	bl 0x0200bc00
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r6, r6, r2
	ldr	r3, [r5, #24]
	ldr	r2, [pc, #572]
	ldr	r4, [sp, #0]
	cmp	r3, r2
	ble.n	.L_02001198
	lsrs	r3, r7, #16
	add	r3, sl
	lsls	r3, r3, #16
	adds	r4, #1
	asrs	r7, r3, #16
	cmp	r4, #4
	ble.n	.L_0200116e
	movs	r0, #30
	bl 0x0200bc00
	movs	r3, #1
	mov	r9, r3
	b.n	.L_02001256
.L_020011ce:
	movs	r0, #114
	bl 0x0200be70
	movs	r2, #0
	mov	r8, r2
	b.n	.L_02001256
.L_020011da:
	cmp	r3, #2
	bne.n	.L_02001256
	adds	r3, r6, #0
	adds	r3, #16
	cmp	r7, r3
	beq.n	.L_02001246
	movs	r3, #0
	movs	r0, #114
	mov	r8, r3
	bl 0x0200be70
	movs	r0, #30
	bl 0x0200bc00
	movs	r4, #0
.L_020011f8:
	adds	r7, r4, #0
	adds	r7, #11
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl 0x0200bcc8
	adds	r5, r0, #0
	movs	r0, #151
	bl 0x0200be70
	ldr	r6, [r5, #24]
	ldr	r2, [pc, #472]
	ldr	r4, [sp, #0]
	cmp	r6, r2
	ble.n	.L_02001230
.L_02001216:
	str	r6, [r5, #28]
	str	r6, [r5, #24]
	movs	r0, #1
	str	r4, [sp, #0]
	bl 0x0200bc00
	ldr	r3, [pc, #460]
	ldr	r2, [pc, #448]
	adds	r6, r6, r3
	ldr	r3, [r5, #24]
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bgt.n	.L_02001216
.L_02001230:
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #0
	str	r4, [sp, #0]
	bl 0x0200bd18
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #4
	ble.n	.L_020011f8
	b.n	.L_02001256
.L_02001246:
	movs	r0, #110
	bl 0x0200be70
	movs	r3, #1
	movs	r0, #30
	mov	r9, r3
	bl 0x0200bc00
.L_02001256:
	ldr	r7, [pc, #384]
	mov	r2, r8
	mov	r3, r9
	strb	r2, [r7, #0]
	cmp	r3, #0
	bne.n	.L_02001264
	b.n	.L_020013c0
.L_02001264:
	subs	r3, r7, #1
	ldrb	r5, [r3, #0]
	adds	r5, #1
	strb	r5, [r3, #0]
	bl 0x0200bc18
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	add	r0, fp
	adds	r0, #1
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	movs	r1, #5
	adds	r0, #5
	bl 0x0200bbf0
	ldr	r6, [pc, #340]
	strb	r0, [r7, #1]
	ldr	r2, [r6, #0]
	movs	r3, #0
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	movs	r3, #128
	lsls	r3, r3, #2
	strh	r3, [r2, #8]
	movs	r3, #192
	lsls	r5, r5, #24
	lsls	r3, r3, #6
	movs	r1, #200
	lsrs	r5, r5, #24
	strh	r3, [r2, #10]
	ldr	r0, [pc, #336]
	lsls	r1, r1, #4
	bl 0x0200bc08
	cmp	r5, #2
	bhi.n	.L_020012d8
	ldr	r3, [r6, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	beq.n	.L_020012ca
	adds	r5, r6, #0
.L_020012ba:
	movs	r0, #1
	bl 0x0200bc00
	ldr	r3, [r5, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_020012ba
.L_020012ca:
	movs	r0, #10
	bl 0x0200bc00
	movs	r0, #110
	bl 0x0200be70
	b.n	.L_020013ba
.L_020012d8:
	movs	r3, #99
	strb	r3, [r7, #0]
	ldr	r3, [r6, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	beq.n	.L_020012f8
	adds	r5, r6, #0
.L_020012e8:
	movs	r0, #1
	bl 0x0200bc00
	ldr	r3, [r5, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_020012e8
.L_020012f8:
	ldr	r2, [r6, #0]
	movs	r3, #2
	movs	r1, #0
	strh	r3, [r2, #0]
	strh	r1, [r2, #2]
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bc68
	movs	r0, #20
	bl 0x0200bca8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200bc68
	ldr	r2, [r6, #0]
	movs	r3, #99
	strh	r3, [r2, #0]
.L_0200132e:
	movs	r0, #190
	bl 0x0200be70
	movs	r3, #192
	lsls	r3, r3, #13
	mov	r8, r3
	ldr	r3, [r6, #0]
	movs	r2, #6
	ldrsh	r7, [r3, r2]
	movs	r3, #192
	lsls	r3, r3, #4
	mov	sl, r3
.L_02001346:
	movs	r4, #0
.L_02001348:
	adds	r6, r4, #0
	adds	r6, #11
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl 0x0200bcc8
	adds	r5, r0, #0
	ldr	r3, [r5, #24]
	subs	r3, #16
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	subs	r3, #16
	str	r3, [r5, #28]
	lsls	r5, r7, #16
	lsrs	r5, r5, #16
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	r1, r8
	bl SceneActor_SetPositionFromTransformedBase
	ldr	r2, [pc, #112]
	ldr	r4, [sp, #0]
	adds	r5, r5, r2
	lsls	r5, r5, #16
	adds	r4, #1
	asrs	r7, r5, #16
	cmp	r4, #4
	ble.n	.L_02001348
	lsls	r3, r7, #16
	lsrs	r3, r3, #16
	add	r3, sl
	lsls	r3, r3, #16
	add	r8, r2
	movs	r0, #1
	asrs	r7, r3, #16
	bl 0x0200bc00
	mov	r3, r8
	cmp	r3, #0
	bgt.n	.L_02001346
	movs	r4, #0
.L_0200139a:
	adds	r0, r4, #0
	adds	r0, #11
	movs	r1, #0
	movs	r2, #0
	str	r4, [sp, #0]
	bl 0x0200bd18
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #4
	ble.n	.L_0200139a
	bl ArutamiraDou_ReleaseWallBurst
	movs	r0, #80
	bl 0x0200be70
.L_020013ba:
	ldr	r0, [pc, #56]
	bl 0x0200bc10
.L_020013c0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x000021db
	.4byte 0x02001001
	.4byte 0x0200bf6c
	.4byte 0x02001000
	.4byte 0xffffcccd
	.4byte 0x00006666
	.4byte 0x0000ffff
	.4byte 0xfffff400
	.2byte 0x8f95
	.2byte 0x0200
	.section .rodata.part1,"a",%progbits
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.global ArutamiraDou_ClearTarget
ArutamiraDou_ClearTarget:
	.4byte 0x0200c904
	.global ArutamiraDou_SceneTableA
ArutamiraDou_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0xc00001f8
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00000210
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x40000168
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00000210
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0xc00002b8
	.4byte 0x00000000
	.4byte 0x00f00228
	.4byte 0x000002d0
	.4byte 0xffff0004
	.4byte 0x00000078
	.4byte 0x40000258
	.4byte 0x00000000
	.4byte 0x00f00228
	.4byte 0x000002d0
	.4byte 0xffff000a
	.4byte 0x00000188
	.4byte 0x400001f8
	.4byte 0x00300000
	.4byte 0x01f00010
	.4byte 0x000002a0
	.4byte 0xffff000b
	.4byte 0x00000088
	.4byte 0x40000068
	.4byte 0x00300000
	.4byte 0x01f00010
	.4byte 0x000002a0
	.4byte 0xffff0014
	.4byte 0x000000e8
	.4byte 0x400000d8
	.4byte 0x00400000
	.4byte 0x02500030
	.4byte 0x00000240
	.4byte 0xffff0015
	.4byte 0x00000148
	.4byte 0x40000108
	.4byte 0x00400000
	.4byte 0x02500030
	.4byte 0x00000240
	.4byte 0xffff001e
	.4byte 0x000000c8
	.4byte 0x400001a8
	.4byte 0x00300000
	.4byte 0x02400050
	.4byte 0x000001e0
	.4byte 0xffff001f
	.4byte 0x00000208
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x02400050
	.4byte 0x000001e0
	.4byte 0xffff0028
	.4byte 0x000002a8
	.4byte 0x40000098
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000200
	.4byte 0xffff0029
	.4byte 0x00000028
	.4byte 0x400000f8
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000200
	.4byte 0xffff0032
	.4byte 0x00000128
	.4byte 0x40000188
	.4byte 0x00a00000
	.4byte 0x02400130
	.4byte 0x000002c0
	.4byte 0xffff0033
	.4byte 0x000001a8
	.4byte 0x40000188
	.4byte 0x00a00000
	.4byte 0x02400130
	.4byte 0x000002c0
	.4byte 0xffff0034
	.4byte 0x000001f8
	.4byte 0xc00000e8
	.4byte 0x01800000
	.4byte 0x02700040
	.4byte 0x000000f0
	.4byte 0xffff0035
	.4byte 0x000001f8
	.4byte 0x40000098
	.4byte 0x01800000
	.4byte 0x02700040
	.4byte 0x000000f0
	.4byte 0xffff0036
	.4byte 0x000000b8
	.4byte 0xc00000d8
	.4byte 0x00400000
	.4byte 0x01300040
	.4byte 0x000000f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ArutamiraDou_SceneTableB
ArutamiraDou_SceneTableB:
	.4byte 0x00000092
	.4byte 0x00118002
	.4byte 0x00203092
	.4byte 0x00302092
	.4byte 0x0040a093
	.4byte 0x00000093
	.4byte 0x00a04092
	.4byte 0x00b14094
	.4byte 0x00000094
	.4byte 0x0140b093
	.4byte 0x0151e095
	.4byte 0x00000095
	.4byte 0x01e15094
	.4byte 0x01f28096
	.4byte 0x00000096
	.4byte 0x0281f095
	.4byte 0x02932097
	.4byte 0x00000097
	.4byte 0x03229096
	.4byte 0x03334097
	.4byte 0x03433097
	.4byte 0x03536097
	.4byte 0x03635097
	.4byte 0x000001ff
	.global gArutamiraDouPlacementsOther
gArutamiraDouPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutamiraDouPlacements2
gArutamiraDouPlacements2:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0041
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0xffff0034
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff00a3
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x0002c000
	.4byte 0xffff00a3
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutamiraDouPlacements4
gArutamiraDouPlacements4:
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0071005d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00006000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutamiraDouPlacements6
gArutamiraDouPlacements6:
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00080000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00280000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00380000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00280000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00080000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ArutamiraDou_PulseScales
ArutamiraDou_PulseScales:
	.4byte 0x00010000
	.4byte 0x00011999
	.4byte 0x00013333
	.4byte 0x00011999
	.global gAltmillerActionA
gAltmillerActionA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gAltmillerActionB
gAltmillerActionB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gAltmillerActionC
gAltmillerActionC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gAltmillerActionD
gAltmillerActionD:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gArutamiraDouEventsOther
gArutamiraDouEventsOther:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff003c
	.4byte FieldScene_RunIndexedStep0
	.4byte 0x00000002
	.4byte 0xffff003d
	.4byte FieldScene_RunIndexedStep1
	.4byte 0x00000002
	.4byte 0xffff003e
	.4byte FieldScene_RunIndexedStep2
	.4byte 0x00000002
	.4byte 0xffff003f
	.4byte FieldScene_RunIndexedStep3
	.4byte 0x00000002
	.4byte 0xffff0040
	.4byte FieldScene_RunIndexedStep4
	.4byte 0x00000002
	.4byte 0xffff0041
	.4byte FieldScene_RunIndexedStep5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte ArutamiraDou_ApplyRoomVisuals
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte SceneState_RunFlag200SetupAndPlaceActors16To20
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte ArutamiraDou_RespawnActorObject
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutamiraDouEvents2
gArutamiraDouEvents2:
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000031
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x09600008
	.4byte FieldScene_RunBranchingCutsceneSequence
	.4byte 0x00000000
	.4byte 0x0f300008
	.4byte 0x0200b4bd
	.4byte 0x00000000
	.4byte 0x09620008
	.4byte FieldScene_RunExtendedActorPresentation
	.4byte 0x00000002
	.2byte 0x0013
	.2byte 0x0961
	push	{r0, r3, r4, lr}
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
	lsls	r0, r1, #16
	lsrs	r0, r4, #5
	add	r4, sp, #276
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r0, r1
	lsrs	r0, r6, #28
.L_020046e4:
	movs	r1, #130
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	lsls	r0, r1, #16
	lsrs	r2, r4, #5
	str	r3, [sp, #996]
	lsls	r0, r0, #8
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c99
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r5, r6, #27
	movs	r5, r0
	lsls	r0, r2, #1
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.global gArutamiraDouEvents3
gArutamiraDouEvents3:
	movs	r1, r4
	movs	r0, r0
	movs	r4, r2
	.2byte 0xffff
	.2byte 0x0014
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r5, r2
	.2byte 0xffff
	.2byte 0x0015
	movs	r0, r0
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
.L_0200474c:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c99
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r4, r3, #30
	lsls	r6, r6, #2
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.global gArutamiraDouEvents4
gArutamiraDouEvents4:
	movs	r1, r4
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x001e
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r7, r3
	.2byte 0xffff
	.2byte 0x001f
	movs	r0, r0
	.2byte 0x4602
	movs	r0, r0
	lsls	r0, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	strh	r2, [r0, #48]
	movs	r0, r0
.L_02004794:
	lsls	r1, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	stmia	r6!, {r1}
	movs	r0, r0
	lsls	r2, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	lsls	r2, r0, #24
	movs	r0, r0
	lsls	r3, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	lsls	r2, r0, #8
	movs	r0, r0
	lsls	r7, r0, #1
	.2byte 0xffff
	.2byte 0x8ead
	lsls	r0, r0, #8
	lsls	r2, r0, #24
	movs	r0, r0
	lsls	r5, r1, #1
	.2byte 0xffff
	.2byte 0x8d49
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x8cf9
	lsls	r0, r0, #8
	adds	r5, r2, r0
	asrs	r0, r0, #32
	movs	r3, r1
	lsls	r1, r2, #8
	ldrh	r1, [r3, #42]
	lsls	r0, r0, #8
	adds	r5, r2, r0
	movs	r0, r0
	movs	r3, r1
.L_020047ea:
	lsls	r1, r2, #8
	ldrh	r1, [r2, #44]
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x89dd
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.global gArutamiraDouEvents5
gArutamiraDouEvents5:
	movs	r1, r4
	movs	r0, r0
	movs	r0, r5
	.2byte 0xffff
	.2byte 0x0028
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r1, r5
	.2byte 0xffff
	.2byte 0x0029
	movs	r0, r0
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c99
.L_02004836:
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r5, r3, #30
	lsls	r0, r0, #3
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.global gArutamiraDouEvents6
gArutamiraDouEvents6:
	movs	r1, r4
	movs	r0, r0
	movs	r2, r6
	.2byte 0xffff
	.2byte 0x0032
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r6
	.2byte 0xffff
	.2byte 0x0033
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r6
	.2byte 0xffff
	.2byte 0x0034
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r6
	.2byte 0xffff
	.2byte 0x0035
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r6
	.2byte 0xffff
	.2byte 0x0036
	movs	r0, r0
	lsls	r2, r0, #8
	movs	r0, r0
	lsls	r6, r0, #1
	.2byte 0xffff
	.2byte 0x8ead
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r0, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r1, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r2, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
.L_020048c0:
	movs	r3, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r4, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r0, r6, #28
	lsls	r5, r5, #3
	movs	r0, r2
	str	r0, [sp, #532]
	str	r0, [r0, r0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8bc9
	lsls	r0, r0, #8
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
.L_020048f6:
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
