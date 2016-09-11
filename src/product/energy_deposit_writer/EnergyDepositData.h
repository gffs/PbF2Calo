#pragma once

struct energy_deposit_data {
    float eng;
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    unsigned int ev_num;
    char vol_nm[16];
    char par_nm[8];
};
