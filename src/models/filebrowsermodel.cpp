/*
   SPDX-FileCopyrightText: 2018 (c) Alexander Stippich <a.stippich@gmx.net>

   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "filebrowsermodel.h"

#include <QUrl>
#include <QString>
#include <QMimeDatabase>
#include <KIOWidgets/KDirLister>

FileBrowserModel::FileBrowserModel(QObject *parent) : KDirModel(parent)
{
    QMimeDatabase db;
    const QList<QMimeType> mimeList = db.allMimeTypes();
    QStringList mimeTypes = { QStringLiteral("inode/directory") };
    for (const QMimeType &mime : mimeList) {
        if (mime.name().startsWith(QLatin1String("audio/"))) {
            mimeTypes << mime.name();
        }
    }

    dirLister()->setMimeFilter(mimeTypes);
}

FileBrowserModel::~FileBrowserModel()
= default;

QString FileBrowserModel::url() const
{
    return dirLister()->url().toString();
}

void FileBrowserModel::setUrl(const QString &url)
{
    QString path = QUrl(url).path();
    path = QUrl::fromLocalFile(path).toString();

    if (dirLister()->url().path() == QUrl(path).path()) {
        dirLister()->updateDirectory(QUrl(path));
        return;
    }

    beginResetModel();
    dirLister()->openUrl(QUrl(path));

    endResetModel();
    emit urlChanged();
}

bool FileBrowserModel::isBusy() const
{
    return false;
}

QHash<int, QByteArray> FileBrowserModel::roleNames() const
{
    auto roles = KDirModel::roleNames();

    roles[static_cast<int>(ColumnsRoles::NameRole)] = "name";
    roles[static_cast<int>(ColumnsRoles::FileUrlRole)] = "fileUrl";
    roles[static_cast<int>(ColumnsRoles::ImageUrlRole)] = "imageUrl";
    roles[static_cast<int>(ColumnsRoles::IsDirectoryRole)] = "isDirectory";
    roles[static_cast<int>(ColumnsRoles::IsPlayListRole)] = "isPlaylist";

    return roles;
}

QVariant FileBrowserModel::data(const QModelIndex &index, int role) const
{
    auto result = QVariant();

    if (role < ColumnsRoles::NameRole) {
        result = KDirModel::data(index,role);
    }

    switch(role)
    {
    case ColumnsRoles::NameRole:
    {
        KFileItem item = itemForIndex(index);
        result = item.name();
        break;
    }
    case ColumnsRoles::FileUrlRole:
    {
        KFileItem item = itemForIndex(index);
        result = item.url();
        break;
    }
    case ColumnsRoles::ImageUrlRole:
    {
        KFileItem item = itemForIndex(index);
        if (item.isDir()) {
            result = QUrl(QStringLiteral("image://icon/folder"));
        } else {
            result = QUrl(QStringLiteral("image://icon/audio-x-generic"));
        }
        break;
    }
    case ColumnsRoles::IsDirectoryRole:
    {
        KFileItem item = itemForIndex(index);
        result = item.isDir();
        break;
    }
    case ColumnsRoles::IsPlayListRole:
    {
        KFileItem item = itemForIndex(index);
        result = (item.currentMimeType().inherits(QStringLiteral("audio/x-mpegurl")));
        break;
    }
    }

    return result;
}

void FileBrowserModel::initialize(MusicListenersManager *manager, DatabaseInterface *database,
                                  ElisaUtils::PlayListEntryType modelType, ElisaUtils::FilterType filter,
                                  const QString &genre, const QString &artist, qulonglong databaseId)
{
}


#include "moc_filebrowsermodel.cpp"
