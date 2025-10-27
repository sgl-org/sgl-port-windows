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

extern const unsigned char gImage_test[1440000];
const sgl_pixmap_t test_pixmap = {
    .width = 800,
    .height = 480,
    .bitmap = gImage_test,
};

sgl_obj_t *rect43 = NULL;

void msgbox_callback(sgl_event_t *event)
{
    if(event->type == SGL_EVENT_PRESSED) {
        //sgl_obj_set_destroyed(rect43);
    }
}

static const uint8_t default_icon_pixmap[] =
{
0x00,0x00,0x00,0x03,0x33,0x33,0x33,0x00,0x00,0x00,0x00,  //.....................
0x00,0x00,0x00,0xae,0xff,0xff,0xff,0xc0,0x00,0x00,0x00,  //......%@@@@@@@%......
0x00,0x00,0x05,0xef,0xec,0xcc,0xcf,0xe8,0x00,0x00,0x00,  //.....+@@@%%%%@@+.....
0x00,0x00,0x0a,0xfe,0x00,0x00,0x0c,0xfe,0x00,0x00,0x00,  //.....%@@.....%@@.....
0x56,0x66,0x6e,0xfc,0x66,0x66,0x6a,0xee,0x86,0x66,0x60,  //+++++@@%+++++%@@+++++
0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0,  //@@@@@@@@@@@@@@@@@@@@@
0xac,0xee,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xae,0xfc,0xa0,  //%%@@%%%%%%%%%%%%%@@%%
0x05,0xee,0x30,0x00,0x00,0x00,0x00,0x00,0x0e,0xfa,0x00,  //.+@@.............@@%.
0x05,0xee,0x30,0x00,0x00,0x00,0x00,0x00,0x0e,0xfa,0x00,  //.+@@.............@@%.
0x05,0xee,0x35,0xaa,0x05,0xa8,0x08,0xa5,0x0e,0xfa,0x00,  //.+@@.+%%.+%+.+%+.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x38,0xee,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x00,  //.+@@.+@@.%@%.%@%.@@%.
0x05,0xee,0x30,0x30,0x00,0x30,0x00,0x30,0x0e,0xfa,0x00,  //.+@@.............@@%.
0x05,0xee,0x30,0x00,0x00,0x00,0x00,0x00,0x0e,0xfa,0x00,  //.+@@.............@@%.
0x05,0xee,0x50,0x00,0x00,0x00,0x00,0x00,0x3e,0xe8,0x00,  //.+@@+............@@+.
0x00,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0x00,  //..@@@@@@@@@@@@@@@@@..
0x00,0x3e,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x80,0x00,  //...@@@@@@@@@@@@@@@+..
};


static sgl_icon_pixmap_t delete_icon = {
    .bitmap = default_icon_pixmap,
    .bpp = 4,
    .height = 24,
    .width = 22,
};


static const uint8_t ok_icon_pixmap[] = {
0x3a,0xcc,0xcc,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //.%%%%%%+.............
0xef,0xff,0xff,0xfe,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  //@@@@@@@@+............
0xef,0xff,0xff,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,  //@@@@@@@@%............
0xef,0xff,0xff,0xff,0xec,0xcc,0xcc,0xcc,0x50,0x00,0x00,  //@@@@@@@@@%%%%%%%+....
0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0x00,0x00,  //@@@@@@@@@@@@@@@@@....
0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe8,0x00,0x00,  //@@@@@@@@@@@@@@@@@+...
0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe8,0x00,0x00,  //@@@@@@@@@@@@@@@@@+...
0xef,0xff,0xff,0xc8,0x66,0x66,0x66,0x66,0x63,0x00,0x00,  //@@@@@@%++++++++++....
0xef,0xff,0xc3,0x03,0x33,0x33,0x33,0x33,0x33,0x33,0x00,  //@@@@%................
0xef,0xfe,0x08,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xe0,  //@@@@.+@@@@@@@@@@@@@@@
0xef,0xe3,0xaf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xa0,  //@@@.%@@@@@@@@@@@@@@@%
0xee,0x58,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x00,  //@@++@@@@@@@@@@@@@@@%.
0xc8,0x5e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,  //%++@@@@@@@@@@@@@@@@..
0x53,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x30,0x00,  //+.@@@@@@@@@@@@@@@@...
0x0e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe5,0x00,0x00,  //.@@@@@@@@@@@@@@@@+...
0x8e,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x30,0x00,0x00,  //+@@@@@@@@@@@@@@%.....
};


static sgl_icon_pixmap_t ok_icon = {
    .bitmap = ok_icon_pixmap,
    .bpp = 4,
    .height = 16,
    .width = 22,
};


static const uint8_t cancel_icon_pixmap[] = {
0x00,0x8e,0xe3,0x00,0x00,0x8e,0xe3,0x00,  //..+@@.....+@@...
0x08,0xef,0xfe,0x30,0x08,0xef,0xfe,0x30,  //.+@@@@...+@@@@..
0x3e,0xff,0xff,0xe3,0x8e,0xff,0xff,0xc0,  //.@@@@@@.+@@@@@%.
0x0a,0xef,0xff,0xfe,0xef,0xff,0xfe,0x50,  //.%@@@@@@@@@@@@+.
0x00,0xae,0xff,0xff,0xff,0xff,0xe5,0x00,  //..%@@@@@@@@@@+..
0x00,0x0a,0xef,0xff,0xff,0xfe,0x50,0x00,  //...%@@@@@@@@+...
0x00,0x00,0xcf,0xff,0xff,0xe8,0x00,0x00,  //....%@@@@@@+....
0x00,0x08,0xef,0xff,0xff,0xfe,0x30,0x00,  //...+@@@@@@@@....
0x00,0x8e,0xff,0xff,0xff,0xff,0xe3,0x00,  //..+@@@@@@@@@@...
0x08,0xef,0xff,0xfe,0xef,0xff,0xfe,0x30,  //.+@@@@@@@@@@@@..
0x3e,0xff,0xff,0xe5,0xae,0xff,0xff,0xc0,  //.@@@@@@+%@@@@@%.
0x0a,0xff,0xfe,0x50,0x0a,0xef,0xfe,0x50,  //.%@@@@+..%@@@@+.
0x00,0xaf,0xe5,0x00,0x00,0xae,0xe5,0x00,  //..%@@+....%@@+..
0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,  //................
};


static sgl_icon_pixmap_t cancel_icon = {
    .bitmap = cancel_icon_pixmap,
    .bpp = 4,
    .height = 14,
    .width = 16,
};


const char *text_string = " \
The Federal Bureau of Investigation (FBI) has seized this domain because it is involved in facilitating the illegal distribution of copyrighted materials, including movies, music, software, and games. Engaging in the unauthorized reproduction, distribution, or exhibition of copyrighted material is a violation of federal law. \
";

static int x_ps = 0, y_ps = 0;
static int msgbox_inx = 0;
sgl_obj_t* msgbox[100];

void button_callback(sgl_event_t *event) 
{
    if(event->type == SGL_EVENT_PRESSED) {
        SGL_LOG_INFO("Button long pressed");
        msgbox[msgbox_inx] = sgl_msgbox_create(NULL);
        sgl_obj_set_pos(msgbox[msgbox_inx], 150 + x_ps, 140 + y_ps);
        sgl_obj_set_size(msgbox[msgbox_inx], 300, 200);
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_FONT, SGL_FONT(consolas23));
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_TITLE, SGL_TEXT("FBI WARNING"));
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_TEXT, SGL_TEXT("The Federal Bureau of Investigation (FBI) has seized this domain because it is involved in facilitating the illegal distribution of copyrighted materials, including movies, music, software, and games. Engaging in the unauthorized reproduction, distribution, or exhibition of copyrighted material is a violation of federal law."));
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_APPLY_TEXT, SGL_TEXT("GOT"));
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_CLOSE_TEXT, SGL_TEXT("CANCEL"));
        sgl_obj_set_radius(msgbox[msgbox_inx], 8);

        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_APPLY_ICON, SGL_ICON(ok_icon));
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_CLOSE_ICON, SGL_ICON(cancel_icon));
        sgl_obj_set_style(msgbox[msgbox_inx], SGL_STYLE_MSGBOX_TITLE_ICON, SGL_ICON(cancel_icon));
        x_ps += 20;
        y_ps += 20;

        msgbox_inx++;
    }
    else if(event->type == SGL_EVENT_RELEASED) {
        //sgl_obj_set_destroyed(rect43);
    }
}


void switch_callback(sgl_event_t *event)
{
    sgl_obj_move_down((sgl_obj_t*)event->param);
}


void keyboard_callback(sgl_event_t *event)
{
    SGL_LOG_INFO("keyboard clicked ascii = %d", sgl_obj_get_style(event->obj, SGL_STYLE_KEYBOARD_OPCODE));
}

uint8_t buf[20] = {0};

void slider_callback(sgl_event_t *event)
{
    sgl_obj_t *label = (sgl_obj_t *)event->param;
    sprintf(buf, "H:%lld%%", sgl_obj_get_style(event->obj, SGL_STYLE_VALUE));
    sgl_obj_set_style(label, SGL_STYLE_TEXT, SGL_TEXT(buf));
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

    //sgl_obj_t *desktop2 = sgl_obj_create(NULL);

    sgl_obj_set_pixmap(sgl_screen_act(), &test_pixmap);

    sgl_obj_t *rect = sgl_rect_create(NULL);
    sgl_obj_set_style(rect, SGL_STYLE_POS_X, 20);
    sgl_obj_set_style(rect, SGL_STYLE_POS_Y, 20);
    sgl_obj_set_style(rect, SGL_STYLE_SIZE_W, 480);
    sgl_obj_set_style(rect, SGL_STYLE_SIZE_H, 200);
    sgl_obj_set_alpha(rect, 255);
    sgl_obj_set_clickable(rect);

    sgl_obj_t *rect2 = sgl_rect_create(rect);
    sgl_obj_set_pos(rect2, 20, 20);
    sgl_obj_set_size(rect2, 400, 400);
    sgl_obj_set_color(rect2, SGL_COLOR_BLUE);
    sgl_obj_set_clickable(rect2);
    sgl_obj_set_radius(rect2, 0);
    sgl_obj_set_border_width(rect2, 5);
    sgl_obj_set_border_color(rect2, SGL_COLOR_RED);

    sgl_obj_t *rect3 = sgl_rect_create(NULL);
    sgl_obj_set_pos(rect3, 220, 128);
    sgl_obj_set_size(rect3, 220, 100);
    sgl_obj_set_clickable(rect3);
    sgl_obj_set_alpha(rect3, 128);
    sgl_obj_set_color(rect3, SGL_COLOR_GRAY);

    sgl_obj_t *rect4 = sgl_rect_create(NULL);
    sgl_obj_set_pos(rect4, 100, 0);
    sgl_obj_set_size(rect4, 220, 100);
    sgl_obj_set_clickable(rect4);
    sgl_obj_set_alpha(rect4, 255);
    sgl_obj_set_color(rect4, SGL_COLOR_GRAY);

    sgl_obj_t *label = sgl_label_create(rect);
    sgl_obj_set_font(label, &song23);
    sgl_obj_set_color(label, SGL_COLOR_RED);
    sgl_obj_set_radius(label, 10);
    sgl_obj_set_style(label, SGL_STYLE_TEXT, SGL_TEXT("Hello World!"));
    sgl_obj_set_style(label, SGL_STYLE_ICON, SGL_ICON(ok_icon));
    //sgl_obj_set_style(label, SGL_STYLE_ALIGN, SGL_ALIGN_TOP_LEFT);

    sgl_obj_t *button = sgl_button_create(NULL);
    sgl_obj_set_pos(button, 320, 120);
    sgl_obj_set_size(button, 200, 100);
    sgl_obj_set_font(button, &consolas23);
    sgl_obj_set_radius(button, 60);
    sgl_obj_set_border_width(button, 0);
    sgl_obj_set_style(button, SGL_STYLE_TEXT_COLOR, 0x5de);
    sgl_obj_set_style(button, SGL_STYLE_TEXT, SGL_TEXT("Delete me"));
    sgl_obj_set_alpha(button, 255);
    sgl_obj_set_style(button, SGL_STYLE_ICON, SGL_ICON(delete_icon));

    sgl_obj_t *circle = sgl_circle_create(NULL);
    sgl_obj_set_pos(circle, 100, 100);
    sgl_obj_set_size(circle, 100, 100);
    sgl_obj_set_border_width(circle, 0);
    sgl_obj_set_radius(circle, 49);
    sgl_obj_set_pixmap(circle, &test_pixmap);

    sgl_obj_t *numbberkbd = sgl_numberkbd_create(NULL);
    sgl_obj_set_pos(numbberkbd, 400, 110);
    sgl_obj_set_size(numbberkbd, 300, 200);
    sgl_obj_set_font(numbberkbd, &song23);
    sgl_obj_set_style(numbberkbd, SGL_STYLE_NUMBERKBD_BTN_RADIUS, 10);

    sgl_obj_t *switch_obj = sgl_switch_create(NULL);
    sgl_obj_set_pos(switch_obj, 500, 100);
    sgl_obj_set_size(switch_obj, 100, 40);

    // sgl_obj_t *switch_obj2 = sgl_switch_create(rect);
    // sgl_obj_set_pos(switch_obj2, 150, 380);
    // sgl_obj_set_size(switch_obj2, 100, 40);
    // sgl_obj_set_radius(switch_obj2, 20);
    // sgl_obj_set_alpha(switch_obj2, 255);
    // sgl_obj_set_color(switch_obj2, SGL_COLOR_LIGHT_GRAY);
    // sgl_obj_set_bg_color(switch_obj2, SGL_COLOR_BLACK);
    // sgl_obj_set_style(switch_obj2, SGL_STYLE_KNOB_COLOR, SGL_COLOR(SGL_COLOR_GREEN));
    // sgl_obj_set_border_width(switch_obj2, 2);
    // sgl_obj_set_border_color(switch_obj2, SGL_COLOR_BLUE);

    sgl_obj_t *ring = sgl_ring_create(NULL);
    sgl_obj_set_pos(ring, 200, 100);
    sgl_obj_set_size(ring, 100, 100);

    sgl_obj_set_radius(ring, 49);
    sgl_obj_set_style(ring, SGL_STYLE_RING_WIDTH, 5);
    sgl_obj_set_color(ring, SGL_COLOR_LIGHT_GRAY);
    sgl_obj_set_alpha(ring, 128);

    sgl_obj_t *slider = sgl_slider_create(NULL);
    sgl_obj_set_pos(slider, 300, 250);
    sgl_obj_set_size(slider, 50, 200);
    sgl_obj_set_style(slider, SGL_STYLE_DIRECTION, SGL_DIRECT_VERTICAL);
    sgl_obj_set_color(slider, SGL_COLOR_RED);
    sgl_obj_set_bg_color(slider, SGL_COLOR_GRAY);
    sgl_obj_set_alpha(slider, 255);
    sgl_obj_set_border_width(slider, 2);

    sgl_obj_t *labeldede = sgl_label_create(slider);
    sgl_obj_set_font(labeldede, &consolas23);
    sgl_obj_set_text(labeldede, "10%");

    sgl_obj_set_event_cb(slider, slider_callback, (size_t)labeldede);

    // sgl_obj_t *label2 = sgl_label_create(slider);
    // sgl_obj_set_font(label2, &consolas23);
    // sgl_label_set_text(label2, "10%");

    // sgl_obj_t *textline = sgl_textline_create(NULL);
    // sgl_obj_set_pos(textline, 0, 0);
    // sgl_obj_set_size(textline, 100, 300);
    // sgl_obj_set_radius(textline, 10);
    // //sgl_obj_set_style(textline, SGL_STYLE_TEXT, SGL_TEXT("012345784785754"));
    // sgl_obj_set_style(textline, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_COLOR_BLUE));
    // sgl_obj_set_font(textline, &consolas23);
    // sgl_obj_set_border_width(textline, 2);
    // sgl_obj_set_border_color(textline, SGL_COLOR_LIGHT_GRAY);
    // sgl_obj_set_text(textline, "Text Line dhekdhefkjehfkjqhfjkqehfkejqfheqkjlfqbekjfbqjklfbqejkfbnqejkfbqjkfnqejkfqefjk");
    // sgl_obj_set_clickable(textline);

    // rect43 = sgl_rect_create(NULL);
    // sgl_obj_set_pos(rect43, 0, 0);
    // sgl_obj_set_size(rect43, 800, 480);
    // sgl_obj_set_clickable(rect43);
    // sgl_obj_set_alpha(rect43, 128);

    sgl_obj_set_event_cb(button, button_callback, (size_t)rect43);

    sgl_obj_t *checkbox = sgl_checkbox_create(NULL);
    sgl_obj_set_pos(checkbox, 200, 350);
    sgl_obj_set_size(checkbox, 200, 23);
    sgl_obj_set_font(checkbox, &song23);
    sgl_obj_set_style(checkbox, SGL_STYLE_TEXT, SGL_TEXT("Check Box Test"));
    sgl_obj_set_style(checkbox, SGL_STYLE_CHECKBOX_STATUS, 1);

    //sgl_obj_set_align(msgbox, SGL_ALIGN_CENTER);
    // sgl_obj_set_event_cb(msgbox, msgbox_callback, (size_t)rect43);

    // sgl_obj_t *icon = sgl_icon_create(NULL);
    // sgl_obj_set_pos(icon, 100, 300);
    // sgl_obj_set_size(icon, 100, 100);
    // sgl_obj_set_alpha(icon, 255);

    // sgl_obj_t *listview = sgl_listview_create(NULL);
    // sgl_obj_set_pos(listview, 0, 0);
    // sgl_obj_set_size(listview, 200, 300);
    // sgl_obj_set_font(listview, &song23);
    // sgl_obj_set_color(listview, SGL_COLOR_WHITE);
    //sgl_obj_set_style(listview, SGL_STYLE_BORDER_WIDTH, 3);
    //sgl_obj_set_style(listview, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_COLOR_GRAY));

    // sgl_listview_add_item(listview, &ok_icon, "Hello World!");
    // sgl_listview_add_item(listview, &ok_icon, "Item 2");
    // sgl_listview_add_item(listview, &ok_icon, "Item 3");
    // sgl_listview_add_item(listview, &ok_icon, "Item 4");
    // sgl_listview_add_item(listview, &ok_icon, "Item 5");
    // sgl_listview_add_item(listview, &ok_icon, "Item 6");
    // sgl_listview_add_item(listview, &ok_icon, "Item 7");
    // sgl_listview_add_item(listview, &ok_icon, "Item 8");
    // sgl_listview_add_item(listview, &ok_icon, "Item 9");
    // sgl_listview_add_item(listview, &ok_icon, "Item 10");
    // sgl_listview_add_item(listview, &ok_icon, "Item 11");
    // sgl_listview_add_item(listview, &ok_icon, "Item 12");
    // sgl_listview_add_item(listview, &ok_icon, "Item 13");
    // sgl_listview_add_item(listview, &ok_icon, "Item 2");
    // sgl_listview_add_item(listview, &ok_icon, "Item 3");
    // sgl_listview_add_item(listview, &ok_icon, "Item 4");
    // sgl_listview_add_item(listview, &ok_icon, "Item 5");
    // sgl_listview_add_item(listview, &ok_icon, "Item 6");
    // sgl_listview_add_item(listview, &ok_icon, "Item 7");
    // sgl_listview_add_item(listview, &ok_icon, "Item 8");
    // sgl_listview_add_item(listview, &ok_icon, "Item 9");
    // sgl_listview_add_item(listview, &ok_icon, "Item 10");
    // sgl_listview_add_item(listview, &ok_icon, "Item 11");
    // sgl_listview_add_item(listview, &ok_icon, "Item 12");
    // sgl_listview_add_item(listview, &ok_icon, "Item 13");
    // sgl_listview_add_item(listview, &ok_icon, "Item 2");
    // sgl_listview_add_item(listview, &ok_icon, "Item 3");
    // sgl_listview_add_item(listview, &ok_icon, "Item 4");
    // sgl_listview_add_item(listview, &ok_icon, "Item 5");
    // sgl_listview_add_item(listview, &ok_icon, "Item 6");
    // sgl_listview_add_item(listview, &ok_icon, "Item 7");
    // sgl_listview_add_item(listview, &ok_icon, "Item 8");
    // sgl_listview_add_item(listview, &ok_icon, "Item 9");
    // sgl_listview_add_item(listview, &ok_icon, "Item 10");
    // sgl_listview_add_item(listview, &ok_icon, "Item 11");
    // sgl_listview_add_item(listview, &ok_icon, "Item 12");
    // sgl_listview_add_item(listview, &ok_icon, "Item 13");
    // sgl_listview_add_item(listview, &ok_icon, "Item 2");
    // sgl_listview_add_item(listview, &ok_icon, "Item 3");
    // sgl_listview_add_item(listview, &ok_icon, "Item 4");
    // sgl_listview_add_item(listview, &ok_icon, "Item 5");
    // sgl_listview_add_item(listview, &ok_icon, "Item 6");
    // sgl_listview_add_item(listview, &ok_icon, "Item 7");
    // sgl_listview_add_item(listview, &ok_icon, "Item 8");
    // sgl_listview_add_item(listview, &ok_icon, "Item 9");
    // sgl_listview_add_item(listview, &ok_icon, "Item 10");
    // sgl_listview_add_item(listview, &ok_icon, "Item 11");
    // sgl_listview_add_item(listview, &ok_icon, "Item 12");
    // sgl_listview_add_item(listview, &ok_icon, "Item 13");
    // sgl_listview_add_item(listview, &ok_icon, "Item 2");
    // sgl_listview_add_item(listview, &ok_icon, "Item 3");
    // sgl_listview_add_item(listview, &ok_icon, "Item 4");
    // sgl_listview_add_item(listview, &ok_icon, "Item 5");
    // sgl_listview_add_item(listview, &ok_icon, "Item 6");
    // sgl_listview_add_item(listview, &ok_icon, "Item 7");
    // sgl_listview_add_item(listview, &ok_icon, "Item 8");
    // sgl_listview_add_item(listview, &ok_icon, "Item 9");
    // sgl_listview_add_item(listview, &ok_icon, "Item 10");
    // sgl_listview_add_item(listview, &ok_icon, "Item 11");
    // sgl_listview_add_item(listview, &ok_icon, "Item 12");
    // sgl_listview_add_item(listview, &ok_icon, "Item 13");

    // sgl_obj_set_pixmap(listview, &test_pixmap);

    // sgl_listview_set_child_style(listview, SGL_STYLE_RADIUS, 6);
    // sgl_listview_set_child_style(listview, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_COLOR_GREEN));
    // sgl_listview_set_child_style(listview, SGL_STYLE_ALPHA, 150);
    // sgl_listview_set_child_style(listview, SGL_STYLE_TEXT_ALPHA, 255);

    sgl_obj_t *keyboard = sgl_keyboard_create(NULL);
    sgl_obj_set_size(keyboard, 800, 200);
    sgl_obj_set_font(keyboard, &consolas23);
    sgl_obj_set_pos_align(keyboard, SGL_ALIGN_BOT_MID);
    // sgl_obj_set_event_cb(keyboard, keyboard_callback, (size_t)keyboard);

    // sgl_obj_t *textline2 = sgl_textline_create(NULL);
    // sgl_obj_set_pos(textline2, 0, 0);
    // sgl_obj_set_size(textline2, 100, 50);
    // sgl_obj_set_font(textline2, &consolas23);
    // //sgl_obj_set_pixmap(textline2, &test_pixmap);
    // sgl_obj_set_text(textline2, "Text Box Test dhedhewfhejfdefef454456");
    // sgl_obj_set_style(textline2, SGL_STYLE_TEXT_MARGIN, 10);
    // sgl_obj_set_style(textline2, SGL_STYLE_RADIUS, 20);

    sgl_obj_t *textbox = sgl_textbox_create(NULL);
    sgl_obj_set_pos(textbox, 100, 100);
    sgl_obj_set_size(textbox, 200, 300);
    sgl_obj_set_font(textbox, &consolas23);
    sgl_obj_set_text(textbox, text_string);

    //sgl_obj_delete(rect);

    sgl_event_t eve = {
        .obj = textbox,
        .distance = 10,
    };


    static int count = 0;
    uint32_t x = 0;
    while (!quit) {
        //SDL_Delay(10);
        SDL_PollEvent(&MouseEvent);
        switch (MouseEvent.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        }

        // if(x == 10) {
        //     sgl_obj_set_destroyed(listview);
        // }

        sgl_task_handle();

    
        //sgl_obj_set_dirty(sgl_screen_act());
        x +=1;
        if(x > 1000000 && count < 10) {
            x = 0;
            count ++;
            eve.obj = textbox;
            eve.type = SGL_EVENT_MOVE_UP;
            sgl_event_send(eve);

            eve.obj = switch_obj;
            eve.type = SGL_EVENT_PRESSED;
            sgl_event_send(eve); 
            eve.obj = switch_obj;
            eve.type = SGL_EVENT_RELEASED;
            sgl_event_send(eve);
        }

        // sgl_obj_set_pos(rect, x, x);

        sgl_port_sdl2_increase_frame_count(sdl2_dev);
    }

    sgl_port_sdl2_deinit(sdl2_dev);

    return 0;
}
