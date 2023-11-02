import struct

unpred = 0b0111 #left shift 16 for unpredicated

def encoder_IADD32I(cc, x, sat, three_for_po, neg_a, src, dst, imm):
    opcode = 0b0001110

    # imm = 0b101011001
    # imm >> 27 #from 20 for 32 bits for all imm
    encode = opcode << 57 #opcode - 64 is the left shift
    encode |= (neg_a & 1) << 56
    encode |= (sat & 1) << 54 
    encode |= (x & 1) << 53
    encode |= (cc & 1) << 52
    encode |= (three_for_po & 3) << 55 
    encode |= imm << 20
    encode |= unpred << 16
    
    encode |= (dst & 255) #ensures width is within 255, do 2^bit width
    encode |= (src & 255) << 8
    
    # print(encode)
    return encode

def encoder_FADD32I(ftz, neg_b, abs_a, cc, neg_a, abs_b, dest_reg, src_a, imm):
    opcode = 0b000010 
    #NOTE for imm for floats work differently, so just use all 0's
    e = opcode << 58
    e |= imm << 20
    e |= (abs_b & 1) << 57
    e |= (neg_a & 1) << 56
    e |= (ftz & 1) << 55
    e |= (abs_a & 1) << 54
    e |= (neg_b & 1) << 53
    e |= (cc & 1) << 52
    e |= unpred << 16
    
    e |= (dest_reg & 255) 
    e |= (src_a & 255) << 8
    
    return e

def encoder_DADD_reg(fp_rounding, neg_b, abs_a, cc, neg_a, abs_b, dest_reg, src_a_reg):
    opcode = 0b0101110001110
    e = opcode << 51 
    e |= (fp_rounding & 3) << 39
    e |= (neg_b & 1) << 45
    e |= (abs_a & 1) << 46
    e |= (cc & 1) << 47
    e |= (neg_a & 1) << 48
    e |= (abs_b & 1) << 49
    e |= unpred << 16
    
    e |= (dest_reg & 255) 
    e |= (src_a_reg & 255) << 8
    
    return e

def encoder_LOP32I(bit_op, x, cc, inv_a, inv_b, dest_reg, src_reg, imm):
    opcode = 0b000001

    e = opcode << 58
    e |= imm << 20
    e |= (bit_op & 3) << 53
    e |= (x & 1) << 57
    e |= (cc & 1) << 52
    e |= (inv_a & 1) << 55
    e |= (inv_b & 1) << 56
    e |= unpred << 16
    
    e |= (dest_reg & 255) 
    e |= (src_reg & 255) << 8
    
    return e
    
def encoder_ISETP_reg(bop_pred, neg_bop_pred, x, bop, is_signed, compare_op, dest_pred_b, dest_pred_a, src_reg_a):
    opcode = 0b010110110110
    e = opcode << 52
    e |= (bop_pred & 7) << 39
    e |= (neg_bop_pred & 1) << 42
    e |= (x & 1) << 43
    e |= (bop & 3) << 45
    e |= (is_signed & 1) << 48
    e |= (compare_op & 7) << 49
    e |= unpred << 16
    
    e |= (dest_pred_a & 7) << 3
    e |= (dest_pred_b & 7)
    e |= (src_reg_a & 255) << 8
    
    return e

def encoder_I2F_reg(float_format, int_format, is_signed, fp_rounding, selector, cc, neg, abs, reg, dest_reg) :
    opcode = 0b0101110010111
    e = opcode << 51
    e |= (float_format & 3) << 8
    e |= (int_format & 3) << 10
    e |= (is_signed & 1) << 13
    e |= (fp_rounding & 3) << 39
    e |= (selector & 3) << 41
    e |= (cc & 1) << 47
    e |= (neg & 1) << 45
    e |= (abs & 1) << 49
    e |= (reg & 255) << 20
    e |= unpred << 16
    
    e |= (dest_reg & 255)
    
    return e


def encoder_STG(data_reg, cache, size, addr_reg, addr_offset, rz_addr_offset, eVar):
    opcode = 0b1110111011011
    e = opcode << 51
    e |= (cache & 3) << 46
    e |= (size & 7) << 48

    e |= (addr_reg & 255) << 8
    e |= addr_offset << 20
    e |= rz_addr_offset << 20
    e |= (eVar & 1) << 45
    e |= unpred << 16
    
    e |= (data_reg & 255)
    

    return e

def encoder_EXIT():
    opcode = 0b111000110000
    e = opcode << 52
    e |= 0b01110000000000001111 
    
    return e

def encoder_MOV32I(dest_reg, mask, mov32i_mask):
    opcode = 0b000000010000
    e = opcode << 52
    e |= dest_reg
    e |= mask << 39
    e |= mov32i_mask << 12
    
    return e
    
    

#0b101011001
u64a = encoder_IADD32I(0,1,4,4,1,4,8,0b101011001)
print(hex(u64a))

u64b = encoder_FADD32I(0,1,0,0,1,1,8,4,0b000000)
print(hex(u64b))

u64c = encoder_DADD_reg(5,0,0,0,0,0,8,4)
print(hex(u64c))

u64d = encoder_LOP32I(0,0,0,0,0,8,4,0b000)
print(hex(u64d))

u64e = encoder_ISETP_reg(0,1,0,0,1,0,8,8,4)
print(hex(u64e))

u64f = encoder_I2F_reg(2,2,1,0,0,0,0,1,4,8)
print(hex(u64f))

u64g = encoder_STG(8,0,4,16,0,0,1)
print(hex(u64g))

u64h = encoder_EXIT()
print(hex(u64h))

u64MOV = encoder_MOV32I(8,0b101,0)
print(hex(u64MOV))


#turns into string?

sencodeA = struct.pack('Q',u64a) 
sencodeB = struct.pack('Q',u64b) 
sencodeC = struct.pack('Q',u64c) 
sencodeD = struct.pack('Q',u64d) 
sencodeE = struct.pack('Q',u64e) 
sencodeF = struct.pack('Q',u64f) 
sencodeG = struct.pack('Q',u64g) 
sencodeH = struct.pack('Q',u64h) 

sencodeMOV = struct.pack('Q',u64MOV)

#write to file
f = open("encodes.txt", "w")
f.write(str(sencodeA)+ '\n')
f.write(str(sencodeB)+ '\n')
f.write(str(sencodeC)+ '\n')
f.write(str(sencodeD)+ '\n')
f.write(str(sencodeE)+ '\n')
f.write(str(sencodeF)+ '\n')
f.write(str(sencodeG)+ '\n')
f.write(str(sencodeH)+ '\n')
f.write(str(sencodeMOV)+ '\n')
f.close()
