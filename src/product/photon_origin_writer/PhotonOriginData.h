#pragma once

struct photon_origin_data {
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    float mom_x;
    float mom_y;
    float mom_z;
    float kin;
    unsigned int ev_num;
    char par_nm[8];
    char proc_nm[8];
};
