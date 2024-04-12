/* Include guard */
#ifndef SHA256_H
#define SHA256_H

/* Some types */
#include "utils.h"

/* Hash type */
/* Hash type */
typedef struct {
  /* A quick way to encode 256 bits in C */
  u32 value[8];
} hash_t;

/* Declarations */
/* Convert a 64-bit number from little-endian to big-endian */
u64 _big_endian64(u64 val);
/* Convert a 32-bit number from little-endian to big-endian */
u32 _big_endian32(u32 val);
/* Prints a byte in binary form to stdout */
void _puts_binary8(u8 val);
/* Prints a u32 in binary form to stdout */
void _puts_binary32(u32 val);
/* Left rotation binary u32 */
u32 _rot_left(u32 val, u8 amount);
/* Right rotation binary u32 */
u32 _rot_right(u32 val, u8 amount);
/* Left shift binary u32 */
u32 _shift_left(u32 val, u8 amount);
/* Right shift binary u32 */
u32 _shift_right(u32 val, u8 amount);
/* Add two binary u32s */
u32 _add_u32(u32 a, u32 b);
/* Generate a hash from a message using the sha256 algorithm */
hash_t sha256(const char* cleartext);

#endif /* SHA256_H */
