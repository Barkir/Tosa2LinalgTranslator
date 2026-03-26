// NOTE - AI-generated

#pragma once
#ifndef ANSI_COLORS_H
#define ANSI_COLORS_H

#include <string>

/* ============================================
 * ANSI ESCAPE SEQUENCES FOR C
 * ============================================ */

/* Escape character - use \x1B for portability */
#define TOSA2LINALG_ESC "\x1B"

/* Control Sequence Introducer */
#define TOSA2LINALG_CSI TOSA2LINALG_ESC "["

/* Reset all styles */
#define TOSA2LINALG_RESET TOSA2LINALG_CSI "0m"

/* ============================================
 * TEXT STYLES
 * ============================================ */
#define TOSA2LINALG_BOLD TOSA2LINALG_CSI "1m"
#define TOSA2LINALG_DIM TOSA2LINALG_CSI "2m"
#define TOSA2LINALG_ITALIC TOSA2LINALG_CSI "3m"
#define TOSA2LINALG_UNDERLINE TOSA2LINALG_CSI "4m"
#define TOSA2LINALG_BLINK TOSA2LINALG_CSI "5m"
#define TOSA2LINALG_REVERSE TOSA2LINALG_CSI "7m"
#define TOSA2LINALG_HIDDEN TOSA2LINALG_CSI "8m"
#define TOSA2LINALG_STRIKETHRU TOSA2LINALG_CSI "9m"

/* Reset specific styles */
#define RESET_BOLD TOSA2LINALG_CSI "22m"
#define RESET_DIM TOSA2LINALG_CSI "22m"
#define RESET_ITALIC TOSA2LINALG_CSI "23m"
#define RESET_UNDERLINE TOSA2LINALG_CSI "24m"
#define RESET_BLINK TOSA2LINALG_CSI "25m"
#define RESET_REVERSE TOSA2LINALG_CSI "27m"
#define RESET_HIDDEN TOSA2LINALG_CSI "28m"
#define RESET_STRIKETHRU TOSA2LINALG_CSI "29m"

/* ============================================
 * STANDARD 8 COLORS (Foreground)
 * ============================================ */
#define FG_BLACK TOSA2LINALG_CSI "30m"
#define FG_RED TOSA2LINALG_CSI "31m"
#define FG_GREEN TOSA2LINALG_CSI "32m"
#define FG_YELLOW TOSA2LINALG_CSI "33m"
#define FG_BLUE TOSA2LINALG_CSI "34m"
#define FG_MAGENTA TOSA2LINALG_CSI "35m"
#define FG_CYAN TOSA2LINALG_CSI "36m"
#define FG_WHITE TOSA2LINALG_CSI "37m"
#define FG_DEFAULT TOSA2LINALG_CSI "39m"

/* ============================================
 * STANDARD 8 COLORS (Background)
 * ============================================ */
#define BG_BLACK TOSA2LINALG_CSI "40m"
#define BG_RED TOSA2LINALG_CSI "41m"
#define BG_GREEN TOSA2LINALG_CSI "42m"
#define BG_YELLOW TOSA2LINALG_CSI "43m"
#define BG_BLUE TOSA2LINALG_CSI "44m"
#define BG_MAGENTA TOSA2LINALG_CSI "45m"
#define BG_CYAN TOSA2LINALG_CSI "46m"
#define BG_WHITE TOSA2LINALG_CSI "47m"
#define BG_DEFAULT TOSA2LINALG_CSI "49m"

/* ============================================
 * BRIGHT COLORS (aixterm - 16 color mode)
 * Foreground
 * ============================================ */
#define FG_BRIGHT_BLACK TOSA2LINALG_CSI "90m"
#define FG_BRIGHT_RED TOSA2LINALG_CSI "91m"
#define FG_BRIGHT_GREEN TOSA2LINALG_CSI "92m"
#define FG_BRIGHT_YELLOW TOSA2LINALG_CSI "93m"
#define FG_BRIGHT_BLUE TOSA2LINALG_CSI "94m"
#define FG_BRIGHT_MAGENTA TOSA2LINALG_CSI "95m"
#define FG_BRIGHT_CYAN TOSA2LINALG_CSI "96m"
#define FG_BRIGHT_WHITE TOSA2LINALG_CSI "97m"

/* Background */
#define BG_BRIGHT_BLACK TOSA2LINALG_CSI "100m"
#define BG_BRIGHT_RED TOSA2LINALG_CSI "101m"
#define BG_BRIGHT_GREEN TOSA2LINALG_CSI "102m"
#define BG_BRIGHT_YELLOW TOSA2LINALG_CSI "103m"
#define BG_BRIGHT_BLUE TOSA2LINALG_CSI "104m"
#define BG_BRIGHT_MAGENTA TOSA2LINALG_CSI "105m"
#define BG_BRIGHT_CYAN TOSA2LINALG_CSI "106m"
#define BG_BRIGHT_WHITE TOSA2LINALG_CSI "107m"

/* ============================================
 * 256 COLOR MODE MACROS
 * ============================================ */

/* Foreground: 0-255 */
#define FG_256(n) TOSA2LINALG_CSI "38;5;" #n "m"
/* Background: 0-255 */
#define BG_256(n) TOSA2LINALG_CSI "48;5;" #n "m"

/* Common 256-color shortcuts */
#define FG_ORANGE FG_256(208) /* Classic orange */
#define FG_PINK FG_256(213)   /* Hot pink */
#define FG_LIME FG_256(154)   /* Bright lime */
#define FG_PURPLE FG_256(141) /* Medium purple */
#define FG_TEAL FG_256(37)    /* Teal/cyan variant */
#define FG_GOLD FG_256(220)   /* Gold/yellow */
#define FG_CORAL FG_256(209)  /* Coral/salmon */

/* Grayscale (232-255) - 24 shades from dark to light */
#define FG_GRAY(n) TOSA2LINALG_CSI "38;5;" #n "m" /* n = 232-255 */
#define BG_GRAY(n) TOSA2LINALG_CSI "48;5;" #n "m" /* n = 232-255 */

/* ============================================
 * TRUE COLOR (24-bit RGB) MACROS
 * Format: ESC[38;2;R;G;Bm
 * ============================================ */

/* Foreground RGB - values 0-255 */
#define FG_RGB(r, g, b) TOSA2LINALG_CSI "38;2;" #r ";" #g ";" #b "m"
/* Background RGB - values 0-255 */
#define BG_RGB(r, g, b) TOSA2LINALG_CSI "48;2;" #r ";" #g ";" #b "m"

/* Common RGB colors */
#define FG_CRIMSON FG_RGB(220, 20, 60)
#define FG_FOREST FG_RGB(34, 139, 34)
#define FG_NAVY FG_RGB(0, 0, 128)
#define FG_GOLD_RGB FG_RGB(255, 215, 0)
#define FG_VIOLET FG_RGB(238, 130, 238)
#define FG_TURQUOISE FG_RGB(64, 224, 208)

/* ============================================
 * CURSOR CONTROL
 * ============================================ */
#define CURSOR_UP(n) TOSA2LINALG_CSI #n "A"
#define CURSOR_DOWN(n) TOSA2LINALG_CSI #n "B"
#define CURSOR_FORWARD(n) TOSA2LINALG_CSI #n "C"
#define CURSOR_BACK(n) TOSA2LINALG_CSI #n "D"
#define CURSOR_NEXT_LINE(n) TOSA2LINALG_CSI #n "E"
#define CURSOR_PREV_LINE(n) TOSA2LINALG_CSI #n "F"
#define CURSOR_COLUMN(n) TOSA2LINALG_CSI #n "G"

/* Position cursor (1-based) */
#define CURSOR_POS(row, col) TOSA2LINALG_CSI #row ";" #col "H"
#define CURSOR_HOME TOSA2LINALG_CSI "H"

/* Save/restore cursor position */
#define CURSOR_SAVE ESC "7"
#define CURSOR_RESTORE ESC "8"

/* Hide/show cursor */
#define CURSOR_HIDE TOSA2LINALG_CSI "?25l"
#define CURSOR_SHOW TOSA2LINALG_CSI "?25h"

/* ============================================
 * ERASE FUNCTIONS
 * ============================================ */
#define ERASE_SCREEN TOSA2LINALG_CSI "2J"
#define ERASE_LINE TOSA2LINALG_CSI "2K"
#define ERASE_TO_END TOSA2LINALG_CSI "0K"
#define ERASE_TO_BEGIN TOSA2LINALG_CSI "1K"
#define ERASE_DOWN TOSA2LINALG_CSI "0J"
#define ERASE_UP TOSA2LINALG_CSI "1J"

/* ============================================
 * HELPER MACROS FOR COMBINING STYLES
 * ============================================ */

/* Combine bold + color */
#define BOLD_RED TOSA2LINALG_CSI "1;31m"
#define BOLD_GREEN TOSA2LINALG_CSI "1;32m"
#define BOLD_YELLOW TOSA2LINALG_CSI "1;33m"
#define BOLD_BLUE TOSA2LINALG_CSI "1;34m"
#define BOLD_MAGENTA TOSA2LINALG_CSI "1;35m"
#define BOLD_CYAN TOSA2LINALG_CSI "1;36m"
#define BOLD_WHITE TOSA2LINALG_CSI "1;37m"

/* Underline + color */
#define UNDERLINE_RED TOSA2LINALG_CSI "4;31m"
#define UNDERLINE_GREEN TOSA2LINALG_CSI "4;32m"
#define UNDERLINE_YELLOW TOSA2LINALG_CSI "4;33m"
#define UNDERLINE_BLUE TOSA2LINALG_CSI "4;34m"

/* ============================================
 * EXAMPLE USAGE:
 *
 * printf(FG_RED "Error message" RESET "\n");
 * printf(BOLD_GREEN "Success!" RESET "\n");
 * printf(FG_RGB(255, 128, 0) "Orange text" RESET "\n");
 * printf(FG_256(196) "Bright red using 256 colors" RESET "\n");
 * ============================================ */

#define BIG_LINE std::cout << FG_VIOLET << "=============================================" << RESET << "\n";

#define SEP_LINE std::cout << FG_CORAL << "---------------------------------------------" << RESET << "\n";

/* MNIST ONNX Node Color Definitions - TOSA2LINALG_CONV_NODE Theme */
/* Format: 0xRRGGBB (24-bit RGB) */

/* Convolution & Core Layers */
#define TOSA2LINALG_CONV_NODE "#FF6B6B"       /* Coral Red - signature color */
#define TOSA2LINALG_CONV2D_NODE "#FF6B6B"     /* Coral Red */
#define TOSA2LINALG_RELU_NODE "#4ECDC4"       /* Turquoise */
#define TOSA2LINALG_LEAKYRELU_NODE "#45B7AA"  /* Deep Turquoise */
#define TOSA2LINALG_MAXPOOL_NODE "#96CEB4"    /* Sage Green */
#define TOSA2LINALG_AVGPOOL_NODE "#88C9A1"    /* Soft Green */
#define TOSA2LINALG_GLOBALPOOL_NODE "#7AB893" /* Forest Green */

/* Normalization & Regularization */
#define TOSA2LINALG_BATCHNORM_NODE "#FFEAA7" /* Cream Yellow */
#define TOSA2LINALG_DROPOUT_NODE "#DDA0DD"   /* Plum */
#define TOSA2LINALG_SOFTMAX_NODE "#98D8C8"   /* Mint */

/* Fully Connected / Dense */
#define TOSA2LINALG_GEMM_NODE "#6C5CE7"   /* Royal Purple */
#define TOSA2LINALG_MATMUL_NODE "#A29BFE" /* Lavender */
#define TOSA2LINALG_LINEAR_NODE "#6C5CE7" /* Royal Purple */

/* Reshape & Tensor Ops */
#define TOSA2LINALG_FLATTEN_NODE "#FDCB6E"   /* Golden Yellow */
#define TOSA2LINALG_RESHAPE_NODE "#E17055"   /* Burnt Orange */
#define TOSA2LINALG_TRANSPOSE_NODE "#FDCB6E" /* Golden Yellow */
#define TOSA2LINALG_CONCAT_NODE "#74B9FF"    /* Sky Blue */
#define TOSA2LINALG_SPLIT_NODE "#0984E3"     /* Azure Blue */

/* Input/Output */
#define TOSA2LINALG_INPUT_NODE "#00B894"  /* Teal */
#define TOSA2LINALG_OUTPUT_NODE "#E84393" /* Hot Pink */

/* Arithmetic & Activations */
#define TOSA2LINALG_ADD_NODE "#FDCB6E"     /* Golden Yellow */
#define TOSA2LINALG_MUL_NODE "#FDCB6E"     /* Golden Yellow */
#define TOSA2LINALG_SIGMOID_NODE "#A29BFE" /* Lavender */
#define TOSA2LINALG_TANH_NODE "#74B9FF"    /* Sky Blue */

/* Fallback */
#define TOSA2LINALG_DEFAULT_NODE "#B2BEC3" /* Cool Gray */
#define TOSA2LINALG_UNKNOWN_NODE "#636E72" /* Slate Gray */

#define TOSA2LINALG_TENSOR "#A45E72"

std::string getNodeColor(const std::string& nodeName);

#endif /* ANSI_COLORS_H */
