// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#include "stdafx.h"
#include "SwitchState.h"
#include "BaseObject.h"
#include "Event.h"
#include "Trigger.h"

#if defined(INCLUDE_FMOD_IMPL_PRODUCTION_CODE)
	#include <Logger.h>
#endif  // INCLUDE_FMOD_IMPL_PRODUCTION_CODE

namespace CryAudio
{
namespace Impl
{
namespace Fmod
{
//////////////////////////////////////////////////////////////////////////
void CSwitchState::Set(IObject* const pIObject)
{
	if (pIObject != nullptr)
	{
		FMOD_RESULT fmodResult = FMOD_ERR_UNINITIALIZED;
		auto const pObject = static_cast<CBaseObject*>(pIObject);
		Events const& objectEvents = pObject->GetEvents();

		for (auto const pEvent : objectEvents)
		{
			FMOD::Studio::EventInstance* const pEventInstance = pEvent->GetInstance();
			CRY_ASSERT_MESSAGE(pEventInstance != nullptr, "Event instance doesn't exist during %s", __FUNCTION__);
			CTrigger const* const pTrigger = pEvent->GetTrigger();
			CRY_ASSERT_MESSAGE(pTrigger != nullptr, "Trigger doesn't exist during %s", __FUNCTION__);

			FMOD::Studio::EventDescription* pEventDescription = nullptr;
			fmodResult = pEventInstance->getDescription(&pEventDescription);
			ASSERT_FMOD_OK;

			if (g_triggerToParameterIndexes.find(pTrigger) != g_triggerToParameterIndexes.end())
			{
				ParameterIdToIndex& parameters = g_triggerToParameterIndexes[pTrigger];

				if (parameters.find(m_id) != parameters.end())
				{
					fmodResult = pEventInstance->setParameterValueByIndex(parameters[m_id], m_value);
					ASSERT_FMOD_OK;
				}
				else
				{
					int parameterCount = 0;
					fmodResult = pEventInstance->getParameterCount(&parameterCount);
					ASSERT_FMOD_OK;

					for (int index = 0; index < parameterCount; ++index)
					{
						FMOD_STUDIO_PARAMETER_DESCRIPTION parameterDescription;
						fmodResult = pEventDescription->getParameterByIndex(index, &parameterDescription);
						ASSERT_FMOD_OK;

						if (m_id == StringToId(parameterDescription.name))
						{
							parameters.emplace(m_id, index);
							fmodResult = pEventInstance->setParameterValueByIndex(index, m_value);
							ASSERT_FMOD_OK;
							break;
						}
					}
				}
			}
			else
			{
				int parameterCount = 0;
				fmodResult = pEventInstance->getParameterCount(&parameterCount);
				ASSERT_FMOD_OK;

				for (int index = 0; index < parameterCount; ++index)
				{
					FMOD_STUDIO_PARAMETER_DESCRIPTION parameterDescription;
					fmodResult = pEventDescription->getParameterByIndex(index, &parameterDescription);
					ASSERT_FMOD_OK;

					if (m_id == StringToId(parameterDescription.name))
					{
						g_triggerToParameterIndexes[pTrigger].emplace(std::make_pair(m_id, index));
						fmodResult = pEventInstance->setParameterValueByIndex(index, m_value);
						ASSERT_FMOD_OK;
						break;
					}
				}
			}
		}

		Switches& objectSwitches = pObject->GetSwitches();

		auto const iter(objectSwitches.find(m_id));

		if (iter != objectSwitches.end())
		{
			iter->second = this;
		}
		else
		{
			objectSwitches.emplace(m_id, this);
		}
	}
#if defined(INCLUDE_FMOD_IMPL_PRODUCTION_CODE)
	else
	{
		Cry::Audio::Log(ELogType::Error, "Invalid object pointer passed to the Fmod implementation of %s", __FUNCTION__);
	}
#endif  // INCLUDE_FMOD_IMPL_PRODUCTION_CODE
}

//////////////////////////////////////////////////////////////////////////
void CSwitchState::SetGlobally()
{
	for (auto const pObject : g_constructedObjects)
	{
		Set(pObject);
	}
}
} // namespace Fmod
} // namespace Impl
} // namespace CryAudio
