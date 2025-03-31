#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum console_scroll_dir_e {
    CONSOLE_SCROLL_UP,
    CONSOLE_SCROLL_DOWN,
} console_scroll_dir_t;

typedef enum console_cursor_e 
{
    CONSOLE_CURSOR_OFF,
    CONSOLE_CURSOR_UNDERLINE,
    CONSOLE_CURSOR_BLOCK,
} console_cursor_t;

typedef struct console_s {
    /**
     * @brief Origin of the viewport. It defines the start of the editable portion of the console.
     *
     */
    uintptr_t viewport_origin;

    /**
     * @brief Visible origin of the viewport. It can change independently from the origin during a scrollback or a
     * scrollfront.
     *
     */
    uintptr_t viewport_visible_origin;

    /**
     * @brief End of the viewport. It defines the end of the editable portion of the console.
     * 
     */
    uintptr_t viewport_end;

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

    size_t viewport_col_size;
    size_t viewport_col_nbr;
} console_t;

typedef struct console_impl_s {
    void (*init)(console_t *);
    bool (*scroll)(console_t *, console_scroll_dir_t, size_t);
    bool (*put_char)(const console_t *, unsigned char, size_t, size_t);
    void (*set_cursor_style)(console_cursor_t);
    bool (*set_cursor_pos)(const console_t *, size_t, size_t);
} console_impl_t;

#endif