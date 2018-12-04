/* -------------------------------------------------------------------------
 *   A Modular Optimization framework for Localization and mApping  (MOLA)
 * Copyright (C) 2018-2019 Jose Luis Blanco, University of Almeria
 * See LICENSE for license information.
 * ------------------------------------------------------------------------- */
/**
 * @file   KittiOdometryDataset.h
 * @brief  RawDataSource from Kitti odometry/SLAM datasets
 * @author Jose Luis Blanco Claraco
 * @date   Nov 29, 2018
 */
#pragma once

#include <mola-kernel/RawDataSourceBase.h>
#include <mrpt/core/Clock.h>
#include <array>

namespace mola
{
/** RawDataSource from Kitti odometry/SLAM datasets.
 * \ingroup mola_sensor_kitti_dataset_grp */
class KittiOdometryDataset : public RawDataSourceBase
{
   public:
    KittiOdometryDataset();
    ~KittiOdometryDataset() override = default;

    // See docs in base class
    void initialize(const std::string& cfg_block) override;
    void spinOnce() override;

   private:
    std::string             kitti_basedir_;  //!< base dir for "sequences/*".
    std::string             replay_selected_seq_;  //!< "00", "01", ...
    mrpt::Clock::time_point replay_begin_time_{};
    bool                    replay_started_{false};
    bool                    publish_lidar_{true};
    double                  time_warp_scale_{1.0};
    std::array<bool, 4>     publish_image_{{true, true, true, true}};

    std::array<std::vector<std::string>, 4> lst_image_files_;
    std::vector<std::string>                lst_velodyne_files_;
    std::vector<double>                     lst_timestamps_;
    double                                  replay_time_{.0};
};

}  // namespace mola
