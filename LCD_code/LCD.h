#include "keypad.h"
#include <util/delay.h>

// 4 Rows × 3 Columns Keypad Layout
// Connect Rows to Px0-Px3, Columns to Px4-Px6
static const char KEYPAD_MAP[4][3] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}
};

char KEYPAD_GetKey(void) {
	// Rows: Px0–Px3 → output
	// Columns: Px4–Px6 → input with pull-up
	KEYPAD_DDR = 0x0F;       // Lower 4 bits output (rows), upper 3 input (cols)
	KEYPAD_PORT = 0x7F;      // Enable pull-ups on cols (Px4-Px6), set rows HIGH

	for (uint8_t row = 0; row < 4; row++) {
		// Ground one row at a time
		KEYPAD_PORT = ~(1 << row) | 0x70;  // 0x70 = 0111 0000 (cols pulled-up)

		_delay_ms(5); // debounce

		uint8_t col_read = KEYPAD_PIN >> 4; // Read Px4–Px6

		for (uint8_t col = 0; col < 3; col++) {
			if (!(col_read & (1 << col))) {
				while (!(KEYPAD_PIN & (1 << (col + 4)))); // Wait for release
				_delay_ms(10); // debounce release
				return KEYPAD_MAP[row][col];
			}
		}
	}

	return 0; // No key pressed
}
