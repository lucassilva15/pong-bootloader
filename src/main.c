#include <multiboot.h>
#include <gfx/video.h>

int init_component(int *component){
    int x = component[0];
    int y = component[1];
    int width = component[2];
    int height = component[3];
    int color = component[4];

    draw_square(x, y, width, height, color);

    return 0;
}

int counter_draw(int player1, int player2){
    int digits[10][10][5] = {
        {
            {150, 50, 80, 10, 0xD3D3D3}, 
            {150, 150, 80, 10, 0xD3D3D3}, 
            {150, 50, 10, 100, 0xD3D3D3}, 
            {230, 50, 10, 110, 0xD3D3D3}
        },
        {
            {190, 50, 10, 110, 0xD3D3D3}
        },
        {
            {150, 50, 80, 10, 0xD3D3D3}, 
            {230, 50, 10, 50, 0xD3D3D3},
            {150, 90, 80, 10, 0xD3D3D3}, 
            {150, 90, 10, 40, 0xD3D3D3}, 
            {150, 120, 80, 10, 0xD3D3D3},
        },
        {
            {150, 50, 80, 10, 0xD3D3D3}, 
            {230, 50, 10, 50, 0xD3D3D3},
            {150, 90, 80, 10, 0xD3D3D3}, 
            {230, 90, 10, 40, 0xD3D3D3}, 
            {150, 120, 80, 10, 0xD3D3D3},
        },
        { 
            {150, 50, 10, 50, 0xD3D3D3},
            {230, 50, 10, 50, 0xD3D3D3},
            {150, 90, 80, 10, 0xD3D3D3}, 
            {230, 90, 10, 40, 0xD3D3D3}
        },
        {
            {150, 50, 90, 10, 0xD3D3D3}, 
            {150, 50, 10, 50, 0xD3D3D3},
            {150, 90, 80, 10, 0xD3D3D3}, 
            {230, 90, 10, 40, 0xD3D3D3}, 
            {150, 120, 80, 10, 0xD3D3D3},
        },
        {
            {150, 50, 10, 80, 0xD3D3D3},
            {150, 90, 80, 10, 0xD3D3D3}, 
            {230, 90, 10, 40, 0xD3D3D3}, 
            {150, 120, 80, 10, 0xD3D3D3},
        },
        {
            {150, 50, 80, 10, 0xD3D3D3},
            {230, 50, 10, 80, 0xD3D3D3},
        },
        {
            {150, 50, 80, 10, 0xD3D3D3}, 
            {150, 50, 10, 80, 0xD3D3D3},
            {150, 120, 80, 10, 0xD3D3D3}, 
            {230, 50, 10, 80, 0xD3D3D3},
            {150, 90, 80, 10, 0xD3D3D3}, 
        },
        {
            {150, 50, 80, 10, 0xD3D3D3}, 
            {150, 50, 10, 50, 0xD3D3D3},
            {150, 95, 80, 10, 0xD3D3D3}, 
            {230, 50, 10, 110, 0xD3D3D3},
        },
    };

    char length = sizeof(digits[player1]) / sizeof(digits[player1][0]);
    for(int i = 0; i < length; i++){
        init_component(digits[player1][i]);
    }

    length = sizeof(digits[player2]) / sizeof(digits[player2][0]);
    int x_player_two = 400;
    for(int i = 0; i < length; i++){
        int *player2Score = digits[player2][i];
        player2Score[0] = player2Score[0] + x_player_two;
        init_component(player2Score);
        player2Score[0] = player2Score[0] - x_player_two;
    }

    return 0;
}

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
    counter_draw(1, 0);

    while (1) {

    }

    return 0;
}
