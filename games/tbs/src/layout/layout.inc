
	.macro layout_header left:req, top:req, right:req, bottom:req, priority0:req, priority1:req, priority2:req, chars0:req, chars1:req, chars2:req, pad0:req, pad1:req
	.byte \left, \top, \right, \bottom
	.byte \priority0, \priority1, \priority2, \chars0, \chars1, \chars2
	.byte \pad0, \pad1
	.endm

	.macro layout_layer x:req, y:req, rate_x:req, rate_y:req, drift_x:req, drift_y:req, a:req, b:req
	.byte \x, \y, \rate_x, \rate_y, \drift_x, \drift_y, \a, \b
	.endm

	.macro layout_offset header:req, part
	.ifnb \part
	.4byte \part - \header
	.else
	.4byte 0
	.endif
	.endm
