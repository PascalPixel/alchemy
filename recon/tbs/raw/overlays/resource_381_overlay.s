.syntax unified
	.thumb
	.section .text.x02009410,"ax",%progbits
	.balign 4
	.global Func_02001410
	.thumb_func
Func_02001410:
	.global Scene_RunExtendedEffectPresentation
	.thumb_func
Scene_RunExtendedEffectPresentation:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #892]
	ldr r3, [r3]
	movs r0, #15
	sub sp, #8
	mov r9, r3
	bl 0x0200b4ec
	mov r10, r0
	ldr r0, [pc, #880]
	bl 0x0200b3f4
	ldr r2, [pc, #876]
	movs r3, #3
	str r3, [r2]
	movs r0, #80
	bl 0x0200b4cc
	movs r0, #17
	bl 0x0200b624
	movs r1, #0
	ldr r0, [pc, #860]
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	movs r0, #40
	bl 0x0200b4cc
	ldr r0, [pc, #848]
	bl 0x0200b3f4
	movs r5, #0
.L_02001410_0:
	adds r0, r5, #0
	adds r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200b534
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #15
	bls .L_02001410_0
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl 0x0200b534
	movs r0, #0
	bl 0x0200a820
	movs r0, #1
	bl 0x0200a820
	ldr r3, [pc, #792]
	movs r5, #0
	add r3, r9
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	str r5, [r3]
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #80
	bl 0x0200b4cc
	bl 0x0200ac9c
	bl 0x0200b5d4
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #231
	lsls r3, r3, #16
	str r3, [r0, #8]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r0, #16]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #56]
	str r3, [r0, #60]
	str r3, [r0, #64]
	str r5, [r0, #12]
	str r5, [r0, #36]
	str r5, [r0, #44]
	movs r0, #4
	bl 0x0200b3e4
	bl 0x0200b464
	movs r0, #4
	bl 0x0200b3e4
	movs r0, #0
	movs r1, #3
	bl 0x0200b5a4
	movs r0, #1
	movs r1, #3
	bl 0x0200b5a4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #0
	bl 0x0200b4ec
	mov r8, r0
	movs r0, #1
	bl 0x0200b4ec
	adds r6, r0, #0
	mov r0, r8
	movs r1, #192
	ldr r2, [r0, #80]
	ldr r7, [r6, #80]
	lsls r1, r1, #7
.L_02001410_1:
	ldrh r3, [r2, #30]
	movs r0, #128
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r3, [r2, #30]
	ldrh r3, [r7, #30]
	ldr r0, [pc, #632]
	adds r3, r3, r0
	strh r3, [r7, #30]
	mov r0, r8
	ldr r3, [r0, #8]
	adds r3, r3, r1
	str r3, [r0, #8]
	ldr r3, [r6, #8]
	subs r3, r3, r1
	str r3, [r6, #8]
	movs r0, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl 0x0200b3e4
	adds r5, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0]
	cmp r5, #19
	bls .L_02001410_1
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
.L_02001570:
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200b4fc
	movs r0, #0
	bl 0x0200b4ec
	ldr r3, [r0, #80]
	movs r5, #0
	strh r5, [r3, #30]
	movs r0, #1
	bl 0x0200b4ec
	ldr r3, [r0, #80]
	movs r1, #6
	strh r5, [r3, #30]
	movs r0, #0
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #0
	movs r1, #246
	movs r2, #150
	bl 0x0200b50c
	movs r2, #150
	movs r0, #1
	movs r1, #220
	bl 0x0200b514
	movs r0, #0
	movs r1, #2
	bl 0x0200b5a4
	movs r1, #2
	movs r0, #1
	bl 0x0200b5a4
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	bl 0x0200b504
	movs r0, #1
	bl 0x0200b504
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #80
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	ldr r5, [pc, #328]
	adds r0, r5, #0
	bl 0x0200b57c
	movs r1, #0
	movs r0, #1
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001570_0
	movs r0, #1
	movs r1, #3
	bl 0x0200b544
	adds r0, r5, #1
	bl 0x0200b57c
	b .L_02001570_1
.L_02001570_0:
	movs r0, #1
	movs r1, #1
	bl 0x0200b55c
	adds r0, r5, #2
	bl 0x0200b57c
.L_02001570_1:
	movs r0, #1
	movs r1, #0
	movs r2, #60
	bl 0x0200b594
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200b54c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	ldr r0, [pc, #216]
	bl 0x0200b57c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x0200b54c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200b5ac
	movs r0, #80
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #2
	bl 0x0200b55c
	movs r1, #0
	movs r0, #1
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001570_2
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #4
	bl 0x0200b53c
	ldr r0, [pc, #44]
	bl 0x0200b57c
	b .L_02001570_3
	.2byte 0x0000
	.2byte 0x1ec4
	.2byte 0x0300
	.2byte 0x935d
	.2byte 0x0200
	.2byte 0xbb68
	.2byte 0x0200
	.2byte 0x7fff
	.2byte 0x0000
	.2byte 0x90c5
	.2byte 0x0200
	.2byte 0x040c
	.2byte 0x0000
	.2byte 0xff00
	.2byte 0xffff
	.4byte 0x000010f8
	.4byte 0x000010fb
	.4byte 0x000010fd
.L_02001570_2:
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #4
	bl 0x0200b53c
	ldr r0, [pc, #816]
	bl 0x0200b57c
.L_02001570_3:
	movs r2, #40
	movs r0, #1
	movs r1, #0
	bl 0x0200b594
	movs r0, #0
	movs r1, #3
	bl 0x0200b53c
	movs r1, #3
	movs r0, #1
	bl 0x0200b544
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #224
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b5bc
	movs r0, #142
	movs r1, #1
	movs r2, #184
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #15
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #160
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #254
	movs r1, #1
	movs r2, #162
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #15
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200b5bc
	movs r0, #152
	movs r1, #1
	movs r2, #147
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #5
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #200
	movs r1, #1
	movs r2, #215
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200b5bc
	movs r1, #1
	movs r2, #145
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	ldr r0, [pc, #508]
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	ldr r0, [pc, #488]
	bl 0x0200b57c
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200b594
	movs r1, #3
	movs r0, #0
	bl 0x0200b544
	movs r0, #10
	bl 0x0200b4cc
	ldr r5, [pc, #360]
	movs r0, #23
	bl 0x0200b624
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200ad48
	add r5, r9
	movs r3, #0
	movs r0, #160
	movs r1, #160
	movs r2, #128
	str r3, [r5]
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200b494
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #40
	movs r2, #0
	bl 0x0200ad48
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #0
	movs r1, #243
	movs r2, #144
	bl 0x0200b50c
	movs r1, #202
	movs r2, #144
	movs r0, #1
	bl 0x0200b50c
	movs r0, #0
	bl 0x0200b52c
	movs r0, #20
	bl 0x0200b4cc
	movs r3, #1
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r3, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #80
	bl 0x0200b4cc
	movs r0, #0
	ldr r1, [pc, #172]
	ldr r2, [pc, #172]
	bl 0x0200b4fc
	movs r0, #1
	ldr r1, [pc, #160]
	ldr r2, [pc, #164]
	bl 0x0200b4fc
	movs r0, #1
	movs r1, #220
	movs r2, #150
	bl 0x0200b51c
	movs r2, #150
	movs r0, #0
	movs r1, #246
	bl 0x0200b524
	movs r0, #1
	movs r1, #1
	bl 0x0200b53c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #1
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001570_4
	movs r1, #128
	lsls r1, r1, #5
	movs r0, #1
	movs r2, #0
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #3
	bl 0x0200b544
	b .L_02001570_5
	.2byte 0x0000
	.4byte 0x000010fe
	.4byte 0x01110000
	.4byte 0x000010ff
	.4byte 0x0000040c
	.4byte 0x00006666
	.4byte 0x00003333
.L_02001570_4:
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #4
	bl 0x0200b53c
	ldr r0, [pc, #996]
	bl 0x0200b57c
.L_02001570_5:
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #0
	bl 0x0200b58c
	movs r1, #220
	movs r2, #152
	lsls r1, r1, #15
	lsls r2, r2, #16
	movs r0, #15
	bl 0x0200b534
	movs r0, #1
	bl 0x0200b3e4
	movs r0, #15
	ldr r1, [pc, #956]
	ldr r2, [pc, #960]
	bl 0x0200b4fc
	movs r0, #15
	movs r1, #171
	movs r2, #152
	bl 0x0200b50c
	movs r2, #0
	movs r1, #0
	movs r0, #15
.L_02001ba8:
	bl 0x0200b59c
	movs r0, #15
	bl 0x0200b4ec
	movs r1, #1
	bl 0x0200b484
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r0, #1
	movs r1, #217
	movs r2, #182
	bl 0x0200b51c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b5bc
	movs r0, #217
	movs r1, #1
	movs r2, #176
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #129
.L_02001c0c:
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200b5b4
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x0200b54c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #4
	movs r2, #0
	movs r0, #0
	bl 0x0200b54c
	movs r0, #30
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b5ac
.L_02001c46:
	movs r1, #1
	movs r0, #1
	bl 0x0200b53c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #4
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #231
	movs r2, #175
	bl 0x0200b524
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	ldr r0, [pc, #728]
	bl 0x0200b57c
	movs r1, #0
	movs r0, #1
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	ldr r1, [pc, #712]
	movs r2, #0
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #1
	bl 0x0200b554
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5b4
	ldr r6, [pc, #664]
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #20
	movs r2, #0
	bl 0x0200ad48
	add r6, r9
	movs r3, #0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r3, [r6]
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	mov r8, r3
	bl 0x0200b494
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #2
	bl 0x0200b554
	movs r1, #2
	movs r0, #1
	bl 0x0200b554
	ldr r0, [pc, #604]
	bl 0x0200b624
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	movs r3, #1
	movs r0, #1
	movs r1, #1
	str r3, [r6]
	ldr r2, [pc, #580]
	negs r1, r1
	negs r0, r0
	bl 0x0200b494
	movs r0, #120
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #100
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ad48
	mov r0, r8
	str r0, [r6]
	movs r1, #128
	movs r0, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200b494
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200b4fc
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #4
	movs r0, #0
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200b54c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #150
	bl 0x0200b50c
	movs r1, #231
	movs r2, #180
	movs r0, #1
	bl 0x0200b50c
	movs r0, #1
	bl 0x0200b52c
	movs r2, #0
	movs r1, #40
	movs r0, #0
	bl 0x0200ad48
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x0200b554
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #40
	bl 0x0200b4cc
	movs r2, #0
	ldr r1, [pc, #296]
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #1
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	ldr r1, [pc, #252]
	movs r2, #0
	movs r0, #15
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #184]
	bl 0x0200b3ec
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #15
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #186
	movs r1, #1
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b5c4
	movs r1, #130
	movs r2, #113
	movs r0, #15
	bl 0x0200b50c
	movs r0, #15
	bl 0x0200b52c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	mov r3, r8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r3, [r6]
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200b494
	movs r1, #1
	ldr r0, [pc, #72]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	bl 0x0200aff0
	movs r0, #15
	movs r1, #2
	bl 0x0200b544
	movs r5, #0
	b .L_02001c46_0
	.2byte 0x0000
	.2byte 0x1103
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x9999
	.2byte 0x0000
	.4byte 0x00001104
	.4byte 0x00000101
	.4byte 0x0000040c
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000103
	.4byte 0x0200a93d
	.4byte 0x0020119e
.L_02001c46_0:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_02001c46_0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #448]
	bl 0x0200b3ec
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #30
	bl 0x0200b4cc
	ldr r0, [pc, #424]
	bl 0x0200b624
	ldr r0, [pc, #420]
	bl 0x0200b3f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200b494
	movs r5, #15
.L_02001c46_1:
	movs r0, #0
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	subs r5, #1
	negs r0, r0
	cmp r5, r0
	bne .L_02001c46_1
	movs r0, #0
	bl 0x0200a84c
	ldr r2, [pc, #360]
	movs r3, #1
	add r2, r9
	str r3, [r2]
	adds r0, r5, #0
	ldr r2, [pc, #356]
	adds r1, r5, #0
	bl 0x0200b494
	bl 0x0200b49c
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	ldr r0, [pc, #320]
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #218
	movs r2, #181
	movs r3, #1
	lsls r0, r0, #16
	adds r1, r5, #0
	lsls r2, r2, #16
	bl 0x0200b5c4
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r1, #169
	movs r2, #151
	movs r0, #15
	bl 0x0200b50c
	movs r0, #15
	bl 0x0200b52c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #15
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r0, #224
	movs r2, #158
	lsls r2, r2, #16
	movs r3, #1
	adds r1, r5, #0
	lsls r0, r0, #16
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #0
	bl 0x0200a9a4
	movs r1, #2
	movs r0, #15
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001c46_2
	movs r0, #40
	bl 0x0200b4cc
	b .L_02001c46_3
	.2byte 0x0000
	.4byte 0x0200b00d
	.4byte 0x00000121
	.4byte 0x0200a93d
	.4byte 0x0000040c
	.4byte 0x0000e666
.L_02001c46_2:
	ldr r0, [pc, #996]
	movs r1, #1
	bl 0x0200b4ac
	movs r0, #40
	bl 0x0200b4cc
.L_02001c46_3:
	movs r0, #0
	bl 0x0200b4ec
	movs r3, #144
	ldr r2, [r0, #12]
	lsls r3, r3, #14
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #22
	bl 0x0200b454
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02001c46_4
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200b414
	ldr r6, [r7, #80]
	adds r2, r6, #0
	movs r3, #0
	adds r2, #38
	strb r3, [r2]
	adds r2, #1
	strb r3, [r2]
	ldrb r2, [r6, #5]
	subs r3, #33
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r5, r0, #0
	strb r3, [r6, #9]
	movs r0, #222
	bl 0x0200b4b4
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x0200b434
	movs r0, #17
	bl 0x0200b424
	movs r0, #0
	movs r1, #28
	bl 0x0200b53c
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200b60c
	movs r0, #0
	movs r1, #28
	bl 0x0200b53c
.L_02001c46_4:
	movs r0, #1
	movs r1, #20
	movs r2, #0
	bl 0x0200ad48
	ldr r0, [pc, #844]
	movs r6, #128
	movs r5, #0
	mov r8, r0
	lsls r6, r6, #9
	mov r0, r10
	bl 0x0200ad94
	movs r0, #1
	bl 0x0200b3e4
	mov r0, r10
	bl 0x0200ad94
	movs r0, #1
	bl 0x0200b3e4
	mov r3, r8
	str r3, [r7, #24]
	str r3, [r7, #28]
.L_0200222a:
	mov r0, r10
	bl 0x0200ad94
	movs r0, #1
	bl 0x0200b3e4
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	str r6, [r7, #24]
	str r6, [r7, #28]
	cmp r5, #23
	bls 0x0200a20c
	movs r0, #15
	movs r1, #0
	bl 0x0200b56c
	movs r1, #20
	movs r2, #0
	movs r0, #0
	bl 0x0200ad48
	ldr r0, [pc, #756]
	bl 0x0200b57c
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	cmp r7, #0
	beq .L_0200222a_0
	adds r0, r7, #0
	bl 0x0200b45c
.L_0200222a_0:
	movs r1, #1
	movs r0, #0
	bl 0x0200b53c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #160
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r6, #232
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	lsls r6, r6, #1
	bl 0x0200b5bc
	adds r1, r6, #0
	movs r0, #232
	bl 0x0200ac1c
	ldr r5, [pc, #644]
	movs r0, #1
	bl 0x0200a9a4
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	adds r0, r5, #0
	movs r1, #144
	bl 0x0200ac1c
	movs r0, #2
	bl 0x0200a9a4
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200ac1c
	movs r0, #3
	bl 0x0200a9a4
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #5
	movs r2, #0
	bl 0x0200b59c
	ldr r2, [pc, #576]
	ldr r1, [pc, #580]
	movs r0, #1
	bl 0x0200b534
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #0
	bl 0x0200b58c
	movs r1, #231
	movs r2, #180
	movs r0, #1
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b534
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b3e4
	movs r0, #219
	movs r1, #171
	bl 0x0200ac1c
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #2
	bl 0x0200b554
	movs r0, #1
	movs r1, #2
	bl 0x0200b55c
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	ldr r2, [pc, #464]
	movs r3, #0
	add r2, r9
	str r3, [r2]
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200b494
	movs r1, #1
	ldr r0, [pc, #440]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #416]
	bl 0x0200b3ec
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #184
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b5c4
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b54c
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #245
	movs r0, #0
	movs r2, #145
	bl 0x0200b50c
	movs r1, #215
	movs r2, #168
	movs r0, #1
	bl 0x0200b50c
	movs r0, #1
	bl 0x0200b52c
	movs r0, #80
	bl 0x0200b4cc
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #184
	movs r2, #87
	movs r0, #15
	bl 0x0200b50c
	movs r0, #15
	bl 0x0200b52c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #2
	bl 0x0200b544
	movs r5, #0
.L_0200222a_1:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_0200222a_1
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #200]
	bl 0x0200b3ec
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #30
	bl 0x0200b4cc
	ldr r0, [pc, #176]
	bl 0x0200b624
	ldr r0, [pc, #160]
	bl 0x0200b3f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b494
	movs r5, #7
.L_0200222a_2:
	movs r0, #0
	bl 0x0200a8dc
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	bl 0x0200a8dc
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	subs r5, #1
	negs r0, r0
	cmp r5, r0
	bne .L_0200222a_2
	movs r0, #0
	bl 0x0200a8dc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl 0x0200b494
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	ldr r0, [pc, #76]
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200b494
	b .L_0200222a_3
	.2byte 0x110c
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x0000110d
	.4byte 0x000002c7
	.4byte 0x01590000
	.4byte 0x02460000
	.4byte 0x0000040c
	.4byte 0x0020119e
	.4byte 0x0200a971
	.4byte 0x0200b00d
	.4byte 0x00000121
.L_0200222a_3:
	movs r1, #1
	ldr r0, [pc, #636]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #612]
	bl 0x0200b3ec
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #127
	movs r2, #110
	bl 0x0200b514
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #2
	bl 0x0200b544
	movs r5, #0
.L_0200222a_4:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_0200222a_4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #544]
	bl 0x0200b3ec
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	ldr r0, [pc, #524]
	bl 0x0200b624
	movs r0, #30
	bl 0x0200b4cc
	ldr r0, [pc, #504]
	bl 0x0200b3f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b494
	movs r5, #7
.L_0200222a_5:
	movs r0, #0
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r3, #1
	subs r5, #1
	negs r3, r3
	cmp r5, r3
	bne .L_0200222a_5
	movs r0, #0
	bl 0x0200a84c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #107
	bl 0x0200b624
	movs r0, #63
	bl 0x0200b624
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200b494
	movs r1, #1
	ldr r0, [pc, #392]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #380]
	bl 0x0200b3ec
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	ldr r5, [pc, #356]
	adds r0, r5, #0
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #184
	movs r2, #87
	bl 0x0200b514
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #15
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	adds r0, r5, #0
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #141
	bl 0x0200b624
	movs r0, #100
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	movs r0, #1
	movs r1, #3
	bl 0x0200b55c
	movs r1, #0
	movs r0, #1
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	ldr r0, [pc, #204]
	bl 0x0200b624
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r5, #0
.L_0200222a_6:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_0200222a_6
	ldr r5, [pc, #144]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200b3ec
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #2
	ldr r0, [pc, #136]
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #100
	bl 0x0200b3e4
	movs r1, #1
	ldr r0, [pc, #116]
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #60
	bl 0x0200b3e4
	adds r0, r5, #0
	bl 0x0200b3f4
	ldr r2, [pc, #96]
	movs r3, #1
	add r2, r9
	movs r0, #1
	movs r1, #1
	str r3, [r2]
	negs r1, r1
	ldr r2, [pc, #84]
	negs r0, r0
	bl 0x0200b494
	bl 0x0200b49c
	bl 0x0200b000
	ldr r0, [pc, #72]
	bl 0x0200b4c4
	ldr r0, [pc, #72]
	bl 0x0200b4c4
	movs r0, #5
	bl 0x0200b5dc
	movs r0, #128
	lsls r0, r0, #1
	bl 0x0200b4c4
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0020119e
	.4byte 0x0200a93d
	.4byte 0x0200b00d
	.4byte 0x00000121
	.4byte 0x0200a971
	.4byte 0x00007fff
	.4byte 0x0000040c
	.4byte 0x0000e666
	.4byte 0x00000814
	.4byte 0x0000083f
	.section .text.x0200b01c,"ax",%progbits
	.global Soru_UpdateRing
	.thumb_func
Soru_UpdateRing:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #202
	lsls	r1, r1, #1
	movs	r0, #33
	sub	sp, #68
	bl 0x0200b41c
	str	r0, [sp, #64]
	str	r0, [sp, #60]
	ldr	r1, [sp, #64]
	movs	r0, #0
	movs	r2, #200
	str	r0, [sp, #56]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0200304e
	b.n	.L_020032e2
.L_0200304e:
	adds	r1, #8
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #64]
	str	r0, [sp, #8]
	ldr	r0, [pc, #668]
	mov	sl, r1
	ldr	r1, [pc, #668]
	adds	r3, #36
	adds	r4, #37
	adds	r0, #1
	str	r3, [sp, #16]
	str	r4, [sp, #12]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
.L_0200306a:
	mov	r3, sl
	ldr	r3, [r3, #8]
	ldr	r2, [sp, #60]
	ldr	r5, [r2, #0]
	str	r3, [sp, #52]
	mov	r4, sl
	ldr	r4, [r4, #12]
	str	r4, [sp, #48]
	mov	r0, sl
	ldr	r0, [r0, #16]
	str	r0, [sp, #44]
	mov	r1, sl
	ldr	r1, [r1, #20]
	str	r1, [sp, #40]
	mov	r2, sl
	ldr	r2, [r2, #24]
	ldr	r4, [sp, #60]
	str	r2, [sp, #36]
	ldr	r3, [sp, #12]
	ldr	r4, [r4, #4]
	ldrb	r3, [r3, #0]
	ldr	r0, [sp, #60]
	str	r4, [sp, #28]
	ldr	r0, [r0, #8]
	ldr	r2, [sp, #60]
	str	r0, [sp, #24]
	ldr	r2, [r2, #12]
	mov	fp, r3
	str	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #0]
	str	r3, [sp, #32]
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r1, fp
	str	r3, [sp, #32]
	cmp	r3, #0
	beq.n	.L_020030ba
	b.n	.L_0200326e
.L_020030ba:
	movs	r4, #3
	str	r4, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0200310e
	ldr	r0, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #56]
	adds	r0, r0, r2
	str	r0, [sp, #40]
	ldr	r3, [pc, #556]
	lsls	r2, r4, #2
	ldr	r3, [r3, r2]
	cmp	r0, r3
	blt.n	.L_020030e0
	ldr	r3, [pc, #552]
	ldr	r3, [r3, r2]
	negs	r3, r3
	str	r3, [sp, #36]
	b.n	.L_02003108
.L_020030e0:
	ldr	r0, [sp, #40]
	ldr	r3, [pc, #544]
	cmp	r0, r3
	bgt.n	.L_02003108
	ldr	r3, [pc, #532]
	ldr	r4, [pc, #536]
	ldr	r3, [r3, r2]
	str	r4, [sp, #40]
	str	r3, [sp, #36]
	ldr	r2, [r5, #8]
	str	r2, [sp, #28]
	ldr	r3, [r5, #12]
	str	r3, [sp, #24]
	ldr	r4, [r5, #16]
	movs	r0, #24
	str	r4, [sp, #20]
	str	r1, [r5, #8]
	str	r1, [r5, #12]
	str	r1, [r5, #16]
	mov	fp, r0
.L_02003108:
	ldr	r0, [sp, #40]
	str	r0, [r5, #24]
	str	r0, [r5, #28]
.L_0200310e:
	bl 0x0200b3fc
	ldr	r2, [pc, #480]
	ldr	r1, [sp, #8]
	ldrb	r3, [r1, r2]
	muls	r3, r0
	lsrs	r6, r3, #16
	bl 0x0200b3fc
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #0]
	muls	r3, r0
	lsrs	r7, r3, #16
	bl 0x0200b3fc
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #1]
	muls	r3, r0
	lsrs	r3, r3, #16
	mov	r8, r3
	cmp	r6, #0
	beq.n	.L_02003148
	movs	r1, #250
	lsls	r0, r6, #16
	lsls	r1, r1, #2
	bl 0x0200b3d4
	adds	r6, r0, #0
	b.n	.L_0200314a
.L_02003148:
	movs	r6, #0
.L_0200314a:
	cmp	r7, #0
	beq.n	.L_0200315c
	movs	r1, #250
	lsls	r0, r7, #16
	lsls	r1, r1, #2
	bl 0x0200b3d4
	mov	r9, r0
	b.n	.L_02003160
.L_0200315c:
	movs	r2, #0
	mov	r9, r2
.L_02003160:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02003172
	movs	r1, #250
	lsls	r0, r3, #16
	lsls	r1, r1, #2
	bl 0x0200b3d4
	b.n	.L_02003174
.L_02003172:
	movs	r0, #0
.L_02003174:
	ldr	r2, [pc, #400]
	ldr	r4, [sp, #8]
	ldrsb	r3, [r2, r4]
	cmp	r3, #1
	bne.n	.L_02003186
	ldr	r1, [sp, #52]
	adds	r1, r1, r6
	str	r1, [sp, #52]
	b.n	.L_02003198
.L_02003186:
	ldr	r4, [sp, #52]
.L_02003188:
	movs	r1, #1
	subs	r4, r4, r6
	negs	r1, r1
	str	r4, [sp, #52]
	cmp	r3, r1
	beq.n	.L_02003198
	movs	r3, #0
	str	r3, [sp, #52]
.L_02003198:
	ldr	r3, [sp, #8]
	adds	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_020031aa
	ldr	r4, [sp, #48]
	add	r4, r9
	str	r4, [sp, #48]
	b.n	.L_020031be
.L_020031aa:
	ldr	r1, [sp, #48]
	mov	r4, r9
	subs	r1, r1, r4
	str	r1, [sp, #48]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_020031be
	movs	r3, #0
	str	r3, [sp, #48]
.L_020031be:
	ldr	r3, [sp, #8]
	adds	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_020031d0
	ldr	r4, [sp, #44]
	adds	r4, r4, r0
	str	r4, [sp, #44]
	b.n	.L_020031e2
.L_020031d0:
	ldr	r1, [sp, #44]
.L_020031d2:
	movs	r2, #1
	subs	r1, r1, r0
	negs	r2, r2
	str	r1, [sp, #44]
	cmp	r3, r2
	beq.n	.L_020031e2
	movs	r3, #0
	str	r3, [sp, #44]
.L_020031e2:
	ldr	r4, [sp, #0]
	ldr	r1, [sp, #52]
	ldrb	r3, [r4, #0]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200b404
	ldr	r2, [sp, #0]
.L_020031f2:
	ldr	r4, [sp, #48]
	ldrb	r3, [r2, #1]
	lsls	r6, r0, #1
	adds	r0, r3, #0
	muls	r0, r4
.L_020031fc:
	bl 0x0200b404
	lsls	r7, r0, #1
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #44]
	ldrb	r3, [r0, #2]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200b40c
	mov	r2, fp
	lsls	r0, r0, #1
	cmp	r2, #0
	beq.n	.L_02003250
	ldr	r3, [sp, #28]
	adds	r3, r3, r6
.L_0200321c:
	str	r3, [sp, #28]
	mov	r3, fp
	ldr	r4, [sp, #24]
	ldr	r1, [sp, #20]
	adds	r3, #255
.L_02003226:
	lsls	r3, r3, #24
	adds	r4, r4, r7
	adds	r1, r1, r0
	lsrs	r3, r3, #24
	str	r4, [sp, #24]
	str	r1, [sp, #20]
	mov	fp, r3
	cmp	r3, #0
	bne.n	.L_0200326e
	ldr	r2, [sp, #28]
	mov	r3, r9
	str	r2, [r5, #8]
	str	r2, [r5, #56]
	cmp	r3, #0
	beq.n	.L_02003248
	str	r4, [r5, #12]
	str	r4, [r5, #60]
.L_02003248:
	ldr	r4, [sp, #20]
	str	r4, [r5, #16]
	str	r4, [r5, #64]
	b.n	.L_0200326e
.L_02003250:
	ldr	r3, [r5, #8]
	mov	r1, r9
	adds	r3, r3, r6
	str	r3, [r5, #8]
	str	r3, [r5, #56]
	cmp	r1, #0
	beq.n	.L_02003266
	ldr	r3, [r5, #12]
	adds	r3, r3, r7
	str	r3, [r5, #12]
	str	r3, [r5, #60]
.L_02003266:
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	str	r3, [r5, #64]
.L_0200326e:
	ldr	r2, [sp, #52]
	mov	r3, sl
	str	r2, [r3, #8]
	ldr	r4, [sp, #48]
	str	r4, [r3, #12]
	ldr	r0, [sp, #44]
	str	r0, [r3, #16]
	ldr	r1, [sp, #40]
	str	r1, [r3, #20]
	ldr	r2, [sp, #36]
	str	r2, [r3, #24]
	ldr	r4, [sp, #12]
	mov	r3, fp
	strb	r3, [r4, #0]
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #60]
	str	r0, [r1, #4]
	ldr	r2, [sp, #24]
	mov	r3, sl
	str	r2, [r3, #0]
	ldr	r4, [sp, #20]
	add	r0, sp, #32
	str	r4, [r1, #12]
	ldrb	r0, [r0, #0]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #8]
	adds	r1, #3
	adds	r2, #3
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #56]
	str	r1, [sp, #0]
	str	r2, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	adds	r3, #3
	adds	r4, #1
	adds	r1, #40
	adds	r2, #40
	str	r3, [sp, #4]
	str	r4, [sp, #56]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r3, [sp, #60]
	movs	r0, #40
	adds	r3, #40
	add	sl, r0
	ldr	r4, [sp, #64]
	movs	r0, #200
	str	r3, [sp, #60]
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [sp, #56]
	cmp	r1, r3
	beq.n	.L_020032e2
	b.n	.L_0200306a
.L_020032e2:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0200ba0c
	.4byte 0x0200ba2a
	.4byte 0x0200ba68
	.4byte 0x0200ba90
	.4byte 0x00001999
	.2byte 0xba48
	.2byte 0x0200
	.section .rodata,"a",%progbits
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00060000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000a3d
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000a3d
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000012
	.4byte 0xc0010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
	.global Funka_ArcOrigins
Funka_ArcOrigins:
	.4byte 0xfffa0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x000a0000
	.4byte 0x00080000
	.4byte 0x00190000
	.4byte 0x00040000
	.4byte 0x001e0000
	.4byte 0xfffb0000
	.4byte 0x00140000
	.4byte 0x00020000
	.4byte 0x00050000
	.4byte 0xfffa0000
	.4byte 0x00230000
	.4byte 0xfff80000
	.4byte 0x000f0000
	.4byte 0x00020000
	.4byte 0x00280000
	.4byte 0xfffe0000
	.4byte 0x000f0000
	.global Data_0200b6d4
Data_0200b6d4:
	.4byte 0xffff0000
	.4byte 0x000001d8
	.4byte 0x40000142
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200b704
Data_0200b704:
	.4byte 0x00000012
	.4byte 0x0050800b
	.4byte 0x000001ff
	.global Data_0200b710
Data_0200b710:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200b998
Data_0200b998:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0028003b
	.4byte 0x00040003
	.4byte 0x003e0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280041
	.4byte 0x00040003
	.4byte 0x00440006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280047
	.4byte 0x00040003
	.4byte 0x004a0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x0028004d
	.4byte 0x00040003
	.4byte 0x00500006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280053
	.4byte 0x00040003
	.4byte 0xffff0000
	.global Funka_EmberScript
Funka_EmberScript:
	.4byte 0x00000022
	.4byte 0x02008f55
	.4byte 0x00000010
	.4byte 0x04040404
	.4byte 0x00040300
	.4byte 0x04000404
	.4byte 0x04040003
	.4byte 0x00060406
	.4byte 0x03000404
	.4byte 0x02010002
	.4byte 0x01010200
	.4byte 0x02000102
	.4byte 0x01020001
	.4byte 0x00010100
	.4byte 0x02010102
	.4byte 0x01020001
	.4byte 0x00010200
	.4byte 0x02000102
	.4byte 0x01010101
	.4byte 0x00010100
	.4byte 0x0100ff01
	.4byte 0x01ff00ff
	.4byte 0x00ff0101
	.4byte 0xff00ffff
	.4byte 0xffff00ff
	.4byte 0x0000ff00
	.global Soru_RingOffsetX
Soru_RingOffsetX:
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00009999
	.4byte 0x0000b333
	.global Soru_RingOffsetZ
Soru_RingOffsetZ:
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x0000028f
	.4byte 0x0000020c
	.4byte 0x0000028f
@ The scene's own variables, which lie past the image.
	.section .bss,"aw",%nobits
	.space 8
	.global gEmberState
gEmberState:
	.space 64
	.global gEmberTimer
gEmberTimer:
	.space 16
	.global gArcEffects
gArcEffects:
	.space 48
	.global gArcEffectTimers
gArcEffectTimers:
	.space 40
	.global gEmberMask
gEmberMask:
	.space 4
	.global gEmberLevel
gEmberLevel:
	.space 4
	.global gEmberLevelTimer
gEmberLevelTimer:
	.space 4
