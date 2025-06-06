/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#pragma once

#include <QColor>
#include <QString>
#include <QWidget>
#include <QStringList>
#include <QFileInfoList>
#include <filesystem>

namespace fs = std::filesystem;

namespace UIUtils {

struct ExampleCategory
{
  constexpr static int DEFAULT_SORT = 10000;

  int sort = DEFAULT_SORT;
  QString name;
  QString tooltip;
};

struct ExampleEntry
{
  constexpr static int DEFAULT_SORT = 10000;

  int sort = DEFAULT_SORT;
  QString name;
  QFileInfo fileInfo;
};

static const int maxRecentFiles = 10;

QFileInfo openFile(QWidget *parent = nullptr, QStringList extensions = {});

QFileInfoList openFiles(QWidget *parent = nullptr, QStringList extensions = {});

QStringList recentFiles();

const QList<UIUtils::ExampleCategory>& exampleCategories();

QFileInfoList exampleFiles(const QString& category);

void openURL(const QString& url);

void openHomepageURL();

void openUserManualURL();

fs::path returnOfflineUserManualPath();

bool hasOfflineUserManual();

void openOfflineUserManual();

void openCheatSheetURL();

fs::path returnOfflineCheatSheetPath();

bool hasOfflineCheatSheet();

void openOfflineCheatSheet();

QString getBackgroundColorStyleSheet(const QColor &color);

QString blendForBackgroundColorStyleSheet(const QColor& input, const QColor& blend, float transparency = 0.2);

} // namespace UIUtils
