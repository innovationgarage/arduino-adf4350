#include <sys/types.h>
#include <sys/param.h>
#include <stdint.h>

#define REF_CLK 10.0e6
#define PFD_FREQ 10.0e6

// Make sure we can recognize the endianness.
#if (!defined(BYTE_ORDER) || !defined(BIG_ENDIAN) || !defined(LITTLE_ENDIAN))
#error "Unknown platform endianness!"
#endif

#if BYTE_ORDER == LITTLE_ENDIAN

uint16_t htons(uint16_t v) {
  return (v >> 8) | (v << 8);
}
uint32_t htonl(uint32_t v) {
  return htons(v >> 16) | (htons((uint16_t) v) << 16);
}

#else

uint16_t htons(uint16_t v) {
  return v;
}
uint32_t htonl(uint32_t v) {
  return v;
}

#endif

uint16_t ntohs(uint16_t v) {
  return htons(v);
}
uint32_t ntohl(uint32_t v) {
  return htonl(v);
}

