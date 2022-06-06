MCASM = ..\bin\mcasm.exe


all : rom0.bin

debug : 
	$(MCASM) --debug=g microcode.mc

silent : 
	$(MCASM) -s microcode.mc

rom0.bin : Makefile microcode.mc inputs.mc signals.mc
	$(MCASM) microcode.mc
