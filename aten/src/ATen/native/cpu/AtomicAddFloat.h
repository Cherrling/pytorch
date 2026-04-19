#ifndef ATOMIC_ADD_FLOAT
#define ATOMIC_ADD_FLOAT

#if (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__))
#include <ATen/native/cpu/Intrinsics.h>
#else
#define _mm_pause()
#endif

#include <atomic>

static inline void cpu_atomic_add_float(float* dst, float fvalue)
{
  std::atomic_ref<float> atomic_dst(*dst);
  float old_value = atomic_dst.load();
  float new_value = old_value + fvalue;
  while (!atomic_dst.compare_exchange_weak(old_value, new_value)) {
#ifdef __aarch64__
    __asm__ __volatile__("yield;" : : : "memory");
#else
    _mm_pause();
#endif
    new_value = old_value + fvalue;
  }
}

#endif
