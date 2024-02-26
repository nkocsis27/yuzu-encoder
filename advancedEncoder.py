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

  
    #    self.pred = 0b0111 #does move32i use unpred
        
    def encoding(self):
        self.encodedInstruct = self.dest_reg.shift() | self.mov32i_mask.shift() | self.imm.shift() | self.oppcode.shift()
        return self.encodedInstruct
        

        
        

#FIXME can use array to go through each attribute
    def decoding(self):
        print("dest_reg: ", self.dest_reg.decode(self.encodedInstruct))
        print("mov32i_mask: ", self.mov32i_mask.decode(self.encodedInstruct))
        print("Immediate: ", self.imm.decode(self.encodedInstruct))
        print("Oppcode: ", bin(self.oppcode.decode(self.encodedInstruct)))
        
    def printAllVals(self):
        print(self.dest_reg.val, self.mov32i_mask.val, self.imm.val)

dest = 0b101
movMask = 0xf
im = 0b0111

mov1 = Mov32I(dest, movMask, im)
print(bin(mov1.encoding()))

mov1.printAllVals()

mov1.decoding()


sencodeA = struct.pack('Q',mov1.encoding()) 
f = open("advancedEncodes.txt", "w")
f.write(str(sencodeA)+ '\n')
f.close();
