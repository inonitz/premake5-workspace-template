#include "example_utils.hpp"
#include <hip/hip_runtime.h>
#include <iostream>

// i32 compute_shader_render_to_screen()
// {
//     printf("HELLO\n");
//     markstr("AWC2 init begin"); /* Init awc2 */
//     markstr("compute_shader_render_to_screen end  ");
//     return 1;
// }


__device__ unsigned int get_thread_idx()
{
    return threadIdx.x;
}

__host__ void print_hello_host()
{
    std::cout << "Hello world from host!" << std::endl;
}

__device__ __host__ void print_hello()
{
    printf("Hello world from device or host!\n");
}

__global__ void helloworld_kernel()
{
    unsigned int thread_idx = get_thread_idx();
    unsigned int block_idx = blockIdx.x;

    print_hello();

    // Only printf is supported for printing from device code.
    printf("Hello world from device kernel block %u thread %u!\n", block_idx, thread_idx);
}

int compute_shader_render_to_screen()
{
    print_hello_host();
    print_hello();
    helloworld_kernel<<<dim3(2), // 3D grid specifying number of blocks to launch: (2, 1, 1)
                        dim3(2), // 3D grid specifying number of threads to launch: (2, 1, 1)
                        0, // number of bytes of additional shared memory to allocate
                        hipStreamDefault // stream where the kernel should execute: default stream
                        >>>();
    HIP_CHECK(hipDeviceSynchronize());


    return 1;
}
