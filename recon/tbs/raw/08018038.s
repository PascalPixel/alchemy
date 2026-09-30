.syntax unified
	.thumb
	.global UiText_BuildRenderEntries
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r0, [sp, #48]
	ldr r0, .L_08018170
	mov r11, r1
	ldr r1, [r0]
	movs r2, #1
	movs r3, #0
	ldr r5, .L_08018174
	str r1, [sp, #44]
	str r2, [sp, #40]
	str r3, [sp, #36]
	adds r3, r1, r5
	ldrh r3, [r3]
	str r2, [sp, #20]
	movs r2, #235
	str r3, [sp, #32]
	adds r6, r3, #0
	lsls r2, r2, #4
	movs r5, #1
	ldr r3, [sp, #48]
	mov r10, r0
	movs r7, #0
	movs r0, #0
	adds r1, r1, r2
	negs r5, r5
	str r0, [sp, #28]
	str r0, [sp, #24]
	str r7, [sp, #52]
	mov r8, r1
	str r0, [sp, #16]
	cmp r3, r5
	bne .L_08018092
	ldr r0, [sp, #44]
	ldr r1, .L_08018178
	adds r3, r0, r1
	ldrh r3, [r3]
	str r3, [sp, #32]
	b .L_0801865a
.L_08018092:
	ldr r5, .L_0801817c
	movs r0, #50
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	lsrs r5, r5, #2
	lsls r2, r2, #24
	adds r1, r0, #0
	ldr r3, .L_08018180
	ldr r0, .L_08018184
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r10
	add r2, sp, #56
	adds r3, #140
	ldr r3, [r3]
	mov r10, r2
	mov r0, r10
	ldr r1, [sp, #48]
	mov r9, r3
	bl UiText_LookupMessage
	mov r3, sp
	adds r3, #84
	str r3, [sp, #12]
.L_080180c8:
	mov r0, r10
	bl _call_via_r9
	adds r5, r7, #0
	adds r7, r0, #0
	cmp r7, #255
	bls .L_080180d8
	movs r7, #64
.L_080180d8:
	ldr r0, [sp, #16]
	cmp r0, #0
	beq .L_08018188
	cmp r7, #31
	bls .L_080180e4
	b .L_08018614
.L_080180e4:
	cmp r7, #18
	beq .L_0801815e
	cmp r7, #18
	bhi .L_0801810e
	cmp r7, #9
	bhi .L_08018102
	cmp r7, #8
	bcs .L_0801815e
	cmp r7, #1
	beq .L_08018166
	cmp r7, #1
	bcc .L_0801813c
	cmp r7, #2
	beq .L_0801813c
	b .L_08018614
.L_08018102:
	cmp r7, #16
	bne .L_08018108
	b .L_08018614
.L_08018108:
	cmp r7, #17
	beq .L_0801815e
	b .L_08018614
.L_0801810e:
	cmp r7, #22
	beq .L_08018142
	cmp r7, #22
	bhi .L_08018128
	cmp r7, #20
	beq .L_08018146
	cmp r7, #20
	bhi .L_08018150
	mov r0, r10
	bl _call_via_r9
	movs r0, #3
	b .L_08018156
.L_08018128:
	cmp r7, #29
	beq .L_0801815e
	cmp r7, #29
	bhi .L_08018136
	cmp r7, #23
	beq .L_08018154
	b .L_08018614
.L_08018136:
	cmp r7, #30
	beq .L_0801813c
	b .L_08018614
.L_0801813c:
	movs r1, #0
	str r1, [sp, #20]
	b .L_08018614
.L_08018142:
	movs r0, #5
	b .L_08018156
.L_08018146:
	mov r0, r10
	bl _call_via_r9
	movs r0, #2
	b .L_08018156
.L_08018150:
	movs r0, #4
	b .L_08018156
.L_08018154:
	movs r0, #6
.L_08018156:
	mov r1, r11
	bl UiRender_LookupNamedValue
	b .L_08018614
.L_0801815e:
	mov r0, r10
	bl _call_via_r9
	b .L_08018614
.L_08018166:
	movs r2, #0
	str r2, [sp, #20]
	movs r7, #2
	b .L_08018614
	.2byte 0x0000
.L_08018170:
	.4byte Data_03001e8c
.L_08018174:
	.4byte 0x000012b2
.L_08018178:
	.4byte 0x000012b4
.L_0801817c:
	.4byte 0x00000140
.L_08018180:
	.4byte 0x040000d4
.L_08018184:
	.4byte Func_08015430
.L_08018188:
	ldr r0, [sp, #44]
	ldr r1, .L_080181d0
	adds r3, r0, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080181b0
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_080181b0
	cmp r7, #222
	beq .L_080181b0
	cmp r7, #223
	beq .L_080181b0
	ldr r3, .L_080181cc
	lsls r2, r6, #1
	mov r0, r8
	ldr r1, .L_080181d4
	adds r6, #1
	strh r3, [r2, r0]
	ands r6, r1
.L_080181b0:
	ldr r2, [sp, #44]
	ldr r0, .L_080181d8
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0801820e
	ldr r1, [sp, #40]
	cmp r1, #0
	bne .L_0801820e
	cmp r7, #222
	beq .L_0801820e
	cmp r7, #223
	beq .L_0801820e
	b .L_080181dc
.L_080181cc:
	.4byte 0x00000005
.L_080181d0:
	.4byte 0x000012fa
.L_080181d4:
	.4byte 0x000001ff
.L_080181d8:
	.4byte 0x000012fb
.L_080181dc:
	movs r2, #128
	lsls r2, r2, #1
	cmp r5, r2
	bhi .L_0801820e
	cmp r5, #127
	bls .L_0801820e
	cmp r5, #222
	beq .L_0801820e
	cmp r5, #223
	beq .L_0801820e
	cmp r5, #32
	beq .L_0801820e
	cmp r5, #165
	beq .L_0801820e
	cmp r5, #161
	beq .L_0801820e
	cmp r5, #164
	beq .L_0801820e
	ldr r3, .L_0801822c
	lsls r2, r6, #1
	mov r5, r8
	ldr r0, .L_08018230
	adds r6, #1
	strh r3, [r2, r5]
	ands r6, r0
.L_0801820e:
	cmp r7, #31
	bls .L_0801828c
	ldr r1, [sp, #44]
	ldr r2, .L_08018234
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08018268
	cmp r7, #32
	beq .L_08018228
	ldr r3, [sp, #28]
	cmp r3, #10
	bls .L_08018268
.L_08018228:
	ldr r0, .L_08018230
	b .L_08018238
.L_0801822c:
	.4byte 0x000000de
.L_08018230:
	.4byte 0x000001ff
.L_08018234:
	.4byte 0x000012fa
.L_08018238:
	lsls r3, r6, #1
	ldr r2, .L_08018264
	adds r6, #1
	ands r6, r0
	mov r5, r8
	strh r2, [r3, r5]
	lsls r3, r6, #1
	adds r6, #1
	ands r6, r0
	mov r1, r8
	strh r2, [r3, r1]
	lsls r3, r6, #1
	strh r2, [r3, r5]
	adds r6, #1
	ldr r1, [sp, #28]
	ands r6, r0
	movs r0, #1
	str r0, [sp, #16]
	cmp r1, #10
	bls .L_08018268
	movs r7, #32
	b .L_08018268
.L_08018264:
	.4byte 0x0000002e
.L_08018268:
	cmp r7, #34
	bne .L_0801827a
	ldr r2, [sp, #36]
	movs r3, #1
	eors r2, r3
	str r2, [sp, #36]
	cmp r2, #0
	beq .L_0801827a
	movs r7, #142
.L_0801827a:
	ldr r0, .L_0801856c
	lsls r3, r6, #1
	mov r5, r8
	adds r6, #1
	movs r1, #0
	strh r7, [r3, r5]
	ands r6, r0
	str r1, [sp, #40]
	b .L_08018614
.L_0801828c:
	cmp r7, #20
	bne .L_08018292
	b .L_080183d8
.L_08018292:
	cmp r7, #20
	bhi .L_080182d2
	cmp r7, #9
	bhi .L_080182b4
	cmp r7, #8
	bcs .L_08018320
	cmp r7, #1
	bne .L_080182a4
	b .L_080185f0
.L_080182a4:
	cmp r7, #1
	bcc .L_0801831a
	cmp r7, #2
	beq .L_0801831a
	cmp r7, #3
	bne .L_080182b2
	b .L_080185f0
.L_080182b2:
	b .L_080185f4
.L_080182b4:
	cmp r7, #17
	bne .L_080182ba
	b .L_080184e4
.L_080182ba:
	cmp r7, #17
	bhi .L_080182c6
	cmp r7, #16
	bne .L_080182c4
	b .L_08018486
.L_080182c4:
	b .L_080185f4
.L_080182c6:
	cmp r7, #18
	bne .L_080182cc
	b .L_080184aa
.L_080182cc:
	cmp r7, #19
	beq .L_080183a6
	b .L_080185f4
.L_080182d2:
	cmp r7, #25
	bne .L_080182d8
	b .L_08018588
.L_080182d8:
	cmp r7, #25
	bhi .L_080182f4
	cmp r7, #22
	beq .L_08018344
	cmp r7, #22
	bcs .L_080182e6
	b .L_0801840e
.L_080182e6:
	cmp r7, #23
	bne .L_080182ec
	b .L_08018448
.L_080182ec:
	cmp r7, #24
	bne .L_080182f2
	b .L_08018546
.L_080182f2:
	b .L_080185f4
.L_080182f4:
	cmp r7, #29
	beq .L_08018320
	cmp r7, #29
	bhi .L_0801830a
	cmp r7, #26
	bne .L_08018302
	b .L_0801851e
.L_08018302:
	cmp r7, #27
	bne .L_08018308
	b .L_080185c0
.L_08018308:
	b .L_080185f4
.L_0801830a:
	cmp r7, #30
	beq .L_0801831a
	movs r2, #1
	negs r2, r2
	cmp r7, r2
	bne .L_08018318
	b .L_08018614
.L_08018318:
	b .L_080185f4
.L_0801831a:
	movs r3, #0
	str r3, [sp, #20]
	b .L_08018614
.L_08018320:
	ldr r0, .L_0801856c
	lsls r3, r6, #1
	mov r5, r8
	adds r6, #1
	ands r6, r0
	strh r7, [r3, r5]
	mov r0, r10
	bl _call_via_r9
	ldr r1, .L_08018570
	lsls r3, r6, #1
	adds r0, r0, r1
	mov r2, r8
	strh r0, [r3, r2]
	ldr r3, .L_0801856c
	adds r6, #1
	ands r6, r3
	b .L_08018614
.L_08018344:
	mov r1, r11
	movs r0, #5
	bl UiRender_LookupNamedValue
	adds r1, r0, #0
	adds r3, r1, #0
	cmp r1, #0
	bge .L_08018356
	negs r3, r1
.L_08018356:
	movs r5, #1
	str r5, [sp, #24]
	cmp r3, #1
	bgt .L_08018362
	movs r0, #0
	str r0, [sp, #24]
.L_08018362:
	add r5, sp, #68
	adds r0, r5, #0
	movs r2, #0
	bl UiText_FormatNumber
	subs r4, r0, r5
	cmp r4, #16
	bne .L_08018374
	b .L_08018614
.L_08018374:
	ldrb r3, [r5, r4]
	cmp r3, #0
	bne .L_0801837c
	b .L_08018614
.L_0801837c:
	ldr r1, .L_0801856c
	adds r0, r4, r5
	mov r12, r1
	adds r1, r0, #0
.L_08018384:
	ldrb r3, [r1]
	lsls r2, r6, #1
	mov r5, r8
	strh r3, [r2, r5]
	adds r6, #1
	mov r2, r12
	adds r4, #1
	adds r1, #1
	ands r6, r2
	cmp r4, #16
	bne .L_0801839c
	b .L_08018614
.L_0801839c:
	adds r0, #1
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_08018384
	b .L_08018614
.L_080183a6:
	mov r0, r10
	bl _call_via_r9
	mov r1, r11
	subs r5, r0, #1
	movs r0, #3
	bl UiRender_LookupNamedValue
	adds r2, r0, #0
	ldr r0, .L_08018574
	ldr r1, [sp, #12]
	adds r0, r2, r0
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r3, [sp, #24]
	str r3, [sp, #4]
	add r3, sp, #52
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	ldr r1, [sp, #12]
	mov r3, r8
	str r5, [sp, #0]
	b .L_08018516
.L_080183d8:
	mov r0, r10
	bl _call_via_r9
	mov r1, r11
	subs r5, r0, #1
	movs r0, #2
	bl UiRender_LookupNamedValue
	adds r2, r0, #0
	ldr r0, .L_0801856c
	ands r2, r0
	ldr r0, .L_08018578
	ldr r1, [sp, #12]
	adds r0, r2, r0
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r1, [sp, #24]
	add r3, sp, #52
	str r1, [sp, #4]
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	ldr r1, [sp, #12]
	mov r3, r8
	str r5, [sp, #0]
	b .L_08018516
.L_0801840e:
	mov r1, r11
	movs r0, #4
	bl UiRender_LookupNamedValue
	adds r2, r0, #0
	ldr r0, .L_0801857c
	ldr r1, [sp, #12]
	adds r0, r2, r0
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r1, [sp, #12]
	ldrh r2, [r1]
	adds r3, r2, #0
	adds r0, r6, #0
	cmp r3, #0
	beq .L_0801851a
	ldr r4, .L_0801856c
.L_08018432:
	lsls r3, r0, #1
	mov r5, r8
	strh r2, [r3, r5]
	adds r1, #2
	ldrh r2, [r1]
	adds r0, #1
	adds r3, r2, #0
	ands r0, r4
	cmp r3, #0
	bne .L_08018432
	b .L_0801851a
.L_08018448:
	mov r1, r11
	movs r0, #6
	bl UiRender_LookupNamedValue
	movs r1, #1
	bl Func_0808a5d0
	ldr r3, .L_08018580
	ldr r1, [sp, #12]
	adds r0, r0, r3
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r1, [sp, #12]
	ldrh r2, [r1]
	adds r3, r2, #0
	adds r0, r6, #0
	cmp r3, #0
	beq .L_0801851a
	ldr r4, .L_0801856c
.L_08018470:
	lsls r3, r0, #1
	mov r5, r8
	strh r2, [r3, r5]
	adds r1, #2
	ldrh r2, [r1]
	adds r0, #1
	adds r3, r2, #0
	ands r0, r4
	cmp r3, #0
	bne .L_08018470
	b .L_0801851a
.L_08018486:
	ldr r3, .L_08018584
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	bl Func_08077008
	add r1, sp, #84
	adds r2, r1, #0
	movs r4, #0
.L_0801849a:
	ldrb r3, [r0]
	adds r4, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r4, #14
	bls .L_0801849a
	b .L_08018506
.L_080184aa:
	mov r0, r10
	bl _call_via_r9
	mov r1, r11
	subs r5, r0, #1
	movs r0, #1
	bl UiRender_LookupNamedValue
	bl Func_08077008
	add r1, sp, #84
	adds r2, r1, #0
	movs r4, #0
.L_080184c4:
	ldrb r3, [r0]
	adds r4, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r4, #14
	bls .L_080184c4
	ldr r2, [sp, #24]
	add r3, sp, #52
	str r2, [sp, #4]
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	mov r3, r8
	str r5, [sp, #0]
	b .L_08018516
.L_080184e4:
	mov r0, r10
	bl _call_via_r9
	subs r2, r0, #1
	adds r0, r2, #0
	bl Func_08077008
	add r1, sp, #84
	adds r2, r1, #0
	movs r4, #0
.L_080184f8:
	ldrb r3, [r0]
	adds r4, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r4, #14
	bls .L_080184f8
.L_08018506:
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r3, sp, #52
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	mov r3, r8
.L_08018516:
	bl UiText_AppendArticleName
.L_0801851a:
	adds r6, r0, #0
	b .L_08018614
.L_0801851e:
	mov r0, r10
	bl _call_via_r9
	subs r0, #1
	lsls r0, r0, #1
	ldr r1, .L_0801856c
	lsls r3, r6, #1
	adds r2, r0, #0
	adds r6, #1
	adds r2, #128
	ands r6, r1
	mov r5, r8
	strh r2, [r3, r5]
	adds r0, #129
	lsls r3, r6, #1
	mov r2, r8
	adds r6, #1
	strh r0, [r3, r2]
	ands r6, r1
	b .L_08018614
.L_08018546:
	ldr r3, .L_08018564
	ldr r0, .L_0801856c
	lsls r2, r6, #1
	mov r5, r8
	adds r6, #1
	strh r3, [r2, r5]
	ands r6, r0
	ldr r3, .L_08018568
	lsls r2, r6, #1
	mov r1, r8
	adds r6, #1
	strh r3, [r2, r1]
	ands r6, r0
	b .L_08018614
	.2byte 0x0000
.L_08018564:
	.4byte 0x0000008f
.L_08018568:
	.4byte 0x0000002d
.L_0801856c:
	.4byte 0x000001ff
.L_08018570:
	.4byte 0x0000ffff
.L_08018574:
	.4byte 0x00000741
.L_08018578:
	.4byte 0x00000182
.L_0801857c:
	.4byte 0x00000333
.L_08018580:
	.4byte 0x0000099b
.L_08018584:
	.4byte gCell
.L_08018588:
	ldr r2, [sp, #24]
	cmp r2, #0
	beq .L_08018614
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_080185a2
	ldr r3, .L_080185b4
	lsls r2, r6, #1
	mov r5, r8
	ldr r0, .L_080185bc
	adds r6, #1
	strh r3, [r2, r5]
	ands r6, r0
.L_080185a2:
	ldr r3, .L_080185b8
	lsls r2, r6, #1
	mov r1, r8
	strh r3, [r2, r1]
	ldr r2, .L_080185bc
	adds r6, #1
	ands r6, r2
	b .L_08018614
	.2byte 0x0000
.L_080185b4:
	.4byte 0x00000065
.L_080185b8:
	.4byte 0x00000073
.L_080185bc:
	.4byte 0x000001ff
.L_080185c0:
	ldr r2, .L_080185e4
	lsls r3, r6, #1
	mov r5, r8
	strh r2, [r3, r5]
	ldr r1, .L_080185ec
	ldr r3, [sp, #52]
	adds r6, #1
	ands r6, r1
	cmp r3, #0
	bne .L_08018614
	ldr r3, .L_080185e8
	lsls r2, r6, #1
	mov r0, r8
	adds r6, #1
	strh r3, [r2, r0]
	ands r6, r1
	b .L_08018614
	.2byte 0x0000
.L_080185e4:
	.4byte 0x00000027
.L_080185e8:
	.4byte 0x00000073
.L_080185ec:
	.4byte 0x000001ff
.L_080185f0:
	movs r1, #1
	str r1, [sp, #40]
.L_080185f4:
	lsls r3, r6, #1
	mov r2, r8
	strh r7, [r3, r2]
	ldr r3, .L_0801860c
	adds r6, #1
	ands r6, r3
	cmp r7, #115
	beq .L_08018608
	cmp r7, #83
	bne .L_08018610
.L_08018608:
	movs r3, #1
	b .L_08018612
.L_0801860c:
	.4byte 0x000001ff
.L_08018610:
	movs r3, #0
.L_08018612:
	str r3, [sp, #52]
.L_08018614:
	ldr r5, [sp, #28]
	ldr r0, [sp, #20]
	adds r5, #1
	str r5, [sp, #28]
	cmp r0, #0
	beq .L_08018628
	ldr r1, .L_0801866c
	cmp r5, r1
	bhi .L_08018628
	b .L_080180c8
.L_08018628:
	ldr r1, .L_0801866c
	lsls r3, r6, #1
	mov r2, r8
	adds r6, #1
	strh r7, [r3, r2]
	ands r6, r1
	ldr r3, .L_08018668
	lsls r2, r6, #1
	mov r5, r8
	strh r3, [r2, r5]
	adds r3, r6, #1
	ands r3, r1
	ldr r0, [sp, #44]
	ldr r1, .L_08018670
	adds r2, r0, r1
	strh r3, [r2]
	movs r0, #50
	bl Runtime_ReleaseHeapBlock
	ldr r5, .L_08018674
	ldr r2, [sp, #44]
	add r0, sp, #32
	ldrh r0, [r0]
	adds r3, r2, r5
	strh r0, [r3]
.L_0801865a:
	mov r1, r11
	cmp r1, #0
	beq .L_08018678
	bl UiWork_ClearValueNameTables
	b .L_08018678
	.2byte 0x0000
.L_08018668:
	.4byte 0x00000000
.L_0801866c:
	.4byte 0x000001ff
.L_08018670:
	.4byte 0x000012b2
.L_08018674:
	.4byte 0x000012b4
.L_08018678:
	ldr r0, [sp, #32]
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
