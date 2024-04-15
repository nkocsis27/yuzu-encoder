This repository contains the code and files for translation validation of Yuzu, a Nintendo Switch game emulator. The validation focuses on the translation from the DADD instruction in Maxwell,
to the Intermediate Representation (IR), then the optimized IR, and finally SpirV. 

Key Files:
/DADD/dadd_test_code.sass --- Sample SASS file passed in to the shader recompiler standalone to begin the translation of the DADD instruction.
/DADD/updatedDADD.txt --- SpirV output of sample SASS file with DADD.
/DADD/DADDwithoutCP --- SpirV output of same sample SASS file without constant propagation enabled.
/encoder.py --- Instruction encoder for Maxwell Instructions (incorrect MOV32I encoder)
/advancedEncoder.py --- Encoder in Python for the Mov32I Maxwell Instruction (correct). 
/nvdisasm_python.py --- Disassembler tool to create SASS files (dadd_test_code.sass) based on bitstring encodings produced from the encoders. 
/validator.c --- Contains the semantics for Maxwell instructions as well as SpirV and runs an assertion check upon the outputs of the two functions using CBMC.
