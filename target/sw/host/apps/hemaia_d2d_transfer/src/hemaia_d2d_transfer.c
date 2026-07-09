#include "../data/data.h"
#include "host.h"

int main() {
    uintptr_t address_prefix = (uintptr_t)get_current_chip_baseaddress();

    init_uart(address_prefix, 32, 1);
    asm volatile("fence" : : : "memory");
    printf("Hello world from HeMAiA chip %d! \r\n", get_current_chip_id());

    // The pointer to the communication buffer
    volatile comm_buffer_t* comm_buffer_ptr = (comm_buffer_t*)0;
    comm_buffer_ptr = (comm_buffer_t*)chiplet_addr_transform(((uint64_t)&__narrow_spm_start));
    // comm_buffer_ptr = (comm_buffer_t*)chiplet_addr_transform(((uint64_t)&__wide_spm_start));

    // Initialize the communication buffer
    initialize_comm_buffer((comm_buffer_t*)comm_buffer_ptr);
    asm volatile("fence" ::: "memory");

    // Data destination in spm_wide 0x8000_0000 after original data 
    uint8_t* data_dest = (uint8_t*)0x80010000;
    printf("Data source address: %p, Data destination address: %p, Data size: %d bytes\r\n",
        (void *)data, (void *)data_dest, data_size);

    printf("Comm buffer: 0x%08x \r\n", comm_buffer_ptr);

    if (get_current_chip_id() == 0) {
        printf("DMA copy on chip 0 started \r\n");
        sys_dma_blk_memcpy(
            get_current_chip_id(), 
            chiplet_addr_transform_full(0x01, (uintptr_t)data_dest),
            (uintptr_t)data, 
            data_size);
        // asm volatile("fence" : : : "memory");
        printf("DMA copy on chip 0 finished \r\n");
    }

    printf("Chip %d barrier start. \r\n", get_current_chip_id());
    chip_barrier(comm_buffer_ptr, 0x00, 0x01, 1);
    printf("Chip %d barrier finished. \r\n", get_current_chip_id());

    if (get_current_chip_id() == 1) {
        uint8_t* data_dest_chip = (uint8_t*)chiplet_addr_transform((uintptr_t)data_dest);
        printf("Checking data correctness... on chip 1\r\n");
        for (uint32_t i = 0; i < data_size; i++) {
            if (data[i] != data_dest_chip[i]) {
                printf("Data mismatch at index %d: expected %d, got %d\n", i,
                    data[i], data_dest_chip[i]);
                return -1;
            }
        }
        printf("Data correctness check passed on chip 1\r\n");
    }

    printf("Finished on chip %d\r\n", get_current_chip_id());
    return 0;
}
