#pragma once

extern struct rgb_color ascii_cols[128];
extern struct rgb_color main_cols[128];
extern char ascii_art[128][128];
extern char label_order[128][128];
extern int  label_order_count;
extern bool bold;
extern int flag_max_colors;
extern int main_max_colors;
extern int max_width;
extern int max_height;
extern char ibuffer[128][128];
extern int  buffer_pos;
extern struct label labels[128];
extern int   label_count;

void fetch(char * path);
