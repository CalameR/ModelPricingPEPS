//
// Created by calamer on 1/19/17.
//

#include "Model.h"
#include <cmath>
void Model::shiftSimulation(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep)
{
    // Model::shiftSimulationDebug(shift_path,path,d,h,t,timeStep);
    // /*
    int row = (int) (t/timestep + 0.5);
    int len = path->m - row;

    PnlMat *modelLine = pnl_mat_create(len, 1);

    pnl_mat_extract_subblock(modelLine, path, row, len, d, 1);
    pnl_mat_mult_scalar(modelLine, (1.0 + h));

    pnl_mat_set_subblock(shift_path, path, 0, 0);
    pnl_mat_set_subblock(shift_path, modelLine, row, d);

    pnl_mat_free(&modelLine);
    // */
}

void Model::shiftSimulationDebug(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep)
{
    if (shift_path == NULL) {
        throw std::invalid_argument("shift_path is NULL!");
    }

    if (path == NULL) {
        throw std::invalid_argument("path is NULL!");
    }

    if (shift_path->m != path->m) {
        throw std::invalid_argument("shift_path->m != path->m!");
    }

    if (shift_path->n != path->n) {
        throw std::invalid_argument("shift_path->n != path->n!");
    }

    if (d < 0 || d >= path->n) {
        throw std::invalid_argument("d is not in path column range!");
    }

    if (t < 0.0) {
        throw std::invalid_argument("t can't be negative!");
    }

    if (timestep <= 0.0) {
        throw std::invalid_argument("timestep can't be negative!");
    }

    int row = floor(t / timestep + 0.5);

    if (row >= path->m) {
        throw std::invalid_argument("t is out of bounds!");
    }

    int len = path->m - row;

    PnlMat *modelLine = pnl_mat_create(len, 1);

    pnl_mat_extract_subblock(modelLine, path, row, len, d, 1);
    pnl_mat_mult_scalar(modelLine, (1.0 + h));

    pnl_mat_set_subblock(shift_path, path, 0, 0);
    pnl_mat_set_subblock(shift_path, modelLine, row, d);

    pnl_mat_free(&modelLine);

}

