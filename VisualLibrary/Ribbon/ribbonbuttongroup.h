/*
 * Copyright (C) Martijn Koopman
 * All Rights Reserved
 *
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 */

#ifndef RIBBONBUTTONGROUP_H
#define RIBBONBUTTONGROUP_H

#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QToolButton>

#include "visuallibrary_global.h"

namespace Ui {
class RibbonButtonGroup;
}

class VISUALLIBRARY_EXPORT RibbonButtonGroup : public QWidget
{
	Q_OBJECT

public:
	explicit RibbonButtonGroup(QWidget* parent = 0);
	virtual ~RibbonButtonGroup();

	/// Set the title of the button group.
	/// The title is shown underneath the buttons.
	///
	/// \param[in] title The title
	void setTitle(const QString& title);

	/// Get the title of the button group.
	///
	/// \return The title
	QString title() const;

	/// Get the number of buttons in the button group.
	///
	/// \return The number of buttons
	int buttonCount() const;

	/// Add a button to the group.
	///
	/// \param[in] button The button
	void addButton(QToolButton* button);

	/// Remove a button from the group.
	///
	/// \param[in] button The button
	void removeButton(QToolButton* button);

private:
	Ui::RibbonButtonGroup* ui;
	QString m_title; ///< Title of the button group
};

#endif // RIBBONBUTTONGROUP_H
