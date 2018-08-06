// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Controls/EditorDialog.h>

#include <FileImportInfo.h>
#include <QDir>

class QAttributeFilterProxyModel;
class QLineEdit;

namespace ACE
{
class CTreeView;
class CFileImporterModel;

class CFileImporterDialog final : public CEditorDialog
{
	Q_OBJECT

public:

	CFileImporterDialog() = delete;

	explicit CFileImporterDialog(
		FileImportInfos const& fileInfos,
		QString const& assetsPath,
		QString const& localizedAssetsPath,
		QString const& targetPath,
		QString const& targetFolderName,
		bool const isLocalized,
		QWidget* const pParent);

	// QDialog
	virtual QSize sizeHint() const override;
	// ~QDialog

private:

	void UpdateTargetPath(QString const& targetFolderName);
	void OnCreateFolderSelector();
	void OnActionChanged(Qt::CheckState const isChecked);
	void OnSetImportAll();
	void OnSetIgnoreAll();
	void OnApplyImport();

	CTreeView* const                  m_pTreeView;
	CFileImporterModel* const         m_pFileImporterModel;
	QAttributeFilterProxyModel* const m_pAttributeFilterProxyModel;
	QLineEdit* const                  m_pTargetDirLineEdit;
	FileImportInfos                   m_fileImportInfos;
	QString const                     m_assetsPath;
	QString const                     m_localizedAssetsPath;
	QString                           m_targetPath;
	QString                           m_targetFolderName;
	QDir const                        m_gameFolder;
	bool                              m_isLocalized;
};
} // namespace ACE
