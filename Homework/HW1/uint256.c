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
  str[strlen(hex)] = '\0';
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
  hex[64] = '\0'; 
  for (int i = 7; i >= 0; i--) {
    char *buf = (char*) malloc(sizeof(char) * 9);
    buf[8] = '\0'; 
    uint32_t num = uint256_get_bits(val, i);
    sprintf(buf, "%x", num);
    strncat(hex, buf, 8U); 
    free(buf); 
  }
  char *cut = r_lead_zeroes(hex); 
  free(hex); 
  return cut; 
}

char* r_lead_zeroes(const char* hex) {
  int c = -1;
  char * cut = (char *) malloc(sizeof(char) * (strlen(hex) + 1));
  cut[strlen(hex)] = '\0';  
  for (unsigned i = 0; i < strlen(hex); i++) {
      if (hex[i] != '0') {
          c = i;
          break;
      }
  }
  strcpy(cut, hex + c);
  return cut; 
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
    nbits %= 256; //makes sure nbits within range
    uint32_t carry = 0; // in case any bits are pushed beyond, brings it back to least sig
    for (int i = 0; i < 8; i++) { //8 x 32 = 256
        int srcIndex = (i - nbits / 32 + 8) % 8; //source index
        int nextIndex = (i - 1 + 8) % 8; //

        int bitShift = nbits % 32; //cal # of bits to shift in current 32 word

        //performs the rotation, shifts val.data to left by bitShift
        //0xFF... makes sure that only lower 32 bits are used
        result.data[i] = (val. data [srcIndex] << bitShift) | ((val. data [nextIndex] > (32 - bitShift)) & 0xFFFFFFFF) ;
        if (bitShift > 0 && i > 0) {
            carry = (uint32_t) (val.data[nextIndex] >> (32 - bitShift));
        }
        }
        if (nbits > 0) {
            result.data [0] = carry;
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
        //cal the source and next indices for bit manipulation
        int srcIndex = (i + nbits / 32) % 8;
        int nextIndex = (i + 1) % 8;

        int bitShift = nbits % 32;

        //performs the right rotation on the current word
        result.data[i] = (val.data[srcIndex] >> bitShift) | ((val.data[nextIndex] << (32 - bitShift)) & 0xFFFFFFFF);

        //updates the carry if necessary for bit wrap-around
        if (bitShift > 0 && i < 7) {
            carry = (uint32_t)(val.data[nextIndex] << (32 - bitShift));
        }
    }
  return result;
}
