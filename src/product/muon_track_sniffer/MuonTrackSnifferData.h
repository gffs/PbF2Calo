#pragma once

struct muon_track_data {
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    float mom_x;
    float mom_y;
    float mom_z;
    float spin_x;
    float spin_y;
    float spin_z;
    unsigned int ev_num;
    unsigned int trk_num;
};
