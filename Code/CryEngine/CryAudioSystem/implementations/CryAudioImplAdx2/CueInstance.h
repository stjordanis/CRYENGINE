// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <PoolObject.h>
#include <atomic>
#include <cri_atom_ex.h>

namespace CryAudio
{
namespace Impl
{
namespace Adx2
{
class CBaseObject;
class CCue;

enum class ECueInstanceFlags : EnumFlagsType
{
	None                = 0,
	IsVirtual           = BIT(0),
	IsPending           = BIT(1),
	HasAbsoluteVelocity = BIT(2),
	HasDoppler          = BIT(3),
	ToBeRemoved         = BIT(4),
#if defined(CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE)
	IsStopping          = BIT(5),
#endif  // CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE
};
CRY_CREATE_ENUM_FLAG_OPERATORS(ECueInstanceFlags);

class CCueInstance final : public CPoolObject<CCueInstance, stl::PSyncNone>
{
public:

	CCueInstance() = delete;
	CCueInstance(CCueInstance const&) = delete;
	CCueInstance(CCueInstance&&) = delete;
	CCueInstance& operator=(CCueInstance const&) = delete;
	CCueInstance& operator=(CCueInstance&&) = delete;

#if defined(CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE)
	explicit CCueInstance(
		TriggerInstanceId const triggerInstanceId,
		CriAtomExPlaybackId const playbackId,
		CCue& cue,
		CBaseObject const& baseObject)
		: m_triggerInstanceId(triggerInstanceId)
		, m_playbackId(playbackId)
		, m_cue(cue)
		, m_flags(ECueInstanceFlags::IsVirtual | ECueInstanceFlags::IsPending) // Voices always start in virtual state.
		, m_timeFadeOutStarted(0.0f)
		, m_baseObject(baseObject)
	{}
#else
	explicit CCueInstance(
		TriggerInstanceId const triggerInstanceId,
		CriAtomExPlaybackId const playbackId,
		CCue& cue)
		: m_triggerInstanceId(triggerInstanceId)
		, m_playbackId(playbackId)
		, m_cue(cue)
		, m_flags(ECueInstanceFlags::None)
	{}
#endif  // CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE

	~CCueInstance() = default;

	TriggerInstanceId   GetTriggerInstanceId() const             { return m_triggerInstanceId; }

	CCue&               GetCue() const                           { return m_cue; }
	CriAtomExPlaybackId GetPlaybackId() const                    { return m_playbackId; }

	ECueInstanceFlags   GetFlags() const                         { return m_flags; }
	void                SetFlag(ECueInstanceFlags const flag)    { m_flags = m_flags | flag; }
	void                RemoveFlag(ECueInstanceFlags const flag) { m_flags = m_flags & ~flag; }

	bool                PrepareForPlayback(CBaseObject& baseObject);

	void                Stop();
	void                Pause();
	void                Resume();

#if defined(CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE)
	void               StartFadeOut();
	float              GetTimeFadeOutStarted() const { return m_timeFadeOutStarted; }
	CBaseObject const& GetObject() const             { return m_baseObject; }
#endif  // CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE

private:

	TriggerInstanceId const        m_triggerInstanceId;
	CriAtomExPlaybackId const      m_playbackId;
	std::atomic<ECueInstanceFlags> m_flags;
	CCue&                          m_cue;

#if defined(CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE)
	float              m_timeFadeOutStarted;
	CBaseObject const& m_baseObject;
#endif  // CRY_AUDIO_IMPL_ADX2_USE_PRODUCTION_CODE
};
} // namespace Adx2
} // namespace Impl
} // namespace CryAudio
