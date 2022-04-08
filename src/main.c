#include <multiboot.h>
#include <gfx/video.h>

int keyboard_read(void) {
	int key = -1;
	unsigned char status;

	status = inb(0x64);
	if (status & (1 << 0))
		key = inb(0x60);

	return key;
}

void draw_ball(int x, int y, int color) {
    int component[5] = {x, y, 10, 10, color};
    draw_component(component);
}

int draw_component(int *component) {
    int x = component[0];
    int y = component[1];
    int width = component[2];
    int height = component[3];
    int color = component[4];

    draw_square(x, y, width, height, color);

    return 0;
}

int draw_counter(int player1, int player2){
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
        draw_component(digits[player1][i]);
    }

    length = sizeof(digits[player2]) / sizeof(digits[player2][0]);
    int x_player_two = 400;
    for(int i = 0; i < length; i++){
        int *player2Score = digits[player2][i];
        player2Score[0] = player2Score[0] + x_player_two;
        draw_component(player2Score);
        player2Score[0] = player2Score[0] - x_player_two;
    }

    return 0;
}

void draw_player(int x, int y, int color){
    int lateral_bar_width = 10;
    int lateral_bar_height = 100;
    int component[5] = {x, y, lateral_bar_width, lateral_bar_height, color};
    draw_component(component);
}

int main(unsigned long addr)
{
    multiboot_info_t *mbi;

    mbi = (multiboot_info_t *) addr;

    int lateral_bar_height = 100;
    int max_screen_height = mbi->framebuffer_height;
    int max_screen_width = mbi->framebuffer_width;
    int y_screen_center_lateral_bars = (max_screen_height / 2) - 50;
    int x_screen_center_central_bar = (max_screen_width / 2);
    int x_right_bar = max_screen_width - 20;

    int player1_x = 10;
    int player1_y = y_screen_center_lateral_bars;

    int player2_x = x_right_bar;
    int player2_y = y_screen_center_lateral_bars;

    int ball_x = 50;
    int ball_y = 500;

    int white = 0xD3D3D3;
    int black = 0x000000;

    // Central bar properties
    int central_bar[5] = {x_screen_center_central_bar, 0, 10, max_screen_height, 0xD3D3D3};

    screen_init(
        mbi->framebuffer_addr, 
        mbi->framebuffer_width, 
        mbi->framebuffer_height
    );

        // Left bar properties
    draw_player(player1_x, player1_y, white);

    // Right bar properties
    draw_player(x_right_bar, y_screen_center_lateral_bars, white);


    draw_component(central_bar);
    draw_counter(1, 0);

    while (1) {
        char key_code = keyboard_read();
        // Desce o player 1
        if (key_code == 31 && (player1_y + lateral_bar_height) <= max_screen_height){
            draw_player(player1_x, player1_y, black);
            player1_y = player1_y + 20;
            draw_player(player1_x, player1_y, white);
        }
        // Sobe o player 1
        if (key_code == 17 && player1_y >= 1){
            draw_player(player1_x, player1_y, black);
            player1_y = player1_y - 20;
            draw_player(player1_x, player1_y, white);
        }

        // Desce o player 2
        if (key_code == 80 && (player2_y + lateral_bar_height) <= max_screen_height){
            draw_player(player2_x, player2_y, black);
            player2_y = player2_y + 20;
            draw_player(player2_x, player2_y, white);
        }
        
        // Sobe o player 2
        if (key_code == 72 && player2_y >= 1){
            draw_player(player2_x, player2_y, black);
            player2_y = player2_y - 20;
            draw_player(player2_x, player2_y, white);
        }

        // Desce a bola
        if (key_code == 35 && (ball_y + 10) < max_screen_height){
            draw_ball(ball_x, ball_y, black);
            ball_y = ball_y + 20;
            draw_ball(ball_x, ball_y, white);
        }
        
        // Sobe a bola
        if (key_code == 21 && ball_y >= 1){
            draw_ball(ball_x, ball_y, black);
            ball_y = ball_y - 20;
            draw_ball(ball_x, ball_y, white);
        }

        // Move para esquerda a bola
        if (key_code == 36 && (ball_x + 10) < max_screen_width){
            draw_ball(ball_x, ball_y, black);
            ball_x = ball_x + 20;
            draw_ball(ball_x, ball_y, white);
        }
        
        // Move para a direita a bola
        if (key_code == 34 && ball_x >= 1){
            draw_ball(ball_x, ball_y, black);
            ball_x = ball_x - 20;
            draw_ball(ball_x, ball_y, white);
        }
    }

    return 0;
}
