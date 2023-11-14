#include"spoor_internal.h"

#include<stdio.h>
#include<stdlib.h>
#include<raylib.h>
#include<raygui.h>
#include <string.h>

Font font_liberation;

typedef struct {
    uint32_t top;
    uint32_t right;
    uint32_t bottom;
    uint32_t left;
} UIInsets;

typedef struct {
    uint32_t x;
    uint32_t y;
} UIVector2;

#define CHILDS_ALLOC_SIZE 5

enum {
    LAYOUT_TYPE_GRID,
    LAYOUT_TYPE_STACK_VERTICAL,
    LAYOUT_TYPE_STACK_HORIZONTAL,
};

typedef struct {
    uint32_t *rows;
    uint32_t rows_count;
    uint32_t *colums;
    uint32_t colums_count;
} UILayoutGrid;

typedef struct {
} UILayoutStack;

typedef struct UIContainer {
    UIInsets margin;
    UIVector2 position;
    UIVector2 size;
    uint32_t height;
    uint32_t layout_type;
    union {
        UILayoutGrid layout_grid;
        UILayoutStack layout_stack;
    };
    void (*draw_func)(struct UIContainer *ui_container);
    struct UIContainer *childs;
    uint32_t childs_count;
} UIContainer;

UIContainer *ui_container_create(void)
{
    UIContainer *ui_container = malloc(sizeof(*ui_container));
    ui_container->margin = (UIInsets){ 0, 0, 0, 0};
    ui_container->position = (UIVector2){ 0, 0 };
    ui_container->size = (UIVector2){ 0, 0 };
    ui_container->layout_type = 0;
    ui_container->draw_func = NULL;
    ui_container->childs = NULL;
    ui_container->childs_count = 0;

    return ui_container;
}

UIContainer *ui_container_child_append(UIContainer *ui_container)
{
    UIContainer *ui_container_child = ui_container_create();

    if (ui_container->childs_count % CHILDS_ALLOC_SIZE == 0)
        ui_container->childs = realloc(ui_container->childs, (ui_container->childs_count + CHILDS_ALLOC_SIZE) * sizeof(*ui_container->childs));

    ui_container->childs[ui_container->childs_count] = *ui_container_child;
    free(ui_container_child);

    return &ui_container->childs[ui_container->childs_count++];
}

void ui_container_draw(UIContainer *para)
{
    uint32_t i;
    for (i = 0; i < para->childs_count; i++)
    {
        if (para->childs[i].draw_func != NULL)
            para->childs[i].draw_func(&para->childs[i]);
        ui_container_draw(&para->childs[i]);
    }
}

void ui_container_resize(UIContainer *para)
{
}

char ui_day_names[7][10] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday",
};

uint32_t ui_day_names_count = 7;

#if 0
void ui_page_days_draw(UIPage *ui_page)
{
    uint32_t i;
    for (i = 0; i < ui_day_names_count; i++)
    {
        uint32_t height = ui_page->height;
        uint32_t width = ui_page->width / ui_day_names_count;
        uint32_t x = ui_page->margin.left + i * width;
        uint32_t y = ui_page->margin.top;

        DrawLine(x + width, y, x + width, y + height, LIGHTGRAY);

        DrawTextEx(font_liberation, ui_day_names[i], (Vector2){ x + 5, y + 5 }, (float)font_liberation.baseSize, 2, BLACK);
        DrawTextEx(font_liberation, "12.08.2023", (Vector2){ x + 5, y + 25 }, (float)font_liberation.baseSize, 2, BLACK);

        y += 60;
        height -= 60;

        char time[6];
        uint32_t k;
        for (k = 0; k < height; k += 5)
        {
            if (k % 60 == 0)
            {
                uint32_t hours = k / 60;
                sprintf(time, "%s%d:00", (hours < 10) ?"0" :"", hours);

                DrawLine(x, y + k, x + width, y + k, BLACK);
                DrawTextEx(font_liberation, time, (Vector2){ x + 5 + 2, y + k + 2 }, (float)font_liberation.baseSize, 2, BLACK);
            }
            else if (k % 30 == 0)
            {
                DrawLine(x + 40, y + k, x + width, y + k, BLACK);
            }
            else
                DrawLine(x + 40, y + k, x + width, y + k, LIGHTGRAY);
        }
    }
}
#endif

void main_page_draw_func(UIContainer *ui_container)
{
    DrawRectangleLines(ui_container->margin.left,
            ui_container->margin.top,
            ui_container->size.x,
            ui_container->size.y,
            BLACK);

#if 0
    ui_page_days_draw(ui_page);
#endif
}

void list_page_draw_func(UIContainer *ui_container)
{
    DrawRectangleLines(ui_container->margin.left,
            ui_container->margin.top,
            ui_container->size.x,
            ui_container->size.y,
            BLACK);
}

void spoor_ui_raylib_object_show(void)
{
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "layout_name");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(60);

    UIContainer *ui_container_head = ui_container_create();
    ui_container_head->layout_type = LAYOUT_TYPE_GRID;
    UIContainer *ui_container_child = ui_container_child_append(ui_container_head);
    ui_container_child->size.x = 500;
    ui_container_child->size.y = 200;
    ui_container_child->draw_func = main_page_draw_func;
    ui_container_child = ui_container_child_append(ui_container_head);


    /* load font */
    font_liberation = LoadFontEx("LiberationMono-Regular.ttf", 32, 0, 250);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            ui_container_draw(ui_container_head);
            /* Pages */
            /*
            if (IsWindowResized())
            */
            /* Main Page */
        }
        EndDrawing();

        char c = GetCharPressed();
        switch (c)
        {
            case 'n':
                ui_day_names_count--;
                break;
            case 'r':
                ui_day_names_count++;
        }
    }

    CloseWindow();

}
