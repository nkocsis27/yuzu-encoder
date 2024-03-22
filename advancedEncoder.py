import struct

class BitField:
    def __init__ (self, s, l, v):
        self.start = s
        self.length = l
        self.val = v
        self.size = 64
        self.mask = (2**self.length) - 1
        
    def getValue(self):
        return self.val;
    
    def setValue(self, v):
        self.val = v
        
    def shift(self):
        return self.val << self.start
    
    def decode(self, encodedInstruct):
        return (encodedInstruct >> self.start) & (self.mask)

        
        
    
class Mov32I: 
    def __init__ (self, dest_reg, move32i_mask, imm):
        self.dest_reg = BitField(0,8,dest_reg)
        self.mov32i_mask = BitField(12,4, move32i_mask)
        self.imm = BitField(20,32,imm)
        self.oppcode = BitField(52,12,0b000000010000)
        
        self.encodedInstruct = 0b0
        self.pred = BitField(16,4,0b0111)
        
    def encoding(self):
        self.encodedInstruct = self.dest_reg.shift() | self.mov32i_mask.shift() | self.imm.shift() | self.oppcode.shift() | self.pred.shift()
        return self.encodedInstruct
        

        
        

#FIXME can use array to go through each attribute
    def decoding(self):
        print("dest_reg: ", self.dest_reg.decode(self.encodedInstruct))
        print("mov32i_mask: ", self.mov32i_mask.decode(self.encodedInstruct))
        print("Immediate: ", self.imm.decode(self.encodedInstruct))
        print("Oppcode: ", bin(self.oppcode.decode(self.encodedInstruct)))
        
    def printAllVals(self):
        print(self.dest_reg.val, self.mov32i_mask.val, self.imm.val)



#Mov32I(dest_reg, movMask, imm)
movR0 = Mov32I(0, 0xf, 0x4b7f30e2)
movR1 = Mov32I(1, 0xf, 0x734573d8)
movR4 = Mov32I(4, 0xf, 0x4426c4ac)
movR5 = Mov32I(5, 0xf, 0xbebea010)
movR16 = Mov32I(16, 0xf, 0)
movR17 = Mov32I(17, 0xf, 0)

#FIXME return 64 bit value (double)


#FIXME use array here to itterate over mov variables
print(bin(movR0.encoding()))
print(bin(movR1.encoding()))
print(bin(movR4.encoding()))
print(bin(movR5.encoding()))
print(bin(movR16.encoding()))
print(bin(movR17.encoding()))

movR0.decoding()
movR1.decoding()
movR4.decoding()
movR5.decoding()
movR16.decoding()
movR17.decoding()


sencodeR0 = struct.pack('Q',movR0.encoding())
sencodeR1 = struct.pack('Q',movR1.encoding()) 
sencodeR4 = struct.pack('Q',movR4.encoding()) 
sencodeR5 = struct.pack('Q',movR5.encoding()) 
sencodeR16 = struct.pack('Q',movR16.encoding()) 
sencodeR17 = struct.pack('Q',movR17.encoding()) 
 
f = open("advancedEncodes.txt", "w")

f.write(str(sencodeR0)+ '\n')
f.write(str(sencodeR1)+ '\n')
f.write(str(sencodeR4)+ '\n')
f.write(str(sencodeR5)+ '\n')
f.write(str(sencodeR16)+ '\n')
f.write(str(sencodeR17)+ '\n')
f.close();
