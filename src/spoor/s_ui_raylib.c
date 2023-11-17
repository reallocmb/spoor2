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

#define LAYOUT_TYPE_DEFAULT 0b0
#define LAYOUT_TYPE_GRID 0b1
#define LAYOUT_TYPE_GRID_CHILD 0b10
#define LAYOUT_TYPE_STACK_VERTICAL 0b100
#define LAYOUT_TYPE_STACK_VERTICAL_CHILD 0b1000
#define LAYOUT_TYPE_STACK_HORIZONTAL 0b10000
#define LAYOUT_TYPE_STACK_HORIZONTAL_CHILD 0b100000

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
    uint16_t layout_type;
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
    ui_container->size = (UIVector2){ 0, 0};
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

void ui_container_grid_set(UIContainer *ui_container)
{
}

void ui_container_draw(UIContainer *ui_container)
{
    uint32_t i;
    for (i = 0; i < ui_container->childs_count; i++)
    {
        if (ui_container->childs[i].draw_func != NULL)
            ui_container->childs[i].draw_func(&ui_container->childs[i]);
        ui_container_draw(&ui_container->childs[i]);
    }
}

void ui_container_child_resize_update(UIContainer *para)
{
    uint32_t i;
    for (i = 0; i < para->childs_count; i++)
    {
        if (para->childs[i].layout_type | LAYOUT_TYPE_STACK_VERTICAL_CHILD)
        {
            para->childs[i].size.y = para->size.y / para->childs_count;
            para->childs[i].position.y = para->size.y / para->childs_count * i;
        }
    }
}

void ui_container_resize_update(UIContainer *para)
{
    para->size = (UIVector2){ GetScreenWidth(), GetScreenHeight() };
    uint32_t i;
    for (i = 0; i < para->childs_count; i++)
    {
        para->childs[i].size = para->size;
        if (para->childs[i].layout_type | LAYOUT_TYPE_STACK_VERTICAL_CHILD)
        {
            para->childs[i].size.y = para->size.y / para->childs_count;
            para->childs[i].position.y = para->size.y / para->childs_count * i;
        }

        if (para->childs[i].layout_type | LAYOUT_TYPE_STACK_HORIZONTAL_CHILD)
        {
            para->childs[i].size.x = para->size.x / para->childs_count;
        }
    }
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

void container_draw_func_test(UIContainer *ui_container)
{
    uint32_t margin = 2;
    DrawRectangleLines(
            ui_container->position.x + margin,
            ui_container->position.y + margin,
            ui_container->size.x - 2 * margin,
            ui_container->size.y - 2 * margin,
            BLACK);
}

void list_page_draw_func(UIContainer *ui_container)
{
    DrawRectangleLines(ui_container->margin.left,
            ui_container->margin.top,
            ui_container->size.x,
            ui_container->size.y,
            BLACK);
}

void test(UIContainer *ptr)
{
    UIContainer *ui_container_child = ui_container_child_append(ptr);
    ui_container_child->draw_func = container_draw_func_test;
    ui_container_child->layout_type |= LAYOUT_TYPE_STACK_VERTICAL_CHILD;
    ui_container_resize_update(ptr);
}

void spoor_ui_raylib_object_show(void)
{
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "layout_name");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(60);

    UIContainer *ui_container_window = ui_container_create();
    ui_container_window->layout_type |= LAYOUT_TYPE_STACK_VERTICAL;
    UIContainer *ui_container_child = ui_container_child_append(ui_container_window);
    ui_container_child->draw_func = container_draw_func_test;
    ui_container_child->layout_type |= LAYOUT_TYPE_STACK_VERTICAL_CHILD;

    ui_container_resize_update(ui_container_window);


    /* load font */
    font_liberation = LoadFontEx("LiberationMono-Regular.ttf", 32, 0, 250);

    _Bool leader = 0;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            if (IsWindowResized())
                ui_container_resize_update(ui_container_window);

            ui_container_draw(ui_container_window);
        }
        EndDrawing();

        char c = GetCharPressed();
        int key = GetKeyPressed();
        printf("key: %d\nchar: %c\n");
        if (leader)
        {
            if (c == 'i')
            {
                test(ui_container_window);
                leader = 0;
            }

            if (key == KEY_S)
            {
                test(ui_container_window);
                leader = 0;
            }
         
        }

        switch (key)
        {
            case KEY_J:
                ui_day_names_count--;
                break;
            case KEY_K:
                ui_day_names_count++;
                break;
            case KEY_SPACE:
                leader = 1;
                printf("leader\n");
                break;
        }

        switch (c)
        {
            case 'n':
                ui_day_names_count--;
                break;
            case 'r':
                ui_day_names_count++;
                break;
            case ' ':
                leader = 1;
                printf("test\n");
                break;
        }
    }

    CloseWindow();

}
