#ifndef _KERNEL_SEGMENTATION_H
#define _KERNEL_SEGMENTATION_H

#include <stdint.h>

enum SegmentMask {
	// first DW
	LIMIT_FIRST_DW_MASK       = 0x0000FFFF,
	BASE_FIRST_DW_MASK        = 0xFFFF0000,

	// sceond DW
	BASE_SECOND_DW_LOWER_MASK = 0x000000FF,
	FLAG_MASK                 = 0x00F0FF00,
	LIMIT_SECOND_DW_MASK      = 0x000F0000,
	BASE_SECOND_DW_UPPER_MASK = 0xFF000000,
};

// all are shift left, except when specified
enum SegmentOffset {
	BASE_FIRST_DW_OFFSET   = 0x10,
	BASE_SECOND_DW_OFFSET  = 0x10,  // shift right

	SEGMENT_TYPE_OFFSET    = 0x08,
	DESCRIPTOR_TYPE_OFFSET = 0x0C,
	PRIVILEGE_LEVEL_OFFSET = 0x0D,
	PRESENT_OFFSET         = 0x0F,
	OPERATION_SIZE_OFFSET  = 0x16,
	GRANULARITY_OFFSET     = 0x17,
};

enum SegmentDataType {
	READ_ONLY                       = 0x00,
	READ_ONLY_ACCESSED              = 0x01,
	READ_WRITE                      = 0x02,
	READ_WRITE_ACCESSED             = 0x03,
	READ_ONLY_EXPAND_DOWN           = 0x04,
	READ_ONLY_EXPAND_DOWN_ACCESSED  = 0x05,
	READ_WRITE_EXPAND_DOWN          = 0x06,
	READ_WRITE_EXPAND_DOWN_ACCESSED = 0x07,
};

enum SegmentCodeType {
	EXECUTE_ONLY                     = 0x08,
	EXECUTE_ONLY_ACCESSED            = 0x09,
	EXECUTE_READ                     = 0x0A,
	EXEUCTE_READ_ACCESSED            = 0x0B,
	EXECUTE_ONLY_CONFORMING          = 0x0C,
	EXECUTE_ONLY_CONFORMING_ACCESSED = 0x0D,
	EXECUTE_READ_CONFORMING          = 0x0E,
	EXECUTE_READ_CONFORMING_ACCESSED = 0x0F,
};

enum DescriptorType {
	SYSTEM    = 0x00,
	CODE_DATA = 0x01,
};

enum PrivilegeLevel {
	KERNEL               = 0x00,
	DEVICE_DRIVERS_HIGH  = 0x01,
	DEVICE_DRIVERS_LOW   = 0x02,
	APPLICATIONS         = 0x03,
};

enum Present {
	NOT_PRESENT = 0x00,
	IN_MEMORY   = 0x01,
};

enum OperationSize {
	BITS_16 = 0x00,
	BITS_32 = 0x01,
};

enum Granularity {
	BYTE     = 0x00,  // 1B to 1MB
	KILOBYTE = 0x01,  // 4KB to 4GB
};

struct SegmentDescriptor {
	/* Defines the location of byte 0 of the segment within the 4GB linear address space */
	uint32_t base;

	/* Defines the size of the segment. See the granularity flag for more information */
	uint32_t limit;

	/* Segment type defines the access control for this specific segment.
	 * Use `SegmentDataType` and `SegmentCodeType` enums for ease.
	 */
	uint16_t segment_type;

	/* Descriptor type specifies the kinds of access that can be made to the segment and the direction of growth
	 *   > 0 - system
	 *   > 1 - code/data
	 */
	uint16_t descriptor_type;

	/* Indicates the "ring" at which this segment is defining. Use `PrivilegeLevel` enum for ease.
	 *   > 0 - Kernel
	 *   > 1 - Device drivers
	 *   > 2 - Device drivers
	 *   > 3 - Applications
	 */
	uint16_t privilege_level;

	/* Indicates whether the segment is present in memory (1) or not present (0).
	 * This will almost always be 0.
	 */
	uint16_t present;

	/* Indicates the default length for effective addresses and operands referenced by instructions in the segment.
	 *   > 0 - 16b
	 *   > 1 - 32b
	 */
	uint16_t operation_size;

	/* Determines the granularity of the "limit" value
	 *   > 0 - 1B to 1MB
	 *   > 1 - 4KB to 4GB
	 */
	uint16_t granularity;
};

void initialize_segments();
void create_descriptor(SegmentDescriptor);
uint32_t create_flags(SegmentDescriptor);

#endif

