// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file ReaderHistory.h
 *
 */

#ifndef _FASTDDS_RTPS_READERHISTORY_H_
#define _FASTDDS_RTPS_READERHISTORY_H_

#include <fastdds/rtps/history/History.h>
#include <fastdds/rtps/common/CacheChange.h>

namespace eprosima {
namespace fastrtps {
namespace rtps {

class WriterProxy;
class RTPSReader;

/**
 * Class ReaderHistory, container of the different CacheChanges of a reader
 * @ingroup READER_MODULE
 */
class ReaderHistory : public History
{
    friend class RTPSReader;

    ReaderHistory(
            ReaderHistory&&) = delete;
    ReaderHistory& operator =(
            ReaderHistory&&) = delete;

public:

    /**
     * Constructor of the ReaderHistory. It needs a HistoryAttributes.
     */
    RTPS_DllAPI ReaderHistory(
            const HistoryAttributes& att);
    RTPS_DllAPI virtual ~ReaderHistory() override;

    /**
     * Virtual method that is called when a new change is received.
     * In this implementation this method just calls add_change. The suer can overload this method in case
     * he needs to perform additional checks before adding the change.
     * @param change Pointer to the change
     * @return True if added.
     */
    RTPS_DllAPI virtual bool received_change(CacheChange_t* change, size_t);

    /**
     * Add a CacheChange_t to the ReaderHistory.
     * @param a_change Pointer to the CacheChange to add.
     * @return True if added.
     */
    RTPS_DllAPI bool add_change(
            CacheChange_t* a_change);

    /**
     * Remove a CacheChange_t from the ReaderHistory.
     * @param a_change Pointer to the CacheChange to remove.
     * @return True if removed.
     */
    RTPS_DllAPI bool remove_change(
            CacheChange_t* a_change) override;

    /**
     * Remove a specific change from the history.
     * @param ch Pointer to the CacheChange_t.
     * @param position Iterator where the CacheChange_t is located in the history.
     * @return An iterator pointing to the new location of the element that followed the removed CacheChange_t.
     */
    const_iterator remove_change_nts(
            CacheChange_t* ch,
            const_iterator position);

    /**
     * Remove all changes from the History that have a certain guid.
     * @param a_guid Pointer to the target guid to search for.
     * @return True if succesful, even if no changes have been removed.
     * */
    RTPS_DllAPI bool remove_changes_with_guid(
            const GUID_t& a_guid);

    /**
     * Remove all fragmented changes from certain writer up to certain sequence number.
     * @param seq_num First SequenceNumber_t not to be removed.
     * @param writer_guid GUID of the writer for which changes should be looked for.
     * @return True if succesful, even if no changes have been removed.
     */
    bool remove_fragmented_changes_until(
            const SequenceNumber_t& seq_num,
            const GUID_t& writer_guid);

    /**
     * Update the maximum and minimum sequenceNumber cacheChanges.
     */
    RTPS_DllAPI void updateMaxMinSeqNum() override;

    RTPS_DllAPI bool get_min_change_from(
            CacheChange_t** min_change,
            const GUID_t& writerGuid);

protected:

    //!Pointer to the reader
    RTPSReader* mp_reader;
};

}
} /* namespace rtps */
} /* namespace eprosima */

#endif /* _FASTDDS_RTPS_READERHISTORY_H_ */
