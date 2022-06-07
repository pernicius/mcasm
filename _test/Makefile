MCASM = ..\bin\mcasm.exe


all : rom0.hex

debug : 
	$(MCASM) --debug=p microcode.mc

silent : 
	$(MCASM) -s microcode.mc

rom0.hex : Makefile microcode.mc inputs.mc signals.mc def_op.mc def_itypes.mc op_RV32I.mc op_RV32Zicsr.mc op_RV32M.mc
	$(MCASM) microcode.mc
