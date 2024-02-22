#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#else
#error "Double precision floating point not supported by OpenCL implementation."
#endif

#define kEmpty          0xffffffff

typedef struct {
    float4 centre;
    float radius;
} Sphere_t;

typedef struct {
    float4 o;
    float4 d;
} Ray_t;

typedef struct {
    float4 p;
    float4 n;
    float t;
} Isect_t;
