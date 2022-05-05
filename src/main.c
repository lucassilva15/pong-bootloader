#include <multiboot.h>
#include <gfx/video.h>

//GLOBAL PROPERTIES
int lateral_bar_height;
int max_screen_height;
int max_screen_width;
int y_screen_center_lateral_bars;
int x_screen_center_central_bar;
int x_right_bar;
int player1_x;
int player1_y;
int player2_x;
int player2_y;
int ball_x;
int ball_y;
int white;
int black;
int central_bar[5];
int player1_goals = 0;
int player2_goals = 0;

// Variables to tests
int i = 0;
int j = 0;
int k = 0;

// MOVE PLAYERS AND BALL
void move_player1(int value) {
    draw_player(player1_x, player1_y, black);
    player1_y = player1_y + (value);
    draw_player(player1_x, player1_y, white);
}

void move_player2(int value) {
    draw_player(player2_x, player2_y, black);
    player2_y = player2_y + (value);
    draw_player(player2_x, player2_y, white);
}

void move_ball_horizontal(int value) {
    draw_ball(ball_x, ball_y, black);
    ball_x = ball_x + (value);
    draw_ball(ball_x, ball_y, white);
}

void move_ball_vertical(int value) {
    draw_ball(ball_x, ball_y, black);
    ball_y = ball_y + (value);
    draw_ball(ball_x, ball_y, white);
}

// DRAW COMPONENTS
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

void draw_all_counter_black() {
    draw_counter(0, 1, black);
    draw_counter(9, 2, black);
    draw_counter(8, 3, black);
    draw_counter(7, 4, black);
    draw_counter(6, 5, black);
    draw_counter(5, 6, black);
    draw_counter(4, 7, black);
    draw_counter(3, 8, black);
    draw_counter(2, 9, black);
    draw_counter(1, 0, black);
}

int draw_counter(int player1, int player2, int color) {
    int digits[10][10][5] = {
        {
            {150, 50, 80, 10, color},
            {150, 150, 80, 10, color},
            {150, 50, 10, 100, color},
            {230, 50, 10, 110, color}
        },
        {
            {190, 50, 10, 110, color}
        },
        {
            {150, 50, 80, 10, color},
            {230, 50, 10, 50, color},
            {150, 90, 80, 10, color},
            {150, 90, 10, 40, color},
            {150, 120, 80, 10, color},
        },
        {
            {150, 50, 80, 10, color},
            {230, 50, 10, 50, color},
            {150, 90, 80, 10, color},
            {230, 90, 10, 40, color},
            {150, 120, 80, 10, color},
        },
        { 
            {150, 50, 10, 50, color},
            {230, 50, 10, 50, color},
            {150, 90, 80, 10, color},
            {230, 90, 10, 40, color}
        },
        {
            {150, 50, 90, 10, color},
            {150, 50, 10, 50, color},
            {150, 90, 80, 10, color},
            {230, 90, 10, 40, color},
            {150, 120, 80, 10, color},
        },
        {
            {150, 50, 10, 80, color},
            {150, 90, 80, 10, color},
            {230, 90, 10, 40, color},
            {150, 120, 80, 10, color},
        },
        {
            {150, 50, 80, 10, color},
            {230, 50, 10, 80, color},
        },
        {
            {150, 50, 80, 10, color},
            {150, 50, 10, 80, color},
            {150, 120, 80, 10, color},
            {230, 50, 10, 80, color},
            {150, 90, 80, 10, color},
        },
        {
            {150, 50, 80, 10, color},
            {150, 50, 10, 50, color},
            {150, 95, 80, 10, color},
            {230, 50, 10, 110, color},
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

// Function to use serial port and debug feature
static int init_serial(int port) {
   outb(port + 1, 0x00);    // Disable all interrupts
   outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(port + 1, 0x00);    //                  (hi byte)
   outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   if(inb(port + 0) != 0xAE) {
      return 1;
   }

   outb(port + 4, 0x0F);
   return 0;
}

// Read functions
int serial_received(int port) {
   return inb(port + 5) & 1;
}

char read_serial(int port) {
   while (serial_received(port) == 0);

   return inb(port);
}

// Write functions
int is_transmit_empty(int port) {
   return inb(port + 5) & 0x20;
}

void write_serial(int port, char a) {
   while (is_transmit_empty(port) == 0);

   outb(port, a);
}

void verifyIfGoal() {
    if (ball_x <= 0) {
        player2_goals++;
        if (player2_goals > 9) {
            player2_goals = 0;
            player1_goals = 0;
        }
        draw_all_counter_black();
        draw_counter(player1_goals, player2_goals, white);

        draw_ball(ball_x, ball_y, black);
        ball_x = 350;
        ball_y = 300;
        draw_ball(ball_x, ball_y, white);
        draw_player(player1_x, player1_y, white);
    }

    if ((ball_x + 10) >= max_screen_width) {
        player1_goals++;
        if (player1_goals > 9) {
            player1_goals = 0;
            player2_goals = 0;
        }
        draw_all_counter_black();
        draw_counter(player1_goals, player2_goals, white);

        draw_ball(ball_x, ball_y, black);
        ball_x = 450;
        ball_y = 300;
        draw_ball(ball_x, ball_y, white);
        draw_player(player1_x, player1_y, white);
    }
}

void isr0() {
    verifyIfGoal();
}

void isr1() {
    char key_code = inb(0x60);

    // Player 1 up and down
    if (key_code == 31 && (player1_y + lateral_bar_height) <= max_screen_height) {
        move_player1(20);
    }
    if (key_code == 17 && player1_y >= 1) {
        move_player1(-20);
    }

    // Player 2 up and down
    if (key_code == 80 && (player2_y + lateral_bar_height) <= max_screen_height) {
        move_player2(20);
    }
    if (key_code == 72 && player2_y >= 1) {
        move_player2(-20);
    }

    // Ball up, down, left and right
    if (key_code == 35) {
        move_ball_vertical(20);
    }
    if (key_code == 21 && ball_y >= 1) {
        move_ball_vertical(-20);
    }
    if (key_code == 36) {
        move_ball_horizontal(20);
    }
    if (key_code == 34 && ball_x >= 1) {
        move_ball_horizontal(-20);
    }
}

int main(unsigned long addr)
{
    multiboot_info_t *mbi;
    mbi = (multiboot_info_t *) addr;

    lateral_bar_height = 100;
    max_screen_height = mbi->framebuffer_height;
    max_screen_width = mbi->framebuffer_width;
    y_screen_center_lateral_bars = (max_screen_height / 2) - 50;
    x_screen_center_central_bar = (max_screen_width / 2);
    x_right_bar = max_screen_width - 20;

    // Player properties
    player1_x = 10;
    player1_y = y_screen_center_lateral_bars;
    player2_x = x_right_bar;
    player2_y = y_screen_center_lateral_bars;

    // Ball properties
    ball_x = 450;
    ball_y = 300;

    // Colors properties
    white = 0xD3D3D3;
    black = 0x000000;

    // Central bar properties
    central_bar[0] = x_screen_center_central_bar;
    central_bar[1] = 0;
    central_bar[2] = 10;
    central_bar[3] = max_screen_height;
    central_bar[4] = 0xD3D3D3;

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
    draw_counter(0, 0, white);

    // Initialize serial ports
    init_serial(0x3f8);

    while (1) {

    }

    return 0;
}
