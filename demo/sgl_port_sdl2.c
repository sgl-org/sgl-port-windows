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


#define  CONFIG_SGL_PANEL_WIDTH         800
#define  CONFIG_SGL_PANEL_HEIGHT        480
#define  CONFIG_SGL_PANEL_BUFFER_LINE   40


static SDL_Renderer * m_renderer = NULL;

typedef struct sgl_port_sdl2 {
    SDL_Window    *m_window;
    SDL_TimerID   systick;
    SDL_TimerID   anim_systick;
    size_t        frame_count;
} sgl_port_sdl2_t;


#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == 32)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_ARGB8888
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 24)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_BGR24
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 16)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_RGB565
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 8)
#define  SDL_PIXEL_FORMAT       SDL_PIXELFORMAT_RGB332
#endif


sgl_color_t sdl2_frame_buffer[CONFIG_SGL_PANEL_WIDTH * CONFIG_SGL_PANEL_HEIGHT] = {0};


static int sdl_create_windows(SDL_Window **m_window, SDL_Renderer **m_renderer, const char *title)
{
    if ( SDL_CreateWindowAndRenderer( CONFIG_SGL_PANEL_WIDTH, CONFIG_SGL_PANEL_HEIGHT, SDL_WINDOW_SHOWN,
                                      &(*m_window), &(*m_renderer )) != 0 ) {
        return -1;
    }

    SDL_SetWindowTitle(*m_window, title);

    // Clear the window with a SGL_COLOR_WHITE background
    for(int i = 0; i< CONFIG_SGL_PANEL_WIDTH * CONFIG_SGL_PANEL_HEIGHT; i++) {
        sdl2_frame_buffer[i] = SGL_COLOR_WHITE;
    }

    SDL_SetRenderDrawColor( *m_renderer, 0, 0, 0, 255);
    SDL_RenderClear( *m_renderer );
    return 0;
}


static void flush_window(SDL_Renderer * m_renderer)
{
    SDL_Texture *texture = SDL_CreateTexture(m_renderer, SDL_PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING, CONFIG_SGL_PANEL_WIDTH, CONFIG_SGL_PANEL_HEIGHT);
    SDL_UpdateTexture(texture, NULL, sdl2_frame_buffer, CONFIG_SGL_PANEL_WIDTH * sizeof(sgl_color_t));
    SDL_RenderCopy(m_renderer, texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
    SDL_DestroyTexture(texture);
}


static uint32_t system_tick(uint32_t interval, void *param)
{
    sgl_mm_monitor_t mm = sgl_mm_get_monitor();
    sgl_port_sdl2_t *sdl2_dev = (sgl_port_sdl2_t*)param;
    SGL_UNUSED(mm);
    printf("SGL SDL2 Frame = %lld\n", sdl2_dev->frame_count);
    printf("Memory: total: %lld used: %lld, free = %lld\n", mm.total_size, mm.used_size, mm.free_size);
    sdl2_dev->frame_count = 0;
	return interval;
}


static uint32_t anim_systick(uint32_t interval, void *param)
{
    SGL_UNUSED(param);
    sgl_anim_tick_inc(1);
    return interval;
}


static bool mouse_press = false;

static int mouse_event_interrupt(void *userdata, SDL_Event *event) 
{
    SGL_UNUSED(userdata);

    sgl_event_pos_t pos;
    switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouse_press = true;
        pos.x = event->motion.x;
        pos.y = event->motion.y;
        sgl_event_send_pos(pos, SGL_EVENT_PRESSED);
        break;

    case SDL_MOUSEBUTTONUP:
        pos.x = event->motion.x;
        pos.y = event->motion.y;
        mouse_press = false;
        sgl_event_send_pos(pos, SGL_EVENT_RELEASED);
        break;

    case SDL_MOUSEMOTION:
        if(mouse_press) {
            pos.x = event->motion.x;
            pos.y = event->motion.y;
            sgl_event_send_pos(pos, SGL_EVENT_MOTION);
        }
        break;

    default: break;
    }

    return 0;
}


static void panel_flush_area(int16_t x, int16_t y, int16_t w, int16_t h, sgl_color_t *src)
{
    sgl_color_t *dest = sdl2_frame_buffer;
    dest += (x + y * CONFIG_SGL_PANEL_WIDTH);

    for(int i = 0; i < h; i ++) {
        memcpy(dest, src, w * sizeof(sgl_color_t));
        dest += CONFIG_SGL_PANEL_WIDTH;
        src += w;
    }

    flush_window(m_renderer);
}


static sgl_color_t panel_buffer[CONFIG_SGL_PANEL_WIDTH * CONFIG_SGL_PANEL_BUFFER_LINE] = {0};


void log_stdout(const char *str)
{
    printf(str);
    fflush(stdout);
}


sgl_port_sdl2_t* sgl_port_sdl2_init(void)
{
    sgl_port_sdl2_t *sdl2_dev = NULL;

    sgl_device_fb_t fb_dev = {
        .xres = CONFIG_SGL_PANEL_WIDTH,
        .yres = CONFIG_SGL_PANEL_HEIGHT,
        .xres_virtual = CONFIG_SGL_PANEL_WIDTH,
        .yres_virtual = CONFIG_SGL_PANEL_HEIGHT,
        .flush_area = panel_flush_area,
        .framebuffer = panel_buffer,
        .framebuffer_size = SGL_ARRAY_SIZE(panel_buffer),
    };

    sgl_device_log_register(log_stdout);
    sgl_device_fb_register(&fb_dev);

    /* init sgl */
    sgl_init();

    sdl2_dev = sgl_malloc(sizeof(sgl_port_sdl2_t));
    if(sdl2_dev == NULL) {
        SGL_LOG_ERROR("SGL SDL2 device alloc initialize failed");
        return NULL;
    }

    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 ) {
        SGL_LOG_ERROR("SGL SDL2 initialize failed");
        sgl_free(sdl2_dev);
        return NULL;
    }

    if(sdl_create_windows(&sdl2_dev->m_window, &m_renderer, "SGL SDL2 demo") < 0) {
        SGL_LOG_ERROR("SGL SDL2 create window failed");
        sgl_free(sdl2_dev);
        return NULL;
    }

    sdl2_dev->systick = SDL_AddTimer(1000, system_tick, sdl2_dev);
    sdl2_dev->anim_systick = SDL_AddTimer(1, anim_systick, sdl2_dev);
    sdl2_dev->frame_count = 0;

    SDL_AddEventWatch(mouse_event_interrupt, NULL);

    return sdl2_dev;
}


size_t sgl_port_sdl2_get_frame_count(sgl_port_sdl2_t* sdl2_dev)
{
    return sdl2_dev->frame_count;
}


void sgl_port_sdl2_increase_frame_count(sgl_port_sdl2_t* sdl2_dev)
{
    sdl2_dev->frame_count ++;
}


void sgl_port_sdl2_deinit(sgl_port_sdl2_t* sdl2_dev)
{
    SDL_RemoveTimer(sdl2_dev->systick);
    SDL_RemoveTimer(sdl2_dev->anim_systick);
    SDL_DestroyWindow(sdl2_dev->m_window);
    SDL_DestroyRenderer(m_renderer);
}
