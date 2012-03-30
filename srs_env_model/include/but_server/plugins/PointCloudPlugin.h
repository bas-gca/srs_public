/******************************************************************************
 * \file
 *
 * $Id:$
 *
 * Copyright (C) Brno University of Technology
 *
 * This file is part of software developed by dcgm-robotics@FIT group.
 *
 * Author: Vit Stancl (stancl@fit.vutbr.cz)
 * Supervised by: Michal Spanel (spanel@fit.vutbr.cz)
 * Date: dd/mm/2012
 * 
 * This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this file.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PointCloudPlugin_H_included
#define PointCloudPlugin_H_included

#include <but_server/ServerTools.h>
#include <message_filters/subscriber.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/message_filter.h>

namespace srs
{

    class CPointCloudPlugin : public CServerPluginBase, public COctomapCrawlerBase<tButServerOcTree::NodeType>, public CDataHolderBase< tPointCloud >
    {
    public:
        /// Constructor
        CPointCloudPlugin(const std::string & name, bool subscribe = true );

        /// Destructor
        virtual ~CPointCloudPlugin();

        //! Enable or disable publishing
        void enable( bool enabled ){ m_publishPointCloud = enabled; }

        //! Should plugin publish data?
        bool shouldPublish();

        //! Initialize plugin - called in server constructor
        virtual void init(ros::NodeHandle & node_handle);

        //! Initialize plugin - called in server constructor, enable or disable subscription.
        virtual void init(ros::NodeHandle & node_handle, bool subscribe){ m_bSubscribe = subscribe; init(node_handle); }

        //! Called when new scan was inserted and now all can be published
        virtual void onPublish(const ros::Time & timestamp);

        //! Set used octomap frame id and timestamp
        virtual void onFrameStart( const SMapParameters & par );

        /// hook that is called when traversing occupied nodes of the updated Octree (does nothing here)
        virtual void handleOccupiedNode(const srs::tButServerOcTree::iterator& it, const SMapParameters & mp);

        /// Called when all nodes was visited.
        virtual void handlePostNodeTraversal(const SMapParameters & mp);

    protected:


        /**
        * @brief Insert point cloud callback
        *
        * @param cloud Input point cloud
        */
        void insertCloudCallback(const sensor_msgs::PointCloud2::ConstPtr& cloud);


    protected:
        //! Is publishing enabled?
        bool m_publishPointCloud;

        //! Point cloud publisher name
        std::string m_pcPublisherName;

        //! Point cloud subscriber name
        std::string m_pcSubscriberName;

        /// Subscriber - point cloud
        message_filters::Subscriber<sensor_msgs::PointCloud2> *m_pcSubscriber;

        //! Message filter (we only want point cloud 2 messages)
        tf::MessageFilter<sensor_msgs::PointCloud2> *m_tfPointCloudSub;

        /// Point cloud publisher
        ros::Publisher m_pcPublisher;

        //! Should this plugin subscribe to some publishing topic?
        bool m_bSubscribe;


        //! Transform listener
        tf::TransformListener m_tfListener;

        //
        bool m_latchedTopics;

        //! Used frame id (point cloud will be transformed to it)
        std::string m_pcFrameId;

        /// Crawled octomap frame id
        std::string m_ocFrameId;

        //! Do pointcloud filtering?
        bool m_bFilterPC;

        //! Minimal Z value
        double m_pointcloudMinZ;

        //! Maximal Z value
        double m_pointcloudMaxZ;

    }; // class CPointCloudPlugin

    /// Declare holder object - partial specialization of the default holder with predefined connection settings
    template< class tpOctomapPlugin >
    struct SPointCloudPluginHolder : public  srs::CCrawlingPluginHolder< srs::CPointCloudPlugin, tpOctomapPlugin >
    {
    protected:
        /// Define holder type
        typedef srs::CCrawlingPluginHolder< srs::CPointCloudPlugin, tpOctomapPlugin > tHolder;

    public:
        /// Create holder
        SPointCloudPluginHolder( const std::string & name )
        : tHolder(  name,  tHolder::ON_START | tHolder::ON_OCCUPIED | tHolder::ON_STOP)
        {

        }

    }; // struct SPointCloudPluginHolder

} // namespace srs


// PointCloudPubPlugin_H_included
#endif

