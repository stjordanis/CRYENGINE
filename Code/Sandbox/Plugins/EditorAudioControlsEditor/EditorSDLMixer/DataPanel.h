// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <QWidget>
#include <SharedData.h>

class QFilteringPanel;
class QPushButton;

namespace ACE
{
namespace Impl
{
namespace SDLMixer
{
class CFilterProxyModel;
class CItemModel;
class CTreeView;
class CImpl;

class CDataPanel final : public QWidget
{
public:

	CDataPanel() = delete;

	explicit CDataPanel(CImpl const& impl);

	void Reset();
	void OnAboutToReload();
	void OnReloaded();
	void OnConnectionAdded() const;
	void OnConnectionRemoved() const;
	void OnSelectConnectedItem(ControlId const id);
	void OnFileImporterOpened();
	void OnFileImporterClosed();

private:

	void OnContextMenu(QPoint const& pos);
	void OnImportFiles();
	void ClearFilters();

	CImpl const&             m_impl;
	CFilterProxyModel* const m_pFilterProxyModel;
	CItemModel* const        m_pModel;
	CTreeView* const         m_pTreeView;
	QPushButton* const       m_pImportButton;
	QFilteringPanel*         m_pFilteringPanel;
	int const                m_nameColumn;
};
} // namespace SDLMixer
} // namespace Impl
} // namespace ACE
