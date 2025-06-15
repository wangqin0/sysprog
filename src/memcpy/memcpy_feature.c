#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    printf("DEMONSTRATING FEATURES AND QUIRKS OF MEMCPY\n");
    printf("===========================================\n\n");
    
    // 1. Basic usage of memcpy
    char src[] = "Hello, memcpy!";
    char dest[20];
    
    memcpy(dest, src, strlen(src) + 1); // +1 to include null terminator
    printf("1. Basic usage:\n");
    printf("   Source: %s\n", src);
    printf("   Destination: %s\n\n", dest);
    
    // 2. Overlapping memory regions - a common quirk
    char buffer[20] = "abcdefghijklmnopqrs";
    printf("2. Overlapping memory regions:\n");
    printf("   Before overlapping memcpy: %s\n", buffer);
    
    // This is problematic - memcpy doesn't handle overlapping memory regions properly
    // The behavior is undefined by the C standard
    memcpy(buffer, buffer + 5, 10);
    printf("   After overlapping memcpy: %s\n", buffer);
    printf("   (Note: Result may vary across different compilers/runs)\n");
    
    // Reset and use memmove for comparison
    strcpy(buffer, "abcdefghijklmnopqrs");
    printf("   Before memmove: %s\n", buffer);
    memmove(buffer, buffer + 5, 10);
    printf("   After memmove: %s\n", buffer);
    printf("   (memmove handles overlapping memory regions correctly)\n\n");
    
    // 3. Structure copying
    typedef struct {
        int id;
        char name[20];
        double value;
    } Record;
    
    Record record1 = {1, "Original", 123.45};
    Record record2;
    
    memcpy(&record2, &record1, sizeof(Record));
    
    printf("3. Structure copying:\n");
    printf("   Original - ID: %d, Name: %s, Value: %.2f\n", 
           record1.id, record1.name, record1.value);
    printf("   Copy - ID: %d, Name: %s, Value: %.2f\n\n", 
           record2.id, record2.name, record2.value);
    
    // 4. Alignment issues demonstration
    typedef struct {
        char c;   // 1 byte
        int i;    // 4 bytes, but may have padding before it
        char d;   // 1 byte
    } Unaligned;
    
    printf("4. Structure alignment:\n");
    printf("   Size of char + int + char: %zu bytes\n", sizeof(char) + sizeof(int) + sizeof(char));
    printf("   Size of Unaligned struct: %zu bytes\n", sizeof(Unaligned));
    printf("   The difference is due to padding bytes added for alignment\n");
    printf("   (memcpy copies these padding bytes as well)\n\n");
    
    // 5. Copying between different types
    int int_array[5] = {1, 2, 3, 4, 5};
    double double_array[5];
    
    // This copies the raw bytes, not the values as interpreted as doubles
    memcpy(double_array, int_array, sizeof(int_array));
    
    printf("5. Copying between different types:\n");
    printf("   Original ints: %d, %d, %d, %d, %d\n", 
           int_array[0], int_array[1], int_array[2], int_array[3], int_array[4]);
    printf("   After memcpy to doubles: %f, %f, %f, %f, %f\n", 
           double_array[0], double_array[1], double_array[2], double_array[3], double_array[4]);
    printf("   (memcpy just copies raw bytes, not semantically converted values)\n\n");
    
    // 6. Return value of memcpy
    char ret_src[] = "Return value test";
    char ret_dest[20];
    void *ret_val = memcpy(ret_dest, ret_src, strlen(ret_src) + 1);
    
    printf("6. Return value of memcpy:\n");
    printf("   Source: %s\n", ret_src);
    printf("   Destination: %s\n", ret_dest);
    printf("   Are return value and destination the same pointer? %s\n\n", 
           (ret_val == ret_dest) ? "Yes" : "No");
    
    // 7. memcpy doesn't check boundaries
    char small_buffer[5];
    char large_data[] = "This string is too long for the buffer";
    
    printf("7. Boundary checking:\n");
    printf("   memcpy doesn't check boundaries. The following would cause a buffer overflow:\n");
    printf("   memcpy(small_buffer, large_data, sizeof(large_data));\n");
    printf("   Always ensure your destination has enough space!\n\n");
    // Not executing this code as it would cause undefined behavior
    
    // 8. Performance test
    const int size = 10000000;
    char *large_src = malloc(size);
    char *large_dest = malloc(size);
    
    if (!large_src || !large_dest) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Fill source buffer
    memset(large_src, 'A', size);
    
    clock_t start = clock();
    memcpy(large_dest, large_src, size);
    clock_t end = clock();
    
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("8. Performance:\n");
    printf("   Time to copy %d bytes: %.6f seconds\n", size, cpu_time_used);
    printf("   (memcpy is highly optimized and often uses SIMD instructions)\n\n");
    
    free(large_src);
    free(large_dest);
    
    // 9. NULL pointer behavior
    printf("9. NULL pointer behavior:\n");
    printf("   memcpy(NULL, src, 10) or memcpy(dest, NULL, 10) causes undefined behavior\n");
    printf("   However, memcpy(dest, src, 0) with NULL pointer is valid if size is 0\n\n");
    
    // 10. Zero-length copy
    char *null_ptr = NULL;
    memcpy(dest, null_ptr, 0); // Valid because size is 0
    printf("10. Zero-length copy with NULL pointer:\n");
    printf("    memcpy(dest, NULL, 0) completed without error\n");
    printf("    (This is valid per the C standard)\n\n");
    
    return 0;
}
