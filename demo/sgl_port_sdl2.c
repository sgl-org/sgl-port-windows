/* source/port/sgl_port.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Document reference link: docs directory
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sgl.h>


#define  CONFIG_SGL_PANEL_WIDTH         480
#define  CONFIG_SGL_PANEL_HEIGHT        320
#define  CONFIG_SGL_PANEL_BUFFER_LINE   100


static SDL_Renderer * m_renderer = NULL;

typedef struct sgl_port_sdl2 {
    SDL_Window    *m_window;
    SDL_TimerID   anim_systick;
    uint32_t      frame_count;
} sgl_port_sdl2_t;


#if (CONFIG_SGL_FBDEV_PIXEL_DEPTH == 32)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_ARGB8888
#elif (CONFIG_SGL_FBDEV_PIXEL_DEPTH == 24)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_BGR24
#elif (CONFIG_SGL_FBDEV_PIXEL_DEPTH == 16)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_RGB565
#elif (CONFIG_SGL_FBDEV_PIXEL_DEPTH == 8)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_RGB332
#endif

static int sdl_create_windows(SDL_Window **m_window, SDL_Renderer **m_renderer, const char *title)
{
    if ( SDL_CreateWindowAndRenderer( CONFIG_SGL_PANEL_WIDTH, CONFIG_SGL_PANEL_HEIGHT, SDL_WINDOW_SHOWN,
                                      &(*m_window), &(*m_renderer )) != 0 ) {
        return -1;
    }

    return 0;
}

static uint32_t anim_systick(uint32_t interval, void *param)
{
    SGL_UNUSED(param);
    sgl_tick_inc(1);
    return interval;
}


static int mouse_event_interrupt(void *userdata, SDL_Event *event) 
{
    static bool mouse_press = false;
    bool pressed = false;
    SGL_UNUSED(userdata);

    sgl_event_pos_t pos;
    switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouse_press = true;
        pos.x = event->button.x;
        pos.y = event->button.y;
        pressed = true;
        sgl_event_pos_input(pos.x, pos.y, pressed);
        break;

    case SDL_MOUSEBUTTONUP:
        pos.x = event->button.x;
        pos.y = event->button.y;
        mouse_press = false;
        sgl_event_pos_input(pos.x, pos.y, pressed);
        break;

    case SDL_MOUSEMOTION:
        if(mouse_press) {
            pos.x = event->motion.x;
            pos.y = event->motion.y;
            sgl_event_pos_input(pos.x, pos.y, true);
        }
        break;

    default: break;
    }

    return 0;
}


static void panel_flush_area(sgl_area_t *area, sgl_color_t *src)
{
    int16_t w = area->x2 - area->x1 + 1;
    int16_t h = area->y2 - area->y1 + 1;

    const SDL_Rect screen_rect = { .x = area->x1, .y = area->y1, .w = w, .h = h };
    SDL_Texture *texture = SDL_CreateTexture(m_renderer, SDL_PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING, w, h);

    SDL_UpdateTexture(texture, NULL, src, w * sizeof(sgl_color_t));
    SDL_RenderCopy(m_renderer, texture, NULL, &screen_rect);
    SDL_RenderPresent(m_renderer); 
    SDL_DestroyTexture(texture);

    sgl_fbdev_flush_ready();
}


static sgl_color_t panel_buffer0[CONFIG_SGL_PANEL_WIDTH * CONFIG_SGL_PANEL_BUFFER_LINE] = {0};
static sgl_color_t panel_buffer1[CONFIG_SGL_PANEL_WIDTH * CONFIG_SGL_PANEL_BUFFER_LINE] = {0};


void log_stdout(const char *str)
{
    printf(str);
    fflush(stdout);
}


sgl_port_sdl2_t* sgl_port_sdl2_init(void)
{
    sgl_port_sdl2_t *sdl2_dev = NULL;

    sgl_fbinfo_t fbinfo = {
        .xres = CONFIG_SGL_PANEL_WIDTH,
        .yres = CONFIG_SGL_PANEL_HEIGHT,
        .flush_area = panel_flush_area,
        .buffer[0] = panel_buffer0,
        .buffer[1] = panel_buffer1,
        .buffer_size = SGL_ARRAY_SIZE(panel_buffer0),
    };

    sgl_logdev_register(log_stdout);
    sgl_fbdev_register(&fbinfo);

    sdl2_dev = malloc(sizeof(sgl_port_sdl2_t));
    if(sdl2_dev == NULL) {
        SGL_LOG_ERROR("SGL SDL2 device alloc initialize failed");
        return NULL;
    }

    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 ) {
        SGL_LOG_ERROR("SGL SDL2 initialize failed");
        free(sdl2_dev);
        return NULL;
    }

    if(sdl_create_windows(&sdl2_dev->m_window, &m_renderer, "SGL SDL2 demo") < 0) {
        SGL_LOG_ERROR("SGL SDL2 create window failed");
        free(sdl2_dev);
        return NULL;
    }
    sdl2_dev->anim_systick = SDL_AddTimer(1, anim_systick, sdl2_dev);
    sdl2_dev->frame_count = 0;

    /* init sgl */
    sgl_init();

    SDL_AddEventWatch(mouse_event_interrupt, NULL);

    return sdl2_dev;
}

void sgl_port_sdl2_deinit(sgl_port_sdl2_t* sdl2_dev)
{
    SDL_RemoveTimer(sdl2_dev->anim_systick);
    SDL_DestroyWindow(sdl2_dev->m_window);
    SDL_DestroyRenderer(m_renderer);
}
