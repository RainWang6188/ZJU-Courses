#pragma once
int jump_state = 0;
int down_state = 0;
int Game_over_state = 0;
bool Game_start_state = 0;

int en_rotate = 0;
int run_state = 1;
int draw_all = -3;/*
				 <=0 ��ʾ�滭�ĸ��� -draw_all
				 >0��ʾdra wall

				 */
int show_texture ;
int ground_type;
int down_down_state;//���޽���״̬


void init_game_state() {
	jump_state = 0;
	down_state = 0;
	Game_over_state = 0;
    Game_start_state = 0;

    en_rotate = 0;
	run_state = 1;
	draw_all = -3;
	show_texture = 1;
	down_down_state = 0;
	ground_type = 1;
}