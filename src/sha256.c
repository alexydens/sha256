/* Implements sha256.h */
#include "sha256.h"

/* stdlib headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Macros */
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (_rot_right(x,2) ^ _rot_right(x,13) ^ _rot_right(x,22))
#define EP1(x) (_rot_right(x,6) ^ _rot_right(x,11) ^ _rot_right(x,25))
#define SIG0(x) (_rot_right(x,7) ^ _rot_right(x,18) ^ _shift_right(x, 3))
#define SIG1(x) (_rot_right(x,17) ^ _rot_right(x,19) ^ _shift_right(x, 10))

/* Definitions */
/* Convert a 64-bit number from little-endian to big-endian */
u64 _big_endian64(u64 val) {
  u64 res = 0;
  u8* p_res = (u8*)&res;
  u8* p_val = (u8*)&val;
  p_res[0] = p_val[7];
  p_res[1] = p_val[6];
  p_res[2] = p_val[5];
  p_res[3] = p_val[4];
  p_res[4] = p_val[3];
  p_res[5] = p_val[2];
  p_res[6] = p_val[1];
  p_res[7] = p_val[0];

  return res;
}
/* Convert a 32-bit number from little-endian to big-endian */
u32 _big_endian32(u32 val) {
  u32 res = 0;
  u8* p_res = (u8*)&res;
  u8* p_val = (u8*)&val;
  p_res[0] = p_val[3];
  p_res[1] = p_val[2];
  p_res[2] = p_val[1];
  p_res[3] = p_val[0];
  return res;
}
/* Prints a byte in binary form to stdout */
void _puts_binary8(u8 val) {
  printf(
      "%c%c%c%c%c%c%c%c",
      ((val >> 7) & 1) ? '1' : '0',
      ((val >> 6) & 1) ? '1' : '0',
      ((val >> 5) & 1) ? '1' : '0',
      ((val >> 4) & 1) ? '1' : '0',
      ((val >> 3) & 1) ? '1' : '0',
      ((val >> 2) & 1) ? '1' : '0',
      ((val >> 1) & 1) ? '1' : '0',
      (val & 1) ? '1' : '0'
  );
}
/* Prints a u32 in binary form to stdout */
void _puts_binary32(u32 val) {
  u8* p_val = (u8*)&val;
  _puts_binary8(p_val[0]);
  _puts_binary8(p_val[1]);
  _puts_binary8(p_val[2]);
  _puts_binary8(p_val[3]);
}
/* Left rotation binary u32 */
u32 _rot_left(u32 val, u8 amount) {
  u32 res = val;
  u8* p_res = (u8*)&res;
  u8 i;
  for (i = 0; i < amount; i++) {
    u32 v = res;
    u8* p_val = (u8*)&v;
    p_res[0] = (p_val[0] << 1) + ((p_val[1]>>7) % 2);
    p_res[1] = (p_val[1] << 1) + ((p_val[2]>>7) % 2);
    p_res[2] = (p_val[2] << 1) + ((p_val[3]>>7) % 2);
    p_res[3] = (p_val[3] << 1) + ((p_val[0]>>7) % 2);
  }
  return res;
}
/* Right rotation binary u32 */
u32 _rot_right(u32 val, u8 amount) {
  u32 res = val;
  u8* p_res = (u8*)&res;
  u8 i;
  for (i = 0; i < amount; i++) {
    u32 v = res;
    u8* p_val = (u8*)&v;
    p_res[0] = (p_val[0] >> 1) + (p_val[3] % 2 == 1 ? 128u : 0);
    p_res[1] = (p_val[1] >> 1) + (p_val[0] % 2 == 1 ? 128u : 0);
    p_res[2] = (p_val[2] >> 1) + (p_val[1] % 2 == 1 ? 128u : 0);
    p_res[3] = (p_val[3] >> 1) + (p_val[2] % 2 == 1 ? 128u : 0);
  }
  return res;
}
/* Left shift binary u32 */
u32 _shift_left(u32 val, u8 amount) {
  u32 res = val;
  u8* p_res = (u8*)&res;
  u8 i;
  for (i = 0; i < amount; i++) {
    u32 v = res;
    u8* p_val = (u8*)&v;
    p_res[0] = (p_val[0] << 1) + ((p_val[1]>>7) % 2);
    p_res[1] = (p_val[1] << 1) + ((p_val[2]>>7) % 2);
    p_res[2] = (p_val[2] << 1) + ((p_val[3]>>7) % 2);
    p_res[3] = (p_val[3] << 1);
  }
  return res;
}
/* Right shift binary u32 */
u32 _shift_right(u32 val, u8 amount) {
  u32 res = val;
  u8* p_res = (u8*)&res;
  u8 i;
  for (i = 0; i < amount; i++) {
    u32 v = res;
    u8* p_val = (u8*)&v;
    p_res[0] = (p_val[0] >> 1);
    p_res[1] = (p_val[1] >> 1) + (p_val[0] % 2 == 1 ? 128u : 0);
    p_res[2] = (p_val[2] >> 1) + (p_val[1] % 2 == 1 ? 128u : 0);
    p_res[3] = (p_val[3] >> 1) + (p_val[2] % 2 == 1 ? 128u : 0);
  }
  return res;
}
/* Add two binary u32s */
u32 _add_u32(u32 a, u32 b) {
  u32 res = 0;
  u8* p_a = (u8*)&a;
  u8* p_b = (u8*)&b;
  u8* p_res = (u8*)&res;
  u16 tmp = p_a[3] + p_b[3];
  p_res[3] = tmp;
  tmp = (tmp >> 8) + p_a[2] + p_b[2];
  p_res[2] = tmp;
  tmp = (tmp >> 8) + p_a[1] + p_b[1];
  p_res[1] = tmp;
  tmp = (tmp >> 8) + p_a[0] + p_b[0];
  p_res[0] = tmp;
  return res;
}
/* Generate a hash from a message using the sha256 algorithm */
hash_t sha256(const char* cleartext) {
  hash_t hash;    /* The hash to generate and return */
  u64 i, t;       /* For iteration */
  u64 msglen;     /* The size, in bytes, of the cleartext */
  u64 buff_size;  /* The size, in bytes, of the buffer */
  u64 num_blocks; /* The number of 512-bit blocks to operate on */
  char* buff;     /* The main buffer */
  u32 w[64];      /* Part of algorithm */

  /* Get sizes */
  msglen = strlen(cleartext);
  buff_size = (((msglen+5)/64)+1)*64;
  num_blocks = buff_size/64;
  /* Initialize buffer */
  buff = malloc(buff_size);
  memset(buff, 0x00, buff_size);
  memcpy(buff, cleartext, msglen);
  buff[msglen] = 128u;
  ((u64*)buff)[(buff_size/8)-1] = _big_endian64(msglen*8);
  /* Set initial hash values */
  hash.value[0] = _big_endian32(0x6a09e667);
  hash.value[1] = _big_endian32(0xbb67ae85);
  hash.value[2] = _big_endian32(0x3c6ef372);
  hash.value[3] = _big_endian32(0xa54ff53a);
  hash.value[4] = _big_endian32(0x510e527f);
  hash.value[5] = _big_endian32(0x9b05688c);
  hash.value[6] = _big_endian32(0x1f83d9ab);
  hash.value[7] = _big_endian32(0x5be0cd19);
  /* Const */
  u32 k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
    0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
    0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
    0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
    0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
    0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
    0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
    0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
    0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
  };
  /* Loop through blocks */
  for (i = 0; i < num_blocks; i++) {
    u32 a, b, c, d, e, f, g, h;
    a = hash.value[0];
    b = hash.value[1];
    c = hash.value[2];
    d = hash.value[3];
    e = hash.value[4];
    f = hash.value[5];
    g = hash.value[6];
    h = hash.value[7];
    /* Set w0-63 */
    for (t = 0; t < 16; t++)
      w[t] = ((u32*)buff)[t];
    for (t = 16; t < 64; t++)
      w[t] = 
        _add_u32(
          _add_u32(SIG1(w[t - 2]), w[t - 7]),
          _add_u32(SIG0(w[t - 15]), w[t - 16])
        );
    /* Perform calculations */
    for (t = 0; t < 64; t++) {
      u32 t1 = 
        _add_u32(
          _add_u32(
            _add_u32(h, EP1(e)),
            _add_u32(CH(e, f, g), _big_endian32(k[t]))
          ),
          w[t]
        );/*h + EP1(e) + CH(e,f,g) + k[t] + w[t];*/
      u32 t2 = _add_u32(EP0(a), MAJ(a,b,c));
      h = g;
      g = f;
      f = e;
      e = _add_u32(d, t1);
      d = c;
      c = b;
      b = a;
      a = _add_u32(t1, t2);
    }
    /* Finish round */
    hash.value[0] = _add_u32(a, hash.value[0]);
    hash.value[1] = _add_u32(b, hash.value[1]);
    hash.value[2] = _add_u32(c, hash.value[2]);
    hash.value[3] = _add_u32(d, hash.value[3]);
    hash.value[4] = _add_u32(e, hash.value[4]);
    hash.value[5] = _add_u32(f, hash.value[5]);
    hash.value[6] = _add_u32(g, hash.value[6]);
    hash.value[7] = _add_u32(h, hash.value[7]);
  }

  return hash;
}
