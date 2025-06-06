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

#include "gui/FontListDialog.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QItemSelection>
#include <QModelIndex>
#include <QClipboard>
#include <QSortFilterProxyModel>

#include "FontCache.h"

FontListDialog::FontListDialog()
{
  model = nullptr;
  proxy = nullptr;
  setupUi(this);
  connect(this->okButton, &QPushButton::clicked, this, &FontListDialog::accept);
}

void FontListDialog::on_copyButton_clicked()
{
  font_selected(selection);

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(selection);
}

void FontListDialog::on_filterLineEdit_textChanged(const QString& text)
{
  proxy->setFilterWildcard(text);
}

void FontListDialog::selection_changed(const QItemSelection& current, const QItemSelection&)
{
  if (current.count() == 0) {
    copyButton->setEnabled(false);
    tableView->setDragText("");
    return;
  }

  const QModelIndex& idx = proxy->mapToSource(current.indexes().at(0));
  const QString name = model->item(idx.row(), 0)->text();
  const QString style = model->item(idx.row(), 1)->text();
  selection = QString("\"%1:style=%2\"").arg(quote(name)).arg(quote(style));
  copyButton->setEnabled(true);
  tableView->setDragText(selection);
}

void FontListDialog::updateFontList()
{
  copyButton->setEnabled(false);

  if (proxy) {
    delete proxy;
    proxy = nullptr;
  }
  if (model) {
    delete model;
    model = nullptr;
  }

  FontInfoList *list = FontCache::instance()->list_fonts();
  model = new QStandardItemModel(list->size(), 3, this);
  model->setHorizontalHeaderItem(0, new QStandardItem(_("Font name")));
  model->setHorizontalHeaderItem(1, new QStandardItem(_("Font style")));
  model->setHorizontalHeaderItem(2, new QStandardItem(_("Filename")));

  int idx = 0;
  for (auto it = list->begin(); it != list->end(); it++, idx++) {
    const FontInfo& font_info = (*it);
    auto *family = new QStandardItem(QString(font_info.get_family().c_str()));
    family->setEditable(false);
    model->setItem(idx, 0, family);
    auto *style = new QStandardItem(QString(font_info.get_style().c_str()));
    style->setEditable(false);
    model->setItem(idx, 1, style);
    auto *file = new QStandardItem(QString(font_info.get_file().c_str()));
    file->setEditable(false);
    model->setItem(idx, 2, file);
  }

  proxy = new QSortFilterProxyModel(this);
  proxy->setSourceModel(model);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  this->tableView->setModel(proxy);
  this->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  this->tableView->sortByColumn(0, Qt::AscendingOrder);
  this->tableView->resizeColumnsToContents();
  this->tableView->setSortingEnabled(true);

  connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FontListDialog::selection_changed);

  delete list;
}

/**
 * Quote a string according to the requirements of font-config.
 * See http://www.freedesktop.org/software/fontconfig/fontconfig-user.html
 *
 * The '\', '-', ':' and ',' characters in family names must be preceded
 * by a '\' character to avoid having them misinterpreted. Similarly, values
 * containing '\', '=', '_', ':' and ',' must also have them preceded by a
 * '\' character. The '\' characters are stripped out of the family name and
 * values as the font name is read.
 *
 * @param text unquoted string
 * @return quoted text
 */
QString FontListDialog::quote(const QString& text)
{
  QString result = text;
  result.replace('\\', R"(\\\\)")
  .replace('-', "\\\\-")
  .replace(':', "\\\\:")
  .replace(',', "\\\\,")
  .replace('=', "\\\\=")
  .replace('_', "\\\\_");
  return result;
}
