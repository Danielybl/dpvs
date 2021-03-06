#ifndef __UOA_EXTRA_H__
#define __UOA_EXTRA_H__

#ifdef UOA_NEED_EXTRA
#include <linux/ipv6.h>
#endif

union inet_addr {
    struct in_addr      in;
    struct in6_addr     in6;
};

#ifdef UOA_NEED_EXTRA
static inline uint32_t inet_addr_fold(int af, const union inet_addr *addr)
{
    uint32_t addr_fold = 0;

    if (af == AF_INET) {
        addr_fold = addr->in.s_addr;
    } else if (af == AF_INET6) {
        addr_fold = addr->in6.s6_addr32[0] ^ addr->in6.s6_addr32[1] ^
                    addr->in6.s6_addr32[2] ^ addr->in6.s6_addr32[3];
    } else {
        return 0;
    }

    return addr_fold;
}

static inline bool inet_addr_equal(int af, const union inet_addr *a1,
                     const union inet_addr *a2)
{
    switch (af) {
        case AF_INET:
            return a1->in.s_addr == a2->in.s_addr;
        case AF_INET6:
            return memcmp(a1->in6.s6_addr, a2->in6.s6_addr, 16) == 0;
        default:
            return memcmp(a1, a2, sizeof(union inet_addr)) == 0;
    }
}

#define IN6_ARE_ADDR_EQUAL(a,b) \
    ((((const uint32_t *) (a))[0] == ((const uint32_t *) (b))[0])     \
     && (((const uint32_t *) (a))[1] == ((const uint32_t *) (b))[1])  \
     && (((const uint32_t *) (a))[2] == ((const uint32_t *) (b))[2])  \
     && (((const uint32_t *) (a))[3] == ((const uint32_t *) (b))[3]))

static inline bool inet_is_addr_any(int af, const union inet_addr *addr)
{
    switch (af) {
        case AF_INET:
            return addr->in.s_addr == htonl(INADDR_ANY);
        case AF_INET6:
            return IN6_ARE_ADDR_EQUAL(&addr->in6, &in6addr_any);
        default:
            return false;
    }
}
#endif

#endif /* ifndef __UOA_EXTRA_H_ */
