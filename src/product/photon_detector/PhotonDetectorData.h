#pragma once

struct photon_detector_data {
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    float mom_x;
    float mom_y;
    float mom_z;
    float pos_org_x;
    float pos_org_y;
    float pos_org_z;
    float mom_org_x;
    float mom_org_y;
    float mom_org_z;
    unsigned int ev_num;
    unsigned int bnc_num;
};
