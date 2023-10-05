#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_SIZE_BYTES 64
#define OUTPUT_SIZE_BYTES 20

char* HMAC(char* secret_key, int key_length, int* message, int message_length);
char* SHA1(char* hash_msg);
char* XOR(char*a, char*b);
int convert_int_to_bit_array(int input, int* bit_array, int bit_array_length);
void print_int_array(int * arr, int length);

int main() {
  

}

char* HMAC(char* secret_key, int key_length, int* message, int message_length) {
 



  // return SHA1(XOR(k_block, opad) + H(XOR(k_block, ipad)) + message);
}

void pad_message(int * message, int message_length) {
   // Pre-Processing
  int total_used_bits = message_length+1+64;
  int block_count = (total_used_bits % (BLOCK_SIZE_BYTES*8)) + 1;
  int padded_message_length = block_count*(BLOCK_SIZE_BYTES*8);
  
  // Allocate 64 integer bit array
  int * message_length_binary = calloc(64, sizeof(int));

  // Store message_length in bit array
  if (convert_int_to_bit_array(message_length, message_length_binary, 64)) {
    printf("Input integer too large for given bit array size\n");
  }


  int *padded_message = malloc((sizeof(char))*padded_message_length);

  int i = 0;
  for (; i < message_length; i++) {
    padded_message[i] = message[i];
  }

  padded_message[i] = 1;
  
  for (;i < padded_message_length-64; i++) {
    padded_message[i] = 0;
  }

  for (int j = 0; i < padded_message_length; i++, j++) {
    padded_message[i] = message_length_binary[j];
  }

  print_int_array(padded_message, padded_message_length);
}

int convert_int_to_bit_array(int input, int* bit_array, int bit_array_length) {
  // Calculate amount of necessary digits
  int required_digits = ceil(log2(input*1.0));

  // Return error if array is not large enough
  if (required_digits > bit_array_length) {
    return 1;
  }

  // Convert input integer to binary, starting from the end of the array (big endian)
  for (int i = bit_array_length-1; input > 0; i--) {
    bit_array[i] = input % 2;
    input = input/2;
  }

  return 0;
}

void print_int_array(int * arr, int length) {
  for (int i = 0; i < length; i++) {
    printf("%d", arr[i]);
  }
  printf("\n");
}

// 67 % 2 = 1
// 67 / 2 = 33
// 33 % 2 = 1
// 33 / 2 = 16
// 16 % 2 = 0
// 16 / 2 = 8
// 8 % 2 = 0
// 4 % 2 = 0
// 2 % 2 = 0
// 1 % 2 = 1

// log2(67) round up = number of digits