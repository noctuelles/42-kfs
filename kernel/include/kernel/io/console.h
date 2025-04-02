#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NBR_AVAILABLE_CONSOLE 2

typedef enum console_scroll_dir_e {
    CONSOLE_SCROLL_UP,
    CONSOLE_SCROLL_DOWN,
} console_scroll_dir_t;

typedef enum console_cursor_e {
    CONSOLE_CURSOR_OFF,
    CONSOLE_CURSOR_UNDERLINE,
    CONSOLE_CURSOR_HALF_BLOCK,
    CONSOLE_CURSOR_BLOCK,
} console_cursor_t;

typedef enum console_color_e {
    CONSOLE_COLOR_BLACK         = 0,
    CONSOLE_COLOR_BLUE          = 1,
    CONSOLE_COLOR_GREEN         = 2,
    CONSOLE_COLOR_CYAN          = 3,
    CONSOLE_COLOR_RED           = 4,
    CONSOLE_COLOR_MAGENTA       = 5,
    CONSOLE_COLOR_BROWN         = 6,
    CONSOLE_COLOR_LIGHT_GREY    = 7,
    CONSOLE_COLOR_DARK_GREY     = 8,
    CONSOLE_COLOR_LIGHT_BLUE    = 9,
    CONSOLE_COLOR_LIGHT_GREEN   = 10,
    CONSOLE_COLOR_LIGHT_CYAN    = 11,
    CONSOLE_COLOR_LIGHT_RED     = 12,
    CONSOLE_COLOR_LIGHT_MAGENTA = 13,
    CONSOLE_COLOR_YELLOW        = 14,
    CONSOLE_COLOR_WHITE         = 15,
} console_color_t;

typedef struct console_s {
    /**
     * @brief Origin of the viewport. It defines the start of the editable portion of the console.
     *
     */
    uintptr_t viewport_origin;

    /**
     * @brief Visible origin of the viewport.
     *
     */
    uintptr_t viewport_visible_origin;

    /**
     * @brief End of the viewport. It defines the end of the editable portion of the console.
     *
     */
    uintptr_t viewport_end;

    /**
     * @brief Visible end of the viewport.
     * 
     */
    uintptr_t viewport_visible_end;

    /**
     * @brief Size in bytes of a single row.
     *
     */
    size_t viewport_row_size;

    /**
     * @brief Number of rows in the viewport.
     *
     */
    size_t viewport_row_nbr;

    /**
     * @brief Size of the viewport in bytes. It is defined as viewport_row_size * viewport_row_nbr.
     *
     */
    size_t viewport_size;

    /**
     * @brief Size in bytes of one column.
     * 
     */
    size_t viewport_col_size;

    /**
     * @brief Number of columns in one row.
     * 
     */
    size_t viewport_col_nbr;

    /**
     * @brief Buffer that is used to save the VRAM.
     * 
     */
    uintptr_t buffer;

    /**
     * @brief Size of the buffer.
     * 
     */
    size_t    buffer_size;

    /**
     * @brief Attribute byte.
     * 
     */
    uint8_t attr;
} console_t;

typedef struct console_impl_s {
    void (*init)(console_t *);
    void (*load)(const console_t *);
    void (*save)(console_t *);
    void (*set_attr)(console_t *, console_color_t, console_color_t);
    bool (*scroll)(console_t *, console_scroll_dir_t);
    bool (*softscroll)(console_t *, console_scroll_dir_t, size_t);
    bool (*put_char)(console_t *, unsigned char, size_t, size_t);
    void (*restore)(console_t *);
    void (*set_cursor_style)(console_cursor_t);
    bool (*set_cursor_pos)(const console_t *, size_t, size_t);

    size_t max_console_nbr;
} console_impl_t;

#endif