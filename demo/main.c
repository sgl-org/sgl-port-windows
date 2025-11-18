#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <sgl.h>
#include <sgl_font.h>


typedef struct sgl_port_sdl2 sgl_port_sdl2_t;


sgl_port_sdl2_t* sgl_port_sdl2_init(void);
size_t sgl_port_sdl2_get_frame_count(sgl_port_sdl2_t* sdl2_dev);
void sgl_port_sdl2_increase_frame_count(sgl_port_sdl2_t* sdl2_dev);
void sgl_port_sdl2_deinit(sgl_port_sdl2_t* sdl2_dev);
void flush_window_callback(void *param);


extern const unsigned char gImage_btn[230400];
const sgl_pixmap_t keyboard_pixmap = {
    .width = 64,
    .height = 64,
    .bitmap = gImage_btn,
};

extern const unsigned char page1_bitmap[425600];
const sgl_pixmap_t page1_pixmap = {
    .width = 266,
    .height = 400,
    .bitmap = page1_bitmap,
};


extern const unsigned char gImage_test[1536000];
const sgl_pixmap_t test_pixmap = {
    .width = 800,
    .height = 480,
    .bitmap = gImage_test,
};

#define  TEST_PAGE_MAX      (10)
sgl_obj_t *test_page[TEST_PAGE_MAX];
const char *test_label[TEST_PAGE_MAX] = {
    "This is 1 page",
    "This is 2 page",
    "This is 3 page",
    "This is 4 page",
    "This is 5 page",
    "This is 6 page",
    "This is 7 page",
    "This is 8 page",
    "This is 9 page",
    "This is 10 page",
};

int32_t select = 0;
bool down_up = 0;


void test_scroll_page(int direction)
{
    static int index = 0;
    if (direction == 1) {
        if (index > ( 1 - TEST_PAGE_MAX)) {
            index --;
        }
    }
    else if (direction == -1) {
        if (index < 0 ) {
            index ++;
        }
    }

    for (int i = 0; i < TEST_PAGE_MAX; i++) {
        sgl_obj_set_pos_y(test_page[i], (i + index) *  SGL_SCREEN_HEIGHT);
    }
}


void test_click_cb(sgl_event_t *event)
{
    if (event->type == SGL_EVENT_PRESSED) {
        for (int i = 0; i < TEST_PAGE_MAX; i++) {
            sgl_obj_size_zoom(test_page[i], -10);
        }
    }
    else if (event->type == SGL_EVENT_RELEASED) {
        int diff = sgl_obj_get_pos_y(test_page[0]) % SGL_SCREEN_HEIGHT;
        SGL_LOG_INFO("Clicked %d", diff);
        for (int i = 0; i < TEST_PAGE_MAX; i++) {
            sgl_obj_size_zoom(test_page[i], 10);
        }
        if (down_up) {
            if (sgl_abs(diff) < SGL_SCREEN_HEIGHT / 2) {
                test_scroll_page(-1);
            }
            else {
                test_scroll_page(0);
            }
        }
        else {
            if (sgl_abs(diff) > SGL_SCREEN_HEIGHT / 2) {
                test_scroll_page(1);
            }
            else {
                test_scroll_page(0);
            }
        }
    }
}


void page_cb(sgl_event_t *event)
{
    int diff = sgl_obj_get_pos_y(test_page[0]) % SGL_SCREEN_HEIGHT;
    SGL_LOG_INFO("Clicked %d", diff);
    if (event->type == SGL_EVENT_MOVE_UP) {
        down_up = false;
        for (int i = 0; i < TEST_PAGE_MAX; i++) {
            sgl_obj_set_pos_y(test_page[i], sgl_obj_get_pos_y(test_page[i]) + -event->distance);
        }
    }
    else if (event->type == SGL_EVENT_MOVE_DOWN) {
        down_up = true;
        for (int i = 0; i < TEST_PAGE_MAX; i++) {
            sgl_obj_set_pos_y(test_page[i], sgl_obj_get_pos_y(test_page[i]) + +event->distance);
        }
    }
}

int main(int argc, char *argv[])
{
    SGL_UNUSED(argc);
    SGL_UNUSED(argv);

    int quit = 0;
    SDL_Event MouseEvent;
    sgl_port_sdl2_t* sdl2_dev = NULL;

    sdl2_dev = sgl_port_sdl2_init();
    if(sdl2_dev == NULL) {
        return -1;
    }

    sgl_page_set_pixmap(sgl_screen_act(), &test_pixmap);
    sgl_obj_set_event_cb(sgl_screen_act(), page_cb, 0);
    sgl_obj_set_movable(sgl_screen_act());

    int pos_y = 0;

    for (int i = 0; i < TEST_PAGE_MAX; i++) {
        test_page[i] = sgl_rect_create(NULL);
        sgl_obj_set_pos(test_page[i], 0, pos_y);
        sgl_obj_set_size(test_page[i], SGL_SCREEN_WIDTH, SGL_SCREEN_HEIGHT);
        sgl_rect_set_radius(test_page[i], 0);
        sgl_rect_set_border_width(test_page[i], 0);
        sgl_rect_set_color(test_page[i], sgl_rgb(sgl_rand() % 255, sgl_rand() % 255, sgl_rand() % 255));
        sgl_rect_set_alpha(test_page[i], 200);
        sgl_rect_set_pixmap(test_page[i], &page1_pixmap);
        sgl_obj_set_clickable(test_page[i]);
        sgl_obj_set_event_cb(test_page[i], test_click_cb, 0);

        sgl_obj_t *label = sgl_label_create(test_page[i]);
        sgl_label_set_font(label, &song23);
        sgl_label_set_text(label, test_label[i]);

        pos_y += SGL_SCREEN_HEIGHT;
    }

    while (!quit) {
        SDL_PollEvent(&MouseEvent);
        switch (MouseEvent.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        }

        sgl_task_handle();

        sgl_port_sdl2_increase_frame_count(sdl2_dev);
    }

    sgl_port_sdl2_deinit(sdl2_dev);

    return 0;
}
