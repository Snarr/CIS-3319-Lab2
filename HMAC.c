#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE_BYTES 64
#define OUTPUT_SIZE_BYTES 20

char* HMAC(char* secret_key, int key_length, int* message, int message_length);
void pad_message(int * message, int message_length, int * padded_message, int padded_message_length);
char* XOR(char*a, char*b);
int * convert_int_to_bit_array(uint32_t input, int bit_array_length);
void print_int_array(int * arr, int length);
uint32_t convert_bit_array_to_int(int* bit_array, int bit_array_length);
int * OR_W(int * word_a, int * word_b, int length);
int * AND_W(int * word_a, int * word_b);
int * NOT_W(int * word);
int * left_rotate(int* arr, int d_positions, int length);

int main() {
  

}

char* HMAC(char* secret_key, int key_length, int* message, int message_length) {
 



  // return SHA1(XOR(k_block, opad) + H(XOR(k_block, ipad)) + message);
}

char* SHA1(char* hash_msg, int message_length) {
  int total_used_bits = message_length+1+64;
  int block_count = (total_used_bits % (BLOCK_SIZE_BYTES*8)) + 1;
  int padded_message_length = block_count*(BLOCK_SIZE_BYTES*8);
  int *padded_message = malloc((sizeof(char))*padded_message_length);
  pad_message(hash_msg, message_length, padded_message, padded_message_length);

  int * h0 = convert_int_to_bit_array(0x67452301, 32);
  int * h1 = convert_int_to_bit_array(0xEFCDAB89, 32);
  int * h2 = convert_int_to_bit_array(0x98BADCFE, 32);
  int * h3 = convert_int_to_bit_array(0x10325476, 32);
  int * h4 = convert_int_to_bit_array(0xC3D2E1F0, 32);

  for (int chunk = 0; chunk < padded_message_length/512; chunk++) {
    int words[80][32] = calloc(80*32, sizeof(int));

    // Copy each word of chunk to "words" array
    for (int word = 0; word < 16; word++) {
      int padded_message_position = chunk*512 + word*32;

      // Copy content of the word
      for (int c = 0; c < 32; c++) {
        words[word][c] = padded_message_position + c;
      }
    }

    // Come up with words 16-79
    for (int word = 16; word < 79; word++) {
      int*new_word = left_rotate(XOR_W(XOR_W(XOR_W(words[word-3], words[word-8]), words[word-14]), words[word-16]), 1, 32);

      for (int i = 0; i < 32; i++) {
        words[word][i] = new_word[i];
      }
    }

    // Initialize hash value for chunk
    int * a = malloc(32*sizeof(int));
    COPY_W(h0, a);

    int * b = malloc(32*sizeof(int));
    COPY_W(h1, b);

    int * c = malloc(32*sizeof(int));
    COPY_W(h2, c);

    int * d = malloc(32*sizeof(int));
    COPY_W(h3, d);

    int * e = malloc(32*sizeof(int));
    COPY_W(h4, e);

    // Inititalize k
    int* k = malloc(32*sizeof(int));

    // Main loop
    for (int i = 0; i < 79; i++) {
      int* f;

      if (i >= 0 && i <= 19) {
        // f = (b and c) OR ((not b) and d)
        f = OR_W(AND_W(b, c), AND_W(NOT_W(b), d), 32);
        convert_int_to_bit_array(0x5A827999, 32);
      } else if (i >= 20 && i <= 39) {
        // f = b xor c xor d
        f = XOR_W(XOR_W(AND_W(b,c),c), d);
        convert_int_to_bit_array(0x6ED9EBA1, 32);
      } if (i >= 40 && i <= 59) {
        // f = (b and c) or (b and d) or (c and d)
        f = OR_W(OR_W(AND_W(b,c), AND_W(b,d)), AND_W(c,d), 32);
        convert_int_to_bit_array(0x8F1BBCDC, 32);
      } else {
        // f = xor c xor d
        f = XOR_W(XOR_W(b, c), d);
        convert_int_to_bit_array(0xCA62C1D6, 32);
      }

      uint32_t temp = convert_bit_array_to_int((a, 5, 32), 32) + convert_bit_array_to_int(f, 32) + convert_bit_array_to_int(e, 32) + convert_bit_array_to_int(k, 32) + convert_bit_array_to_int(words[i], 32);

      COPY_W(d, e);
      COPY_W(c, d);
      COPY_W(left_rotate(b, 30, 32), c);
      COPY_W(a, b);
      COPY_W(temp, a);
    }


    COPY_W(convert_int_to_bit_array(convert_bit_array_to_int(h0, 32) + convert_bit_array_to_int(a, 32), 32), h0);
    COPY_W(convert_int_to_bit_array(convert_bit_array_to_int(h1, 32) + convert_bit_array_to_int(b, 32), 32), h1);
    COPY_W(convert_int_to_bit_array(convert_bit_array_to_int(h2, 32) + convert_bit_array_to_int(c, 32), 32), h2);
    COPY_W(convert_int_to_bit_array(convert_bit_array_to_int(h3, 32) + convert_bit_array_to_int(d, 32), 32), h3);
    COPY_W(convert_int_to_bit_array(convert_bit_array_to_int(h4, 32) + convert_bit_array_to_int(e, 32), 32), h4);
  }

  int* hh = OR_W(
              OR_W(
                OR_W(
                  OR_W(
                    left_shift(h0, 128, 160),
                    left_shift(h1, 96, 160),
                    160
                  ),
                  left_shift(h2, 64, 160),
                  160
                ),
              left_shift(h3, 32, 160),
              160),
              h4,
            160);
}

void pad_message(int * message, int message_length, int * padded_message, int padded_message_length) {
  // Allocate 64 integer bit array
  int * message_length_binary = calloc(64, sizeof(int));

  // Store message_length in bit array
  // if (convert_int_to_bit_array(message_length, message_length_binary, 64)) {
  //   printf("Input integer too large for given bit array size\n");
  // }

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
}

int * convert_int_to_bit_array(uint32_t input, int bit_array_length) {
  int * bit_array = malloc(bit_array_length*sizeof(int));

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

  return bit_array;  
}

uint32_t convert_bit_array_to_int(int* bit_array, int bit_array_length) {
  int sum = 0;

  for (int base = 0; base < 32; base++) {
    if (bit_array[bit_array_length-base] == 0) continue;
    sum += pow(2.0, base*1.0);
  } 

  return sum;
}

void print_int_array(int * arr, int length) {
  for (int i = 0; i < length; i++) {
    printf("%d", arr[i]);
  }
  printf("\n");
}

int * XOR_W(int * word_a, int * word_b) {
  int * output = calloc(32, sizeof(int));

  for (int i = 0; i < 32; i++) {
    if (word_a[i] != word_b[i]) {
      output[i] = 1;
    }
  }

  return output;
}

int * AND_W(int * word_a, int * word_b) {
  int * output = calloc(32, sizeof(int));
  for (int i = 0; i < 32; i++) {
    if (word_a[i] == 1 && word_b[i] == 1) {
      output[i] = 1;
    }
  }

  return output;
}

int * OR_W(int * word_a, int * word_b, int length) {
  int * output = calloc(length, sizeof(int));

  for (int i = 0; i < length; i++) {
    if (word_a[i] == 1 || word_b[i] == 1) {
      output[i] = 1;
    }
  }
  return output;
}

int * NOT_W(int * word) {
  int * output = malloc(32*sizeof(int));
  for (int i = 0; i < 32; i++) {
    output[i] = (word[i] == 1) ? 0 : 1;
  }

  return output;
}

int * COPY_W(int * src_word, int * dest_word) {
  for (int i = 0; i < 32; i++){
    src_word[i] = dest_word[i];
  }
}

int * ADD_W(int * word_a, int * word_b) {
  int * result = calloc(32, sizeof(int));
  int s = 0;

  int a_i = 31;
  int b_i = 31;

  while (a_i >= 0 || b_i >= 0 || s == 1) {
    s += ((a_i >= 0) ? word_a[a_i] : 0);
    s += ((b_i >= 0) ? word_b[b_i] : 0);

    result += (s % 2);

    s /= 2;

    a_i--;
    b_i--;
  }

  return result;
}

int * left_rotate_w(int * int_array) {
  int start = int_array[0];
  int * output_array = calloc(32, sizeof(int));
  for (int i = 1; i < 32; i++) {
    output_array[i-1] = output_array[i];
  }
  output_array[32] = start;

  return output_array;
}

int * left_rotate(int* arr, int d_positions, int length) {
    // Storing rotated version of array
    int temp[length];
 
    // Keeping track of the current index
    // of temp[]
    int k = 0;
 
    // Storing the n - d elements of
    // array arr[] to the front of temp[]
    for (int i = d_positions; i < length; i++) {
        temp[k] = arr[i];
        k++;
    }
 
    // Storing the first d elements of array arr[]
    //  into temp
    for (int i = 0; i < d_positions; i++) {
        temp[k] = arr[i];
        k++;
    }
 
    // Copying the elements of temp[] in arr[]
    // to get the final rotated array
    for (int i = 0; i < length; i++) {
        arr[i] = temp[i];
    }

    return arr;
}

int* left_shift(int * arr, int d, int size) {
  int * output = calloc(160, sizeof(int));

  int most_significant_digit;

  for (int i = 0; i < size; i++) {
    if (arr[i] == 1) {
      most_significant_digit = i;
      break;
    }
  }

  for (int i = most_significant_digit; i < size; i++) {
    if (i-d < 0) continue;
    output[i-d] = output[i];
  }

  return output;
}