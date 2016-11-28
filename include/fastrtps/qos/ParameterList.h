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
 * @file ParameterList.h
 */

#ifndef PARAM_LIST_T_H_
#define PARAM_LIST_T_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC
#include "ParameterTypes.h"
#include "../rtps/messages/RTPS_messages.h"
#include "../rtps/common/CDRMessage_t.h"
#include "../rtps/messages/CDRMessage.h"
#include "../rtps/common/CacheChange.h"

namespace eprosima {
namespace fastrtps {

/**
 * ParameterList_t class, used to store multiple parameters as a vector of pointers to the base class.
 * @ingroup PARAMETER_MODULE
 */
class ParameterList_t {
public:
	ParameterList_t():m_cdrmsg(RTPSMESSAGE_DEFAULT_SIZE),m_hasChanged(true){};
	virtual ~ParameterList_t()
	{

	}
	/**
	 * Delete all parameters in the list.
	 */
	void deleteParams()
	{
		for(std::vector<Parameter_t*>::iterator it = m_parameters.begin();
				it!=m_parameters.end();++it)
			delete(*it);
		resetList();
	}
	/**
	 * Reset the list of parameters (without deleting them, since they can be in use in another list).
	 */
	void resetList(){
		m_parameters.clear();
		CDRMessage::initCDRMsg(&m_cdrmsg);
		m_hasChanged = true;
	}
	//! Vector of the pointers to the parameters.
	std::vector<Parameter_t*> m_parameters;
	//! Associated CDRMessage_t message.
	CDRMessage_t m_cdrmsg;
	//! Bool variable to indicate whether a new parameter has been added.
	bool m_hasChanged;
};

/**
 * ParameterList class has static methods to update or read a ParameterList_t
 * @ingroup PARAMETER_MODULE
 */

class ParameterList
{
public:
	/**
	 * Update the CDRMessage of a parameterList.
	 * @param plist Pointer to the parameterList.
	 * @param endian Endianness that the resulting CDRMessage_t should have.
	 * @return True if correct.
	 */
	static bool updateCDRMsg(ParameterList_t* plist,Endianness_t endian);
	/**
	 * Read a parameterList from a CDRMessage
	 * @param[in] msg Pointer to the message (the pos should be correct, otherwise the behaviour is undefined).
	 * @param[out] plist Pointer to the parameter list.
	 * @param[out] change Pointer to the cache change.
	 * @return Number of bytes of the parameter list.
	 */
	static int32_t readParameterListfromCDRMsg(CDRMessage_t* msg, ParameterList_t* plist, CacheChange_t* change);

};

} /* namespace  */
} /* namespace eprosima */
#endif
#endif /* PARAMLISTT_H_ */
