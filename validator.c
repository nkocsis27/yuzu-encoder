#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
/*
    Treat everything as unsigned so u_int_...
*/

//\IADD R2, R1, R3 -- Maxwell Instruction
uint32_t IADD32 (uint32_t a, uint32_t b) {
    return a + b;
}

uint64_t IADD64 (uint64_t a, uint64_t b) { 
    return a + b;
}



//if either number is NaN (+/- infinity) result is also NaN
double DADD (double r1, double r2) {
    return r1 + r2;
}



/*
OpIAdd -- SPRI-V
    Wrap around rule/overflow/underflow
        mod 16, retain lower bits -- c handles this
    Subtraction is two's complement notation (steps in notes)
        Treat underflow the same (unsigned numbers)

*/
uint32_t OpIAdd32(uint32_t v1,uint32_t v2) {
    return v1 + v2;
}

uint64_t OpIAdd64(uint64_t v1,uint64_t v2) {
    return v1 + v2;
}

float OpConvertSToF(int32_t signedInt) {
    return (float) signedInt;
}

float OpFAdd(float op1, float op2) {
    return op1 + op2;
}


//smaller bit size than 32 for int type? only 1 or 0, boolean?
uint32_t OpBitwiseAnd(uint32_t op1, uint32_t op2) {
    if (op1 == 1 && op2 == 1) {
        return 1;
    } else {
        return 0;
    }
}

//TODO spir-v implementation of double addition

union doubleToInt64 {
    double f;
    uint64_t i;
} v; //variable v of type doubleToInt64

uint64_t f64toU64(double d) {
    v.f = d;
    return v.i;

}


//bitcast from f32 to u32
union floatToUint32 {
    float f;
    uint32_t i;
} t;

uint32_t f32ToU32(float fa) {
    t.f = fa;
    return t.i;
}



//Bitcase from uint32_t to float.
union int32ToFloat {
    uint32_t i;
    float f;
} u;

float U32toF32(uint32_t t) {
    u.i = t;
    return u.f;
}

//CompositeExtract
uint32_t CompositeExtract(uint64_t packed, int index) {
    //if 0 return lo, if 1 return hi
    if (index == 0 ) {
        packed &= 0xffffffffu;
        return packed;
    } else {
        packed >>= 32;
        return packed;
    }
}

//BitFieldExtract
uint32_t BitFieldExtract(uint32_t base, uint32_t offset, uint32_t count) {
    base >>= offset;
    base &= (1 << count) - 1;
    return base;
}


//BitwiseOr
uint32_t BitwiseOr (uint32_t mantissa_hi, uint32_t mantissa_lo) {
    return (mantissa_hi | mantissa_lo);
}

//ShiftLeftLogical
uint32_t ShiftLeftLogical (uint32_t mantissa_hi, uint32_t shift) {
    if (shift >= 32) {
        //Handle error
        printf("Undefined Shift\n");
    } else {
        mantissa_hi <<= shift ;
    }
    //shift left is the same as multiplying by base^shift
    //FIXME special case for shifting 32 or greater
    return mantissa_hi;
}

//Select, like a (?)
uint32_t Select (bool condition, uint32_t ifTrue, uint32_t ifFalse) {
    return (condition ? ifTrue : ifFalse);
}

//IEqual
bool IEqual (uint32_t lhs, uint32_t rhs) {
    if (lhs == rhs) {
        return true;
    } else {
        return false;
    }
}

float PackedF64ToF32(uint64_t value) {
    uint32_t lo = CompositeExtract(value,0);
    uint32_t hi = CompositeExtract(value,1);
    uint32_t sign = BitFieldExtract(hi,31,1);
    uint32_t exp = BitFieldExtract(hi, 20, 11);
    uint32_t mantissa_hi = BitFieldExtract(hi, 0, 20);
    uint32_t mantissa_lo = BitFieldExtract(lo, 29, 3);
    uint32_t mantissa = BitwiseOr(ShiftLeftLogical(mantissa_hi,3), mantissa_lo);
    uint32_t exp_if_subnorm = Select(IEqual(exp, 0), 0, IADD32(exp,1023 - 127));
    uint32_t exp_if_infnan = Select(IEqual(exp,0x7ff),0xff, exp_if_subnorm);
    uint32_t result = BitwiseOr(ShiftLeftLogical(sign,31), BitwiseOr(ShiftLeftLogical(exp_if_infnan,23), mantissa));
    return U32toF32(result);
}



// double SprivDoubleAdd (double a, double b) {
//     return (double)OpIAdd64(yuzuLowered(a), yuzuLowered(b));
// }




// //32 bit float only
// float OpDADD(double op1, double op2) {
//     return (float)op1 + (float)op2;
// }




/*
IAdd -- Translation or IR (intermediate representation)
IAdd.X -- with carry from previous operation
    IAdd_X(a,b,x) where x is 0 or 1 indicating a carry

*/
int IAdd(int a, int b){
    return a + b;
}

int IAdd_X(int a, int b, bool x) {
    if (x) {
        //with carry
        int y = 0; //get the value of the carry
        IAdd(IAdd(a,b),y); 
    } else {
        //without carry
        return IAdd(a,b);
    }
    return 0;
}

// void fuzz(const uint8_t *Data, size_t Size) {

//     double X = DADD(Data[0], Data[1]);
//     double Y = SprivDoubleAdd(Data[0], Data[1]);
//     assert(X == Y);
// }

//TODO FUZZER
// extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
//   fuzz(Data, Size);
//   return 0;
// }

double SprivDoubleAdd (double a, double b) {
    //FIXME figure out what is a and b (or their hi and lo)
    //why is there no composite extract?
    uint32_t p66 = ShiftLeftLogical(524288,3); //line 130
    uint32_t p67 = BitwiseOr(p66, 0);
    uint32_t p70 = ShiftLeftLogical(904, 23);
    uint32_t p71 = BitwiseOr(p70, p67);
    uint32_t p73 = ShiftLeftLogical(0,31);
    uint32_t p74 = BitwiseOr(p73 p71);
    uint32_t p75 = U32toF32(p74);

    uint32_t p79 = ShiftLeftLogical(524288,3);
    uint32_t p80 = BitwiseOr(p79, 0);
    uint32_t p82 = ShiftLeftLogical(1414,23);
    uint32_t p83 = BitwiseOr(p82, p80);
    uint32_t p84 = ShiftLeftLogical(0,31);
    uint32_t p85 = BitwiseOr(p84,p83);
    uint32_t p86 = U32toF32(p85);

    float p87 = OpFAdd(p86,p75);
    uint32_t p88 = f32ToU32(p87);

    uint32_t p90 = BitFieldExtract(p88,31,1);
    uint32_t p91 = BitFieldExtract(p88,23,8);
    uint32_t p92 = BitFieldExtract(p88,0,23);
    uint32_t p94 = BitFieldExtract(p92,3,20);
    uint32_t p95 = BitFieldExtract(p92,0,3);

    uint32_t p97 = OpIAdd32(p91,4294966400);
    bool p98 = IEqual(p91,0);
    uint32_t p99 = Select(p98,0,p97);
    bool p101 = IEqual(p91,255);
    uint32_t p103 = Select(p101, 2047, p99);
    uint32_t p105 = ShiftLeftLogical(p95,29);
    uint32_t p106 = ShiftLeftLogical(p103,20);
    uint32_t p107 = BitwiseOr(p106,p94);
    uint32_t p108 = ShiftLeftLogical(p90,31);
    uint32_t p109 = BitwiseOr(p108,p107);
    float p111 = CompositeConstruct(7864320,p60);
    uint64_t p112 = f64toU64(p111);
    float p113 = CompositeConstruct(p105,p109);
    //FIXME return the correct double
    return p112;

    /*
    //split double a into hi %64 and lo %65
    uint32_t a_hi = CompositeExtract(a,0);
    uint32_t a_lo = CompositeExtract(a,1);


    uint32_t p68 = BitFieldExtract(a_hi, 31, 1);
    uint32_t p71 = BitFieldExtract(a_hi, 20, 11);
    uint32_t p72 = BitFieldExtract(a_hi, 0, 20);
    uint32_t p75 = BitFieldExtract(a_lo, 29, 3);
    uint32_t p76 = ShiftLeftLogical(p72, 3);
    uint32_t p77 = BitwiseOr(p76, p75);
    uint32_t p79 = OpIAdd32(p71, 896);
    bool p80 = IEqual(p71, 0);
    uint32_t p81 = Select(p80, 0, p79);
    bool p83 = IEqual(p71, 2047);
    uint32_t p85 = Select(p83, 225, p81);
    uint32_t p87 = ShiftLeftLogical(p85, 23);
    uint32_t p88 = BitwiseOr(p87, p77);
    uint32_t p89 = ShiftLeftLogical(p68, 31);
    uint32_t p90 = BitwiseOr(p89, p88);
    float p91 = U32toF32(p90); //bitcast

    //split b into hi %62 and lo %63
    uint32_t b_hi = CompositeExtract(b,0);
    uint32_t b_lo = CompositeExtract(b,1);


    uint32_t p92 = BitFieldExtract(b_hi, 31, 1);
    uint32_t p93 = BitFieldExtract(b_hi, 20, 11);
    uint32_t p94 = BitFieldExtract(b_hi, 0 , 20);
    uint32_t p95 = BitFieldExtract(b_lo, 29, 3);
    uint32_t p96 = ShiftLeftLogical(p94, 3);
    uint32_t p97 = BitwiseOr(p96, p95);
    uint32_t p98 = OpIAdd32(p93, 896);
    bool p99 = IEqual(p93, 0);
    uint32_t p100 = Select(p99, 0, p98);
    bool p101 = IEqual(p93, 2047);
    uint32_t p102 = Select(p101, 255, p100);
    uint32_t p103 = ShiftLeftLogical(p102, 23);
    uint32_t p104 = BitwiseOr(p103, p97);
    uint32_t p105 = ShiftLeftLogical(p92, 31);
    uint32_t p106 = BitwiseOr(p105, p104);
    float p107 = U32toF32(p106); //bitcast

    float p108 = OpFAdd(p107, p91); //
    uint32_t p109 = f32ToU32(p108); //float to u32

    uint32_t p110 = BitFieldExtract(p109, 0, 23);
    uint32_t p111 = BitFieldExtract(p110, 0, 3);
    uint32_t p112 = ShiftLeftLogical(p111, 29);
    */

    //%113 = CompositeConstruct (%u32x2) p61 p60
    // Type vector of two u32 elements

    //FIXME return double
}

int main(int argc, char const *argv[])
{
    // uint32_t R2, R1 = 15, R3 = 5;
    // R2 = IADD32 (R1,R3);
    // printf("Maxwell IADD: %u\n", R2);

    // uint64_t X2, X1 = 15, X3 = 5;
    // X2 = OpIAdd64 (X1,X3);
    // printf("SPRI-V OpIAdd: %llu\n", X2);

    // float result;
    // uint32_t i = 5;
    // result = OpConvertSToF(i);
    // printf("OpConvertSToF: %f\n", result);

    // float opResult, op1 = 1, op2 = 4;
    // opResult = OpFAdd(op1, op2);
    // printf("OpFAdd: %f\n", opResult);

    // uint32_t x, a = 1, b = 1;
    // x = OpBitwiseAnd(a,b);
    // printf("OpBitwiseAnd: %u\n", x);


    double X, Y, b2 = 7.4, b3 = 2.0;
    X = DADD(b2, b3);
    printf("DADD: %f\n", X);

    Y = SprivDoubleAdd(b2,b3);
    printf("SpirV Double Add: %f\n", Y);


    // assert (R1 == X1);
    // assert (R3 == X3); 
    // assert (R2 == X2); 
    // assert (b1 == c1);
    //\X is DADD and Y is Spir-v interpretation of double addition.






    // assert(X == Y);

    
    return 0;
}
