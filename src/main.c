// 16bit Single Instruction Computer (emulation) based on Subleq.
// Author: Paul Schulz <paul@mawsonlakes.org>

#include <stdio.h>
#include <stdlib.h>  // Required for exit()
#include <ctype.h>   // Required for isprint()
#include <stdint.h>  // Required for 16bit arithmetic
#include <stdbool.h> // Required for boolean operators

/// \brief Hello World Program
//
/// This program is based on the example (pp) from the paper "A Simple
/// Multi-Processing Computer Based on Subleq" by Oleg Mazonka and Alex Kolodin.
///
int data[] = {
    0x0010, 0xFFFF, 0x0003,  // 0000:  L:H (-1)
    0x000F, 0x0000, 0x0006,  // 0003:    U L
    0x000F, 0x000A, 0x000A,  // 0006:    U ?+2
    0x001D, 0x0010, 0xFFFF,  // 0009:    Z H (-1)
    0x001D, 0x001D, 0x0000,  // 000C:    Z Z L
    0xFFFF, 'h',    'e',     // 000F: .U:-1  H:"hello, world\n"  Z:0
    'l',    'l',    'o',     // 0012:
    ',',    ' ',    'w',     // 0015:
    'o',    'r',    'l',     // 0018:
    'd',    '\n',   0x0000,  // 001B:
    // L=0000
    // U=000F
    // H=0010
    // Z=001D
};


/// \brief Get data
///
/// *Not yet implemented*. The function will block until a data word can be read
/// from the input. For the moment, this just causes program to exit. Will put
/// the input into the location pointed to by "B'.
int input (void){
    exit(1);
}

/// \brief Write data
///
/// Writes out the ASCII character to stdout. Assumes the character is
/// printable, or a newline.
bool output (int value){
    if (isprint(value)) {
        printf("%c",value);
    } else if (value == '\n') {
        printf("\n");
    }
    return true;
}


/// \brief Safe Lookup function
///
/// Look up the value in the memory array checking that the index is within
/// bounds. At the moment, it just checks that the index in non-negative and
/// then just returns this value.
///
/// If this wasn't done then the monitor functions could cause Illegal lookups
/// and crash the system.
int lookup(int* data, int ptr) {
    if (ptr != 0xFFFF) {
        return data[ptr];
    } else {
        return 0xFFFF;
    }
}


/// \brief Display internal machine state
///
/// Display the state of the system to assist in debugging.
///
/// In the future this may be more dynamic allowing selected memory locations to
/// be selected at run time.
bool monitor(int* data, int pc) {
    int ptr_a;
    int ptr_b;
    int ptr_c;
    ptr_a = data[pc];
    ptr_b = data[pc + 1];
    ptr_c = data[pc + 2];

    printf("%04X: %04X(%04X)  %04X(%04X)  %04X",
           pc,
           ptr_a, lookup(data, ptr_a),
           ptr_b, lookup(data, ptr_b),
           ptr_c);

    printf("            ");
    printf(" %04X", data[0x0000]);
    printf(" %04X", data[0x0010]);
    printf(" %04X", data[0x000F]);
    printf(" %04X", data[0x001D]);
    printf("\n");

    return true;
}

/// \brief Process a single machine step.
///
/// Step the machine process, and return the new program counter.
///
int process (int* data, int pc){
    int ptr_a;
    int ptr_b;
    int ptr_c;
    int a;
    int b;
    int new_pc;

    ptr_a = data[pc];
    ptr_b = data[pc + 1];
    ptr_c = data[pc + 2];

    if (ptr_a == 0xFFFF) {
        // Input
        new_pc = pc + 0x03;
    } else if (ptr_b == 0xFFFF) {
        // Output
        output(data[ptr_a]);
        new_pc = pc + 0x03;
    } else {
        // Process
        a = data[ptr_a];
        b = data[ptr_b];

        b = b - (int16_t) a;
        data[ptr_b] = b;
        if (b<=0){
            // Jump
            new_pc = ptr_c;
        } else {
            // Next instruction
            new_pc = pc + 0x03;
        }
    }
    return new_pc;
}


/// Display Usage Information
/**
 *  WIP
 */
void usage (void) {
    printf("sic [arguments]\n");
    printf("  --hello-world  Run inbuilt 'hello, world' program.\n");
}

/// Main Function
/**
 *  WIP Main processing loop. Run the program on the machine until it halts.
 */
int main (int argc, char** argv) {
    printf("Single Instruction Computer\n");
    printf("---------------------------\n");

    // Initialise Program Counter
    int pc = 0;

    while (pc != 0xFFFF) {
        //    monitor(data, pc);
        pc = process(data, pc);
    }
    return 0;
}
