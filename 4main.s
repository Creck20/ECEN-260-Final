.text
.global main
.cpu cortex-m4
.syntax unified

// Starting Code to configure B1 and LED2
// and have LED2 turn on whenever B1 is pushed

main:

	// Base address of Port A (PA) = 0x48000000
	MOV	R4, #0x0000
	MOVT	R4, #0x4800

	// Base Address of Port C (PC) = 0x48000800
	MOV	R5, #0x0800
	MOVT	R5, #0x4800

	// Address of Reset/Clock control Advanced High Performance Bus 2 Enable Register
	// RCC AHB2 ENR = 0x4002104C
	MOV	R6, #0x104C
	MOVT	R6, #0x4002

	// Enable the I/O Port clokcs with RCC AHB2 ENR (R6)
	MOV	R0, #0b00000101 // bit 0 for PA, bit 2 for PC
	STR	R0, [R6]

	// *** Configure LED2 (PA5) and LED4 (PA0) as output ***
	// Create bitmask to reset bit 11d and 1d of PA MODER (bit 10d and 0d already initializes to 1)
	MOV	R1, #0x0802 	// 0b 0000 1000 0000 0010
	MVN	R1, R1		// Negate 0x0800, 0b 1111 0111 1111 1101
	// Read PA MODER value, resest bit 11, and write back to PA MODER
	LDR	R0, [R4] 	// Load PA MODER
	AND 	R0, R1		// Reset bit 11d and 1d to 0
	STR	R0, [R4]	// Store PA MODER

	// *** Configure BUTTON1 (PC13) as input ***
	MOV	R1, #0x0000
	MOVT	R1, #0x0C00	// 0000 1100 0000 0000 0000 0000 0000 0000
	MVN	R1, R1		// 1111 0011 1111 1111 1111
	// Read PC MODER value, reset bits 26 and 27, and write back to PC MODER
	LDR	R0, [R5]	// load PC MODER
	AND	R0, R1		// Bit mask
	STR	R0, [R5]	// store PC MODER

	// *** Configure BUTTON3 (PA1) as input ***
	MOV	R1, #0x000C
	MOVT	R1, #0x0000	// 0000 0000 0000 0000 0000 0000 0000 1100
	MVN	R1, R1		// 1111 1111 1111 1111 1111 1111 1111 0011
	// Read PA MODER value, reset bits 2 and 3, and write back to PA MODER
	LDR	R0, [R4]	// Load PA MODER
	AND	R0, R1		// Bit mask
	STR	R0, [R4]	// store PA MODER

	// *** Activate Pull Up Resistor for BUTTON3 (PA1) ***
	// Note that the bits of the PUPDR are initialized to 0x00000000 -- Chat GPT
	// Create a bit mask to set bit 2d to 1
	MOV	R1, #0x0004
	MOVT	R1, #0x0000	// 0000 0000 0000 0000 0000 0000 0000 0100
	// Read PA PUPDR value, reset bit 2, and write back to PA PUPDR
	LDR	R0, [R4, 0xC]
	ORR	R0, R1
	STR	R0, [R4, 0xC]

loop:
	// *** Read Status of Button 1 (PC13) ***
	// Create bitmask to read bit 13 of PC IDR
	MOV	R1, #0x2000 	// mask = 0010 0000 0000 0000
	// Read PC IDR, reset all bits except bit 13
	LDRH	R0, [R5, #0x10]	// load PC IDR (PC base + 0x10)
	AND	R0, R1

	// Check value. Zero = button pushed. Not zero = button not pushed.
	CMP	R0, #0		// Compare with zero
	BNE	else		// if not pushed go to "else" other wise go to "if"

if:
	BL turnonLED2		// call turnonLED2 function
	B button3		// return to top of loop
else:
	BL turnoffLED2		// call turnoffLED2 function

button3:
	// *** Read Status of Button 3 (PA1) ***
	// Create bitmask to read bit 1 of PA IDR
	MOV	R1, #0x0002 	// mask = 0000 0000 0000 0010
	// Read PA IDR, reset all bits except bit 1
	LDRH	R0, [R4, #0x10]	// load PA IDR (PA base + 0x10)
	AND	R0, R1

	// Check value. Zero = button pushed. Not zero = button not pushed.
	CMP	R0, #0		// Compare with zero
	BNE	else3		// if not pushed go to "else3" other wise go to "if"

if3:
	BL turnonLED4		// call turnonLED4 function
	B loop			// return to top of loop
else3:
	BL turnoffLED4		// call turnoffLED2 function
	B loop			// return to top of loop

// This fucntion turns off LED2.
// Note that this function assumes R4 still has PA base address
turnoffLED2:
	// Create bitmask to reset bit 5
	MOV	R1, #0x0020 	// 0000 0000 0010 0000
	MVN	R1, R1		// 1111 1111 1101 1111

	// Read PA ODR value, reset bit 5, and write back to PA ODR
	LDRH 	R0, [R4, #0x14]	// load PA ODR (PA base + 0x14)
	AND	R0, R1		// reset bit 5 using mask
	STRH	R0, [R4, #0x14] // store PA ODR

	// return
	BX LR

// Turn on LED2 function, assuming that R4 still has PA base address.
turnonLED2:
	// Create bitmask to set bit 5
	MOV	R1, #0x0020 	// 0000 0000 0010 0000

	// Read PA ODR value, set bit 5, and write back to PA ODR
	LDRH 	R0, [R4, #0x14]	// load PA ODR (PA base + 0x14)
	ORR	R0, R1		// set bit 5 using mask
	STRH	R0, [R4, #0x14] // store PA ODR

	// return
	BX LR

// This fucntion turns off LED4.
// Note that this function assumes R4 still has PA base address
turnoffLED4:
	// Create bitmask to reset bit 1
	MOV	R1, #0x0001 	// 0000 0000 0000 0001
	MVN	R1, R1		// 1111 1111 1111 1110

	// Read PA ODR value, reset bit 1, and write back to PA ODR
	LDRH 	R0, [R4, #0x14]	// load PA ODR (PA base + 0x14)
	AND	R0, R1		// reset bit 0 using mask
	STRH	R0, [R4, #0x14] // store PA ODR

	// return
	BX LR

// Turn on LED4 function, assuming that R4 still has PA base address.
turnonLED4:
	// Create bitmask to set bit 0
	MOV	R1, #0x0001 	// 0000 0000 0000 0001

	// Read PA ODR value, set bit 0, and write back to PA ODR
	LDRH 	R0, [R4, #0x14]	// load PA ODR (PA base + 0x14)
	ORR	R0, R1		// set bit 0 using mask
	STRH	R0, [R4, #0x14] // store PA ODR

	// return
	BX LR

// End file
.end
