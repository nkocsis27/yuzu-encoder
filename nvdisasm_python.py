import subprocess
import os
import sys

# this is some random 4 instruction sequence that I grabbed from a real disassembly, 
# though it was for Pascal.
# Make sure it works for maxwell, or try to get a 4 instruction sequence using cuobjdump 
# on some binary (we can chat briefly over email or zoom if you need help on this)
INITIAL = bytearray(b'\xf2\x07!\xfe\x00\xbc\x1f\x00\x00\x02G\x00\x00\x88\x10\\\x00\x00\x07\x01\x00\x00@\xe2\x00\x0f\x07\x00\x00\x00\xb0P')

working_text = INITIAL.copy()

encode = bytearray(b'\x00*\x00\x00\x00\x00\xba\\')
stg = bytearray(b'\x00\x01\x00\x00\x00 \xd8\xee')
exit = bytearray(b'\x00\x00\x00\x00\x00\x00\x00\xe3')

# edit parts of working_text here
# first 64 bytes correspond to the third instruction, so stick to this
# e.g., you can edit bytes like so:
# for i in range(8):  # have to edit all 8 bytes of the encoding
#     working_set[i] = x  # here, x is the corresponding part of the instruction encoding.
for i in range(8):
    working_text[i+8] = encode[i]
    
for i in range(8):
    working_text[i+16] = stg[i]
    
for i in range(8):
    working_text[i+24] = exit[i]
    


with open('tmp_bin', 'wb') as outFile:
    outFile.write(working_text)

sp_args = ["nvdisasm", "-b", "SM53", "tmp_bin", "-hex"]
try:
    res = subprocess.run(sp_args, capture_output=True, check=True)
    txt = res.stdout.decode('utf-8')#.split('\n')[2]
    f = open("instructs","w")
    f.write(txt)
    # print out whichever instruction you modify, change 'n' to whichever is appropriate here.
    print(txt)
    #print("nvdisasm output:", res.stdout.decode('utf-8').split('\n')[1])
except subprocess.CalledProcessError:
    print("nvdisasm error, likely due to an invalid instruction encoding", file=sys.stderr)
