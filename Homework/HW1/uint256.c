/*
* Functions implementing simple C library operations on a 256-bit unsigned integer data type
* CSF Assignment 1
* R. Nguyen
* rnguye14@jhu.edu
* J. Liu
* jliu259@jhu.edu
*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"

// Create a UInt256 value from a single uint32_t value.
// Only the least-significant 32 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u32(uint32_t val) {
  UInt256 result;
  result.data[0] = val; //Initialize least significant bit (index = 0) to val
  for (unsigned i = 1; i < 8; ++i) {
    result.data[i] = 0; //Initialize all other bits to 0
  }
  return result;
}

// Create a UInt256 value from an array of NWORDS uint32_t values.
// The element at index 0 is the least significant, and the element
// at index 7 is the most significant.
UInt256 uint256_create(const uint32_t data[8]) {
  UInt256 result;
  for (unsigned i = 0; i < 8; ++i){
    result.data[i] = data[i]; //Direct copy of data to result array
  }
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result = uint256_create_from_u32(0U); //Initialize empty UInt256 w/ 0s 
  char * str = (char *) malloc(sizeof(char) * (strlen(hex) + 1)); //Allocate enough memory for each hex digit and null character 
  strncpy(str, hex, strlen(hex)); 
  str[strlen(hex)] = '\0';
  int cut = find_cut(hex); //Find which part of hex string to start copying from
  for (unsigned i = 0; strlen(str) > 0 && i < 8; ++i) {
    char buf[9] = {'\0'}; //Initialize "empty" buffer string
    strncpy(buf, str + cut, 8); //Copy over from string to buffer
    unsigned long val = strtoul(buf, NULL, 16); //Convert hex string to unsigned long
    result.data[i] = val; //Set corresponding bits in UInt256 to unsigned val
    *(str + cut) = '\0'; //Terminate string at cut point to prevent unnecessary copying
    cut = find_cut(str); //Find new cut
  }
  free(str); 
  return result;
}

// Determine which part of a string of hexadecimal digits to copy from
unsigned find_cut(const char *hex) {
  return strlen(hex) >= 8 ? strlen(hex) - 8 : 0; //If string is less than 8, cut would be negative so avoid that
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char *hex = (char*) malloc(sizeof(char) * 65); //Initialize potentially 64 character long hex string w/ null
  char* buf = (char*)malloc(9 * sizeof(char)); //Initialize buffer string w/ null
  hex[0] = '\0'; 
  for (int i = 7; i >= 0; i--) {
    uint32_t num = uint256_get_bits(val, i); //Access unsigned long
    sprintf(buf, "%x", num); //Converted unsigned long to hexadecimal  
    strcat(hex, buf); //Copy buffer hex string to main string
  }
  free(buf); 
  char *cut = r_lead_zeros(hex); //Get rid of leading zeros
  free(hex); 
  return cut; 
}

char* r_lead_zeros(const char* hex) {
  char *str = (char*) malloc(sizeof(char) * (strlen(hex) + 1)); // String to conduct operations on
  char* res = (char*) malloc(sizeof(char) * (strlen(hex) + 1)); // Final string to return
  strcpy(str, hex); 
  unsigned counter = 0; //Pointer arithmetic to free memory without errors
  while ((*str == '0') && (strlen(str) > 1)) { //Shorten string as long as there's a 0 and string of nonzero length
    str = str + 1;
    counter++; 
  }
  strcpy(res, str); //Copy shortened string to final result
  free(str - counter); //Free string at original memory location as no other opportunity to do so
  return res; 
}

// Get 32 bits of data from a UInt256 value.
// Index 0 is the least significant 32 bits, index 7 is the most
// significant 32 bits.
uint32_t uint256_get_bits(UInt256 val, unsigned index) {
  uint32_t bits;
  bits = val.data[index]; //Access bit at index
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
    UInt256 sum;
    uint32_t carry = 0; // Initialize carry to 0
    for (int i = 0; i < 8; i++) {
        uint64_t temp_sum = (uint64_t)left.data[i] + (uint64_t)right.data[i] + (uint64_t)carry; //Accounting for how sum may be greater than what 32 bit unsigned can hold
        sum.data[i] = (uint32_t)temp_sum; //Convert back to standard uint_32 format
        carry = (temp_sum > UINT32_MAX) ? 1 : 0; //Decide if carry is needed for next bit
    }
    return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
    UInt256 result;
    uint64_t borrow = 0; //Borrow instead of carry 
    for (int i = 0; i < 8; i++) {
      uint64_t temp_diff = (uint64_t)left.data[i] - (uint64_t)right.data[i] - borrow; //Similar to addition for underflow
      borrow = (temp_diff > (uint64_t)left.data[i]) ? 1 : 0; //Decide to borrow or not 
      result.data[i] = (uint32_t)temp_diff; //Set bit value
    }
    return result;
}

// Return the two's-complement negation of the given UInt256 value.
UInt256 uint256_negate(UInt256 val) {
  UInt256 result;  
  for (unsigned i = 0; i < 8; ++i) {
    result.data[i] = ~(uint256_get_bits(val, i)); //Find complement of each bit
  }
  return uint256_add(result, uint256_create_from_u32(1U)); //Add one to final value
}

// Return the result of rotating every bit in val nbits to
// The left.  Any bits shifted past the most significant bit
// Should be shifted back into the least significant bits.
UInt256 uint256_rotate_left(UInt256 val, unsigned nbits) {
    UInt256 result;
    nbits %= 256; // Ensure nbits is within the range [0, 255]

    // Calculate the number of full 32-bit word shifts
    unsigned wordShift = nbits / 32;
    unsigned bitShift = nbits % 32;

    for (int i = 0; i < 8; i++) {
        // Calculate source and next indices for rotation
        int sourceIndex = (i + wordShift) % 8;
        int nextIndex = (sourceIndex - 1 + 8) % 8;

        // Perform the rotation
        result.data[i] = (val.data[sourceIndex] << bitShift) | (val.data[nextIndex] >> (32 - bitShift));
    }

    return result;
}
// Return the result of rotating every bit in val nbits to
// the right. Any bits shifted past the least significant bit
// should be shifted back into the most significant bits.
UInt256 uint256_rotate_right(UInt256 val, unsigned nbits) {
  UInt256 result;
    nbits %= 256;
    uint32_t carry = 0;
    for (int i = 0; i < 8; i++) {
        // Calculates the source and next indices for bit manipulation
        int sourceIndex = (i + nbits / 32) % 8;
        int nextIndex = (i + 1) % 8;

        int bitShift = nbits % 32;

        // Performs the right rotation on the current word
        result.data[i] = (val.data[sourceIndex] >> bitShift) | ((val.data[nextIndex] << (32 - bitShift)) & 0xFFFFFFFF);

        // Updates the carry if necessary for bit wrap-around
        if (bitShift > 0 && i < 7) {
            carry = (uint32_t)(val.data[nextIndex] << (32 - bitShift));
        }
    }
  return result;
}
