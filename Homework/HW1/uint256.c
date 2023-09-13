/*
* Functions implementing simple C library operations on a 256-bit unsigned integer data type
* CSF Assignment 1
* R. Nguyen
* rnguye14@jhu.edu
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
  result.data[0] = val;
  for (unsigned i = 1; i < 8; ++i) {
    result.data[i] = 0; 
  }
  return result;
}

// Create a UInt256 value from an array of NWORDS uint32_t values.
// The element at index 0 is the least significant, and the element
// at index 7 is the most significant.
UInt256 uint256_create(const uint32_t data[8]) {
  UInt256 result;
  for (unsigned i = 0; i < 8; ++i){
    result.data[i] = data[i];
  }
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result = uint256_create_from_u32(0U); 
  char * str = (char *) malloc(sizeof(char) * (strlen(hex) + 1)); 
  strncpy(str, hex, strlen(hex)); 
  int cut = find_cut(hex); 
  for (unsigned i = 0; strlen(str) > 0 && i < 8; ++i) {
    char buf[9] = {'\0'}; 
    strncpy(buf, str + cut, 8); 
    unsigned long val = strtoul(buf, NULL, 16);
    result.data[i] = val;  
    *(str + cut) = '\0'; 
    cut = find_cut(str); 
  }
  free(str); 
  return result;
}

// Determine which part of a string of hexadecimal digits to copy from
unsigned find_cut(const char *hex) {
  return strlen(hex) >= 8 ? strlen(hex) - 8 : 0; 
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char *hex = (char*) malloc(sizeof(char) * 65);
  for (unsigned i = 0; i < 8; ++i) {
    char *buf = (char*) malloc(sizeof(char) * 9);
    uint32_t num = uint256_get_bits(val, i);
    sprintf(buf, "%x", num);
    strncat(hex, buf, 8U); 
    free(buf); 
  }
  char *sliced = r_lead_zeroes(hex); 
  free(hex); 
  return hex;
}

char* r_lead_zeroes(const char* hex) {
  //TODO
  return; 
}

// Get 32 bits of data from a UInt256 value.
// Index 0 is the least significant 32 bits, index 7 is the most
// significant 32 bits.
uint32_t uint256_get_bits(UInt256 val, unsigned index) {
  uint32_t bits;
  bits = val.data[index]; 
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum = uint256_create_from_u32(0U);
  unsigned carry = 0;
  for (unsigned i = 0; i < 8; ++i) {
    uint32_t leftval = uint256_get_bits(left, i);
    uint32_t rightval = uint256_get_bits(right, i);
    uint32_t sum_val = leftval + rightval + carry; 
    sum.data[i] = sum_val; 
    if (sum_val < leftval) {
      carry = 1; 
    } else {
      carry = 0; 
    }
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  return uint256_add(left, uint256_negate(right));
}

// Return the two's-complement negation of the given UInt256 value.
UInt256 uint256_negate(UInt256 val) {
  UInt256 result;  
  for (unsigned i = 0; i < 8; ++i) {
    result.data[i] = ~(uint256_get_bits(val, i)); 
  }
  return uint256_add(result, uint256_create_from_u32(1U)); 
}

// Return the result of rotating every bit in val nbits to
// the left.  Any bits shifted past the most significant bit
// should be shifted back into the least significant bits.
UInt256 uint256_rotate_left(UInt256 val, unsigned nbits) {
  UInt256 result;
  // TODO: implement
  return result;
}

// Return the result of rotating every bit in val nbits to
// the right. Any bits shifted past the least significant bit
// should be shifted back into the most significant bits.
UInt256 uint256_rotate_right(UInt256 val, unsigned nbits) {
  UInt256 result;
  // TODO: implement
  return result;
}
