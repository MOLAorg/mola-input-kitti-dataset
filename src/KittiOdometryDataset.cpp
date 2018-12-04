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

/** \defgroup mola_sensor_kitti_dataset_grp RawDataSource from Kitti
 * odometry/SLAM datasets
 */

#include <mola-sensor-kitti-dataset/KittiOdometryDataset.h>
#include <mrpt/system/CDirectoryExplorer.h>
#include <mrpt/system/filesystem.h>  //ASSERT_DIRECTORY_EXISTS_()
#include <yaml-cpp/yaml.h>

using namespace mola;

MOLA_REGISTER_RAWDATASOURCE(KittiOdometryDataset)

KittiOdometryDataset::KittiOdometryDataset()
{
    this->setLoggerName("KittiOdometryDataset");
}

void KittiOdometryDataset::initialize(const std::string& cfg_block)
{
    MRPT_START
    MRPT_LOG_DEBUG_STREAM("Initializing with these params:\n" << cfg_block);

    // Mandatory parameters:
    auto cfg = YAML::Load(cfg_block);

    ENSURE_YAML_ENTRY_EXISTS(cfg, "params");
    auto params = cfg["params"];

    ENSURE_YAML_ENTRY_EXISTS(params, "base_dir");
    kitti_basedir_ = params["base_dir"].as<std::string>();
    ASSERT_DIRECTORY_EXISTS_(kitti_basedir_);

    ENSURE_YAML_ENTRY_EXISTS(params, "sequence");
    replay_selected_seq_ = params["sequence"].as<std::string>();
    const std::string seq_dir =
        kitti_basedir_ + "/sequences/" + replay_selected_seq_;
    ASSERT_DIRECTORY_EXISTS_(seq_dir);

    // Optional params with default values:
    time_warp_scale_ = params["time_warp_scale"].as<double>(time_warp_scale_);
    publish_lidar_   = params["publish_lidar"].as<bool>(publish_lidar_);
    for (unsigned int i = 0; i < 4; i++)
        publish_image_[i] =
            params[mrpt::format("publish_image_%u", i)].as<bool>(
                publish_image_[i]);

    // Make list of all existing files and preload everything we may need later
    // to quickly replay the dataset in realtime:

    // Odometry datasets:
    // Images  : "000000.png"
    // Velodyne: "001092.bin"
    // Raw datasets:
    // Velodyne: "0000000000.bin"
    // Images  : "0000000000.png"
    // Non-uniform format. Solution: make a list of files and sort them.
    using direxpl = mrpt::system::CDirectoryExplorer;
    direxpl::TFileInfoList lstFiles;
    direxpl::explore(seq_dir + "velodyne", FILE_ATTRIB_ARCHIVE, lstFiles);

    MRPT_END
}
void KittiOdometryDataset::spinOnce()
{
    MRPT_START

    if (!replay_started_)
    {
        replay_begin_time_ = mrpt::Clock::now();
        replay_started_    = true;
    }

    MRPT_END
}
