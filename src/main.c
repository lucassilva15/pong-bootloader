#include <multiboot.h>
#include <gfx/video.h>

int main(unsigned long addr)
{
    multiboot_info_t *mbi;

    mbi = (multiboot_info_t *) addr;

    int lateral_bar_height = 100;
    int lateral_bar_width = 10;
    int max_screen_height = mbi->framebuffer_height;
    int max_screen_width = mbi->framebuffer_width;
    int x_screen_center_lateral_bars = (max_screen_height / 2) - (lateral_bar_height / 2);
    int x_screen_center_central_bar = (max_screen_width / 2);
    int x_right_bar = max_screen_width - 20;
    int y_screen_center = (max_screen_width / 2);

    // Left bar properties
    int left_bar[5] = {10, x_screen_center_lateral_bars, lateral_bar_width, lateral_bar_height, 0xD3D3D3};

    // Right bar properties
    int right_bar[5] = {x_right_bar, x_screen_center_lateral_bars, lateral_bar_width, lateral_bar_height, 0xD3D3D3};

    // Central bar properties
    int central_bar[5] = {x_screen_center_central_bar, 0, lateral_bar_width, max_screen_height, 0xD3D3D3};

    screen_init(
        mbi->framebuffer_addr, 
        mbi->framebuffer_width, 
        mbi->framebuffer_height
    );

    init_component(left_bar);
    init_component(right_bar);
    init_component(central_bar);

    while (1) {

    }

    return 0;
}

void init_component(int *component){
    int x = component[0];
    int y = component[1];
    int width = component[2];
    int height = component[3];
    int color = component[4];

    draw_square(x, y, width, height, color);
}

