// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _FASTDDS_SHAREDMEM_MANAGER_H_
#define _FASTDDS_SHAREDMEM_MANAGER_H_

#include <atomic>
#include <list>
#include <unordered_map>

#include <rtps/transport/shared_mem/SharedMemGlobal.hpp>

namespace eprosima {
namespace fastdds {
namespace rtps {

class SharedMemManager
{
public:

    SharedMemManager(
            const std::string& domain_name)
        : global_segment_(domain_name) // TODO: How to do this?
    {
    }

    struct BufferNode
    {
        std::atomic<uint32_t> ref_count;
        SharedMemSegment::offset data_offset;
        uint32_t data_size;
    };

    class Buffer
    {
    public:

        virtual void* data() = 0;
        virtual uint32_t size() = 0;
    };

    class SharedMemBuffer : public Buffer
    {
    public:

        SharedMemBuffer(
                std::shared_ptr<SharedMemSegment>& segment,
                SharedMemSegment::Id segment_id,
                BufferNode* buffer_node)
            : segment_(segment)
            , segment_id_(segment_id)
            , buffer_node_(buffer_node)
        {
            increase_ref();
        }

        ~SharedMemBuffer()
        {
            decrease_ref();
        }

        void* data() override
        {
            return segment_->get().get_address_from_handle(buffer_node_->data_offset);
        }

        uint32_t size() override
        {
            return buffer_node_->data_size;
        }

        SharedMemSegment::offset node_offset()
        {
            return segment_->get_offset_from_address(buffer_node_);
        }

        SharedMemSegment::Id segment_id()
        {
            return segment_id_;
        }

        uint32_t increase_ref()
        {
            return buffer_node_->ref_count.fetch_add(1);
        }

        uint32_t decrease_ref()
        {
            return buffer_node_->ref_count.fetch_sub(1);
        }

private:

        std::shared_ptr<SharedMemSegment> segment_;
        SharedMemSegment::Id segment_id_;
        BufferNode* buffer_node_;
    };

    class Segment
    {
    public:

        Segment(
                uint32_t size)
            : segment_id_()
        {

            auto segment_name = segment_id_.to_string();

            SharedMemSegment::remove(segment_name.c_str());

            segment_ = std::unique_ptr<SharedMemSegment>(
                new SharedMemSegment(boost::interprocess::create_only, segment_name.c_str(), size));
        }

        SharedMemSegment::Id id()
        {
            return segment_id_;
        }

        std::shared_ptr<Buffer> alloc_buffer(
                uint32_t size)
        {
            std::lock_guard<std::mutex> lock(alloc_mutex_);

            release_unused_buffers();

            void* data = nullptr;
            BufferNode* buffer_node = nullptr;
            std::shared_ptr<SharedMemBuffer> new_buffer;

            try
            {
                data = segment_->get().allocate(size);
                buffer_node = segment_->get().construct<BufferNode>(boost::interprocess::anonymous_instance)();
                buffer_node->data_offset = segment_->get().get_handle_from_address(data);
                buffer_node->data_size = size;
                buffer_node->ref_count.store(0, std::memory_order_relaxed);

                new_buffer = std::make_shared<SharedMemBuffer>(segment_, segment_id_, buffer_node);

                allocated_nodes_.push_back(buffer_node);
            }
            catch (const std::exception&)
            {
                if (buffer_node)
                {
                    release_buffer(buffer_node);
                }
                else if (data)
                {
                    segment_->get().deallocate(data);
                }

                throw;
            }

            return new_buffer;
        }

private:

        std::list<BufferNode*> allocated_nodes_;
        std::mutex alloc_mutex_;
        std::shared_ptr<SharedMemSegment> segment_;
        SharedMemSegment::Id segment_id_;

        void release_buffer(
                BufferNode* buffer_node)
        {
            segment_->get().deallocate(segment_->get_address_from_offset(buffer_node->data_offset));
            segment_->get().destroy_ptr(buffer_node);
        }

        void release_unused_buffers()
        {
            auto node_it = allocated_nodes_.begin();
            while (node_it != allocated_nodes_.end())
            {
                if ((*node_it)->ref_count.load() == 0)
                {
                    release_buffer(*node_it);

                    allocated_nodes_.erase(node_it++);
                }
                else
                {
                    node_it++;
                }
            }
        }

    }; // Segment

    class Port;
    class Listener
    {
    public:

        Listener(
                SharedMemManager& shared_mem_manager,
                std::shared_ptr<SharedMemGlobal::Port> port)
            : global_port_(port)
            , shared_mem_manager_(shared_mem_manager)
            , is_closed_(false)
        {
            global_listener_ = global_port_->create_listener();
        }

        /**
         * Extract the first buffer enqued in the port.
         * If the queue is empty, blocks until a buffer is pushed
         * to the port.
         * @remark Multithread not supported.
         */
        std::shared_ptr<Buffer> pop()
        {
            std::shared_ptr<Buffer> buffer_ref;

            SharedMemGlobal::PortCell* head_cell = nullptr;

            while ( !is_closed_.load() && !(head_cell = global_listener_->head()) )
            {
                // Wait until threre's data to pop
                global_port_->wait_pop(*global_listener_, is_closed_);
            }

            if(!head_cell)
                return nullptr;

            SharedMemGlobal::BufferDescriptor buffer_descriptor = head_cell->data();

            auto segment = shared_mem_manager_.find_segment(buffer_descriptor.source_segment_id);
            auto buffer_node =
                    static_cast<BufferNode*>(segment->get_address_from_offset(buffer_descriptor.buffer_node_offset));
            buffer_ref = std::make_shared<SharedMemBuffer>(segment, buffer_descriptor.source_segment_id, buffer_node);

            // If the cell has been read by all listeners
            bool was_cell_freed;
            if (global_port_->pop(*global_listener_, was_cell_freed))
            {
                if (was_cell_freed)
                {
                    buffer_node->ref_count.fetch_sub(1);
                }
            }

            return buffer_ref;
        }

        /**
         * Unblock a thread blocked in pop() call, not allowing pop() to block again,
         * and performs listener unregister from the port.
         */
        void close()
        {
            // Just in case a thread is blocked in pop() function
            global_port_->close_listener(&is_closed_);
        }

private:

        std::shared_ptr<SharedMemGlobal::Port> global_port_;

        std::shared_ptr<SharedMemGlobal::Listener> global_listener_;

        SharedMemManager& shared_mem_manager_;

        std::atomic<bool> is_closed_;
    }; // Listener

    class Port
    {
    public:

        Port(
                SharedMemManager& shared_mem_manager,
                std::shared_ptr<SharedMemGlobal::Port> port)
            : shared_mem_manager_(shared_mem_manager)
            , global_port_(port)
        {
        }

        void push(
                const std::shared_ptr<Buffer>& buffer)
        {
            assert(std::dynamic_pointer_cast<SharedMemBuffer>(buffer));

            SharedMemBuffer* shared_mem_buffer = std::static_pointer_cast<SharedMemBuffer>(buffer).get();
            shared_mem_buffer->increase_ref();

            if (!global_port_->push({shared_mem_buffer->segment_id(), shared_mem_buffer->node_offset()}))
            {
                shared_mem_buffer->decrease_ref();
            }
        }

        std::shared_ptr<Listener> create_listener()
        {
            return std::make_shared<Listener>(shared_mem_manager_, global_port_);
        }

private:

        SharedMemManager& shared_mem_manager_;

        std::shared_ptr<SharedMemGlobal::Port> global_port_;

    }; // Port

    std::shared_ptr<Segment> create_segment(
            uint32_t size)
    {
        return std::make_shared<Segment>(size);
    }

    std::shared_ptr<Port> open_port(
            uint32_t port_id,
            uint32_t max_descriptors)
    {
        return std::make_shared<Port>(*this, global_segment_.open_port(port_id, max_descriptors));
    }

private:

    std::unordered_map<SharedMemSegment::Id::type, std::shared_ptr<SharedMemSegment>,
            boost::hash<SharedMemSegment::Id::type> > ids_segments_;
    std::mutex ids_segments_mutex_;

    SharedMemGlobal global_segment_;

    std::shared_ptr<SharedMemSegment> find_segment(
            SharedMemSegment::Id id)
    {
        std::lock_guard<std::mutex> lock(ids_segments_mutex_);

        std::shared_ptr<SharedMemSegment> segment;

        // TODO: Garbage collector for opened but unused segments????

        try
        {
            segment = ids_segments_.at(id.get());
        }
        catch (std::out_of_range&)
        {
            segment = std::make_shared<SharedMemSegment>(boost::interprocess::open_only, id.to_string());
            ids_segments_[id.get()] = segment;
        }

        return segment;
    }
};

} // namespace rtps
} // namespace fastdds
} // namespace eprosima

#endif // _FASTDDS_SHAREDMEM_MANAGER_H_