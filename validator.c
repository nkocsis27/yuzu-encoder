#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
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


//TODO DADD
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

//FIXME: floating point type (signed int only up to 32)
float OpConvertSToF(int32_t signedInt) {
    //FIXME: cast to float?
    return (float) signedInt;
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
//Converts from double to int and performs arithmetic

//FIXME allignment wrong? 64 bit int wrong?
//memcpy function?
union doubleToInt64 {
    double f;
    u_int64_t i;
} v; //variable v of type doubleToInt64

u_int64_t yuzuLowered(double d) {
//FIXME missing details 
    v.f = d;
    printf("%llu\n",v.i);
    return v.i;

}

double SprivDoubleAdd (double a, double b) {
    return (double)OpIAdd64(yuzuLowered(a), yuzuLowered(b));
}




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

//TODO FUZZER
int LLVMFuzzerTestOneInput(double a, double b) {
  SprivDoubleAdd(a, b);
  return 0;
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
    
    


    assert(X == Y);

    
    return 0;
}
