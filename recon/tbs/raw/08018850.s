.syntax unified
	.thumb
	.global UiText_MeasureEntryDimensions
	.thumb_func
UiText_MeasureEntryDimensions:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r1, [sp, #12]
	str r2, [sp, #8]
	adds r6, r3, #0
	ldr r3, .L_080189f0
	ldr r4, [r3]
	ldr r3, .L_080189f4
	movs r2, #0
	mov r10, r2
	mov lr, r3
	movs r2, #24
	movs r3, #16
	movs r1, #15
	movs r5, #0
	add r2, sp
	add r3, sp
	mov r11, r1
	movs r7, #0
	movs r1, #0
	mov r8, r2
	mov r12, r5
	mov r9, r3
.L_0801888a:
	movs r2, #235
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r4, r3]
	ldr r3, .L_080189f8
	adds r0, #1
	ands r0, r3
	cmp r2, #31
	bls .L_080188c6
	cmp r2, #32
	bne .L_080188a8
	adds r1, #5
	adds r5, #1
	b .L_0801888a
.L_080188a8:
	ldr r3, .L_080189fc
	subs r2, #32
	lsls r2, r2, #5
	ldrh r2, [r3, r2]
	ldr r3, .L_08018a00
	adds r3, r4, r3
	ldrh r3, [r3]
	str r3, [sp, #0]
	cmp r3, #1
	beq .L_080188c0
	cmp r3, #5
	bne .L_080188c2
.L_080188c0:
	adds r2, #1
.L_080188c2:
	adds r1, r1, r2
	b .L_0801888a
.L_080188c6:
	cmp r2, #28
	bhi .L_0801888a
	lsls r3, r2, #2
	mov r2, lr
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080188d4:
	.2byte 0x89a2
	.2byte 0x0801
	.2byte 0x89a2
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x8948
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x897e
	.2byte 0x0801
	.2byte 0x8986
	.2byte 0x0801
	.2byte 0x897e
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x8978
	.2byte 0x0801
	.2byte 0x897e
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x897e
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x888a
	.2byte 0x0801
	.2byte 0x8978
	.2byte 0x0801
	.2byte 0x4643
	.2byte 0x4662
	.2byte 0x3501
	.2byte 0x529d
	.2byte 0x464b
	.2byte 0x5299
	.2byte 0x428f
	.2byte 0xd200
	.2byte 0x1c0f
	.2byte 0x4653
	.2byte 0x2b02
	.2byte 0xd804
	.2byte 0x2101
	.2byte 0x448a
	.2byte 0x4652
	.2byte 0x0052
	.2byte 0x4694
	.2byte 0x4a22
	.2byte 0x230f
	.2byte 0x2500
	.2byte 0x2100
	.2byte 0x449b
	.2byte 0x4696
	.2byte 0xe788
	.2byte 0x4b1f
	.2byte 0x3001
	.2byte 0x4018
	.2byte 0x4b1e
	.2byte 0x3001
	.2byte 0x4018
	.2byte 0xe781
	.2byte 0x22eb
	.2byte 0x0043
	.2byte 0x0112
	.2byte 0x189b
	.2byte 0x5ae2
	.2byte 0x4b1b
	.2byte 0x191b
	.2byte 0x801a
	.2byte 0x4b18
	.2byte 0x4a16
	.2byte 0x3001
	.2byte 0x4018
	.2byte 0x4696
	.2byte 0xe773
	.2byte 0x4643
	.2byte 0x4662
	.2byte 0x3501
	.2byte 0x529d
	.2byte 0x464b
	.2byte 0x5299
	.2byte 0x428f
	.2byte 0xd200
	.2byte 0x1c0f
	.2byte 0x4913
	.2byte 0x1863
	.2byte 0x781b
	.2byte 0x2b00
	.2byte 0xd000
	.2byte 0x3702
	.2byte 0x9a03
	.2byte 0x6017
	.2byte 0x9902
	.2byte 0x465b
	.2byte 0x600b
	.2byte 0x1c3b
	.2byte 0x3313
	.2byte 0x08df
	.2byte 0x00fb
	.2byte 0x1c1f
	.2byte 0x3f10
	.2byte 0x2e00
	.2byte 0xd031
	.2byte 0x2200
	.2byte 0x2500
	.global Func_080189de
	.thumb_func
Func_080189de:
	.2byte 0x4641
	.2byte 0x5a6b
	.2byte 0x2b01
	.2byte 0xd810
	.2byte 0x4b01
	.2byte 0x8033
	.2byte 0xe023
	.2byte 0x0000
	.2byte 0x0000
.L_080189f0:
	.4byte Data_03001e8c
.L_080189f4:
	.4byte .L_080188d4
.L_080189f8:
	.4byte 0x000001ff
.L_080189fc:
	.4byte UiText_Glyphs
.L_08018a00:
	.4byte 0x00000eac
