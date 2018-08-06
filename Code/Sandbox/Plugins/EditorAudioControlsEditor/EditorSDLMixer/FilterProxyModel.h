// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <ProxyModels/AttributeFilterProxyModel.h>

namespace ACE
{
namespace Impl
{
namespace SDLMixer
{
class CFilterProxyModel final : public QAttributeFilterProxyModel
{
public:

	CFilterProxyModel() = delete;

	explicit CFilterProxyModel(QObject* const pParent);

private:

	// QAttributeFilterProxyModel
	virtual bool rowMatchesFilter(int sourceRow, QModelIndex const& sourcePparent) const override;
	// ~QAttributeFilterProxyModel

	// QSortFilterProxyModel
	virtual bool lessThan(QModelIndex const& left, QModelIndex const& right) const override;
	// ~QSortFilterProxyModel
};
} // namespace SDLMixer
} // namespace Impl
} // namespace ACE
