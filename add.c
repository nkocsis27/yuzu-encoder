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

//fixme change types
uint64_t IADD64 (uint64_t a, uint64_t b) { 
    return a + b;
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


int main(int argc, char const *argv[])
{
// change int to u32

   


    uint32_t R2;
    uint32_t R1 = 15;
    uint32_t R3 = 5;
    R2 = IADD32 (R1,R3);
    printf("Maxwell IADD: %u\n", R2);

    uint64_t X2;
    uint64_t X1 = 15;
    uint64_t X3 = 5;
    X2 = OpIAdd64 (X1,X3);
    printf("SPRI-V OpIAdd: %llu\n", X2);


    assert (R1 == X1);
    assert (R3 == X3); //FIXME assertion fails with signed numbers 
    assert (R2 == X2); 

    
    return 0;
}


