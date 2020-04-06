/*
 * Copyright 2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "elisaLib_export.h"

#include "elisautils.h"

#include <QObject>
#include <QUrl>

#include <memory>

class ViewManagerPrivate;
class ViewParameters;
class ViewsListData;

class ELISALIB_EXPORT ViewManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int viewIndex
               READ viewIndex
               NOTIFY viewIndexChanged)

    Q_PROPERTY(ViewsListData* viewsData READ viewsData WRITE setViewsData NOTIFY viewsDataChanged)

public:

    enum SortOrder {
        NoSort,
        SortAscending,
        SortDescending,
    };

    Q_ENUM(SortOrder)

    enum AlbumCardinality {
        SingleAlbum = true,
        MultipleAlbum = false,
    };

    Q_ENUM(AlbumCardinality)

    enum AlbumViewStyle {
        NoDiscHeaders,
        DiscHeaders,
    };

    Q_ENUM(AlbumViewStyle)

    enum RadioSpecificStyle {
        IsTrack,
        IsRadio,
    };

    Q_ENUM(RadioSpecificStyle)

    enum DelegateUseSecondaryText {
        DelegateWithSecondaryText = true,
        DelegateWithoutSecondaryText = false,
    };

    Q_ENUM(DelegateUseSecondaryText)

    enum ViewCanBeRated {
        ViewShowRating = true,
        ViewHideRating = false,
    };

    Q_ENUM(ViewCanBeRated)

    enum ViewPresentationType {
        ContextView,
        GridView,
        ListView,
        FileBrowserView,
        UnknownViewPresentation,
    };

    Q_ENUM(ViewPresentationType)

    explicit ViewManager(QObject *parent = nullptr);

    ~ViewManager() override;

    int viewIndex() const;

    ViewsListData* viewsData() const;

Q_SIGNALS:

    void openGridView(ElisaUtils::FilterType filterType, int expectedDepth,
                      const QString &mainTitle, const QString &secondaryTitle, const QUrl &imageUrl,
                      ElisaUtils::PlayListEntryType dataType, const QUrl &viewDefaultIcon,
                      const QString &genreNameFilter, const QString &artistNameFilter,
                      ViewManager::ViewCanBeRated viewShowRating,
                      ViewManager::DelegateUseSecondaryText viewDelegateDisplaySecondaryText);

    void openListView(ElisaUtils::FilterType filterType, int expectedDepth,
                      const QString &mainTitle, const QString &secondaryTitle, qulonglong databaseId,
                      const QUrl &imageUrl, ElisaUtils::PlayListEntryType dataType, const QVariant &sortRole,
                      ViewManager::SortOrder sortOrder, ViewManager::AlbumCardinality displaySingleAlbum,
                      ViewManager::AlbumViewStyle showDiscHeaders, ViewManager::RadioSpecificStyle radioCase);

    void switchFilesBrowserView(int expectedDepth, const QString &mainTitle, const QUrl &imageUrl);

    void switchContextView(int expectedDepth, const QString &mainTitle, const QUrl &imageUrl);

    void popOneView();

    void viewIndexChanged();

    void viewsDataChanged();

public Q_SLOTS:

    void openView(int viewIndex);

    void openChildView(const QString &innerMainTitle, const QString & innerSecondaryTitle,
                       const QUrl &innerImage, qulonglong databaseId,
                       ElisaUtils::PlayListEntryType dataType);

    void viewIsLoaded();

    void goBack();

    void setViewsData(ViewsListData* viewsData);

private:

    void openViewFromData(const ViewParameters &viewParamaters);

private:

    std::unique_ptr<ViewManagerPrivate> d;

};

#endif // VIEWMANAGER_H
