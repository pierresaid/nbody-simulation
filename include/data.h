//
// Created by psaid on 05/03/19.
//

#ifndef NBODY_DATA_H
#define NBODY_DATA_H

#include "config.h"
#include "body.h"

typedef struct data {
    s_config config;
    s_body *bodies;
} s_data;

#endif //NBODY_DATA_H
