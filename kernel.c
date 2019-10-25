// Import the headers provided by GCC.
#include <stddef.h>
#include <stdint.h>

// Check we're using the correct compiler.
#if defined(__linux__)
  #error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
  #error "This code must be compiled with an x86-elf compiler"
#endif

// To display text, we write to this location.
volatile uint16_t *vga_buffer = (uint16_t *)0xb8000;
// VGA textmode buffer size.
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

// Display in the top-left.
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0f; // Black background, white foreground.

void term_init() {
  // Clear the textmode buffer.
  for (int col = 0; col < VGA_COLS; col++)
    for (int row = 0; row < VGA_ROWS; row++) {
      const size_t index = (VGA_COLS * row) + col;
      // Entries in the VGA buffer take the binary form BBBBFFFFCCCCCCCC where
      // B is the background color
      // F is the foreground color
      // C is the ASCII char
      vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
    }
}

void term_putc(char c) {
  switch (c) {
  case '\n': {
    term_col = 0;
    term_row++;
    break;
  }

  default: {
    const size_t index = (VGA_COLS * term_row) + term_col;
    vga_buffer[index] = ((uint16_t)term_color << 8) | c;
    term_col++;
    break;
  }
  }
  if (term_col >= VGA_COLS) {
    term_col = 0;
    term_row++;
  }
  if (term_row >= VGA_ROWS) {
    term_col = 0;
    term_row = 0;
  }
}

void term_print(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++)
    term_putc(str[i]);
}

void kernel_main() {
  term_init();
  term_print("Welcome to ghOSt v0.1.0\n");
  term_print("Hello world!\n");
}
