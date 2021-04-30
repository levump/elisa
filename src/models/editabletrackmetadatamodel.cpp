/*
   SPDX-FileCopyrightText: 2020 (c) Matthieu Gallien <matthieu_gallien@yahoo.fr>

   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "editabletrackmetadatamodel.h"

#include <KI18n/KLocalizedString>

#include <QDebug>

EditableTrackMetadataModel::EditableTrackMetadataModel(QObject *parent)
    : TrackMetadataModel(parent)
{
}

QVariant EditableTrackMetadataModel::data(const QModelIndex &index, int role) const
{
    auto result = QVariant{};

    const auto currentKey = trackKey(index.row());

    switch (role)
    {
    case ReadOnlyRole:
        switch (currentKey)
        {
        case DataTypes::TitleRole:
            result = false;
            break;
        case DataTypes::ResourceRole:
            switch (allTrackData().elementType())
            {
            case ElisaUtils::Track:
                result = true;
                break;
            case ElisaUtils::Radio:
                result = false;
                break;
            case ElisaUtils::Album:
            case ElisaUtils::Artist:
            case ElisaUtils::Composer:
            case ElisaUtils::Container:
            case ElisaUtils::FileName:
            case ElisaUtils::Genre:
            case ElisaUtils::Lyricist:
            case ElisaUtils::Unknown:
                result = true;
                break;
            }
            break;
        case DataTypes::ImageUrlRole:
            result = false;
            break;
        case DataTypes::ArtistRole:
            result = false;
            break;
        case DataTypes::AlbumRole:
            result = false;
            break;
        case DataTypes::AlbumArtistRole:
            result = false;
            break;
        case DataTypes::TrackNumberRole:
            result = false;
            break;
        case DataTypes::DiscNumberRole:
            result = false;
            break;
        case DataTypes::RatingRole:
            result = false;
            break;
        case DataTypes::GenreRole:
            result = false;
            break;
        case DataTypes::LyricistRole:
            result = false;
            break;
        case DataTypes::ComposerRole:
            result = false;
            break;
        case DataTypes::CommentRole:
            result = false;
            break;
        case DataTypes::YearRole:
            result = false;
            break;
        case DataTypes::LastPlayDate:
            result = true;
            break;
        case DataTypes::PlayCounter:
            result = true;
            break;
        case DataTypes::LyricsRole:
            result = false;
            break;
        case DataTypes::DurationRole:
            result = true;
            break;
        case DataTypes::SampleRateRole:
            result = false;
            break;
        case DataTypes::BitRateRole:
            result = false;
            break;
        case DataTypes::ChannelsRole:
            result = false;
            break;
        case DataTypes::FirstPlayDate:
            result = true;
            break;
        case DataTypes::PlayFrequency:
            result = true;
            break;
        case DataTypes::SecondaryTextRole:
        case DataTypes::ShadowForImageRole:
        case DataTypes::ChildModelRole:
        case DataTypes::StringDurationRole:
        case DataTypes::IsValidAlbumArtistRole:
        case DataTypes::AllArtistsRole:
        case DataTypes::HighestTrackRating:
        case DataTypes::IdRole:
        case DataTypes::ParentIdRole:
        case DataTypes::DatabaseIdRole:
        case DataTypes::IsSingleDiscAlbumRole:
        case DataTypes::ContainerDataRole:
        case DataTypes::IsPartialDataRole:
        case DataTypes::AlbumIdRole:
        case DataTypes::HasEmbeddedCover:
        case DataTypes::FileModificationTime:
        case DataTypes::ElementTypeRole:
        case DataTypes::FullDataRole:
        case DataTypes::IsDirectoryRole:
        case DataTypes::IsPlayListRole:
        case DataTypes::FilePathRole:
        case DataTypes::HasChildrenRole:
            break;
        }
        break;
    case RemovableFieldRole:
        switch (allTrackData().elementType())
        {
        case ElisaUtils::Track:
            switch (currentKey)
            {
            case DataTypes::TitleRole:
            case DataTypes::BitRateRole:
            case DataTypes::ChannelsRole:
            case DataTypes::ArtistRole:
            case DataTypes::AlbumRole:
            case DataTypes::AlbumArtistRole:
            case DataTypes::TrackNumberRole:
            case DataTypes::DiscNumberRole:
            case DataTypes::RatingRole:
            case DataTypes::GenreRole:
            case DataTypes::LyricistRole:
            case DataTypes::ComposerRole:
            case DataTypes::CommentRole:
            case DataTypes::YearRole:
            case DataTypes::LyricsRole:
            case DataTypes::SampleRateRole:
                result = true;
                break;
            case DataTypes::LastPlayDate:
            case DataTypes::PlayCounter:
            case DataTypes::ImageUrlRole:
            case DataTypes::DurationRole:
            case DataTypes::SecondaryTextRole:
            case DataTypes::ShadowForImageRole:
            case DataTypes::ChildModelRole:
            case DataTypes::StringDurationRole:
            case DataTypes::IsValidAlbumArtistRole:
            case DataTypes::AllArtistsRole:
            case DataTypes::HighestTrackRating:
            case DataTypes::IdRole:
            case DataTypes::ParentIdRole:
            case DataTypes::DatabaseIdRole:
            case DataTypes::IsSingleDiscAlbumRole:
            case DataTypes::ContainerDataRole:
            case DataTypes::IsPartialDataRole:
            case DataTypes::AlbumIdRole:
            case DataTypes::HasEmbeddedCover:
            case DataTypes::FileModificationTime:
            case DataTypes::FirstPlayDate:
            case DataTypes::PlayFrequency:
            case DataTypes::ElementTypeRole:
            case DataTypes::FullDataRole:
            case DataTypes::IsDirectoryRole:
            case DataTypes::IsPlayListRole:
            case DataTypes::FilePathRole:
            case DataTypes::ResourceRole:
            case DataTypes::HasChildrenRole:
                result = false;
                break;
            }
            break;
        case ElisaUtils::Radio:
        case ElisaUtils::Album:
        case ElisaUtils::Artist:
        case ElisaUtils::Composer:
        case ElisaUtils::Container:
        case ElisaUtils::FileName:
        case ElisaUtils::Genre:
        case ElisaUtils::Lyricist:
        case ElisaUtils::Unknown:
            result = false;
            break;
        }
        break;
    default:
        result = TrackMetadataModel::data(index, role);
    }

    return result;
}

QHash<int, QByteArray> EditableTrackMetadataModel::roleNames() const
{
    auto names = TrackMetadataModel::roleNames();

    names[EditableColumnRoles::ReadOnlyRole] = "isReadOnly";
    names[EditableColumnRoles::RemovableFieldRole] = "isRemovable";

    return names;
}

bool EditableTrackMetadataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto result = TrackMetadataModel::setData(index, value, role);

    if (result) {
        modelHasBeenModified();
        validData();
    }

    return result;
}

void EditableTrackMetadataModel::saveData()
{
    mIsNewRadio = false;
    mIsDirty = false;
    Q_EMIT isDirtyChanged();

    auto newTrackData = allTrackData();

    newTrackData[DataTypes::ResourceRole] = QUrl::fromUserInput(newTrackData[DataTypes::ResourceRole].toString());

    QString imageUrl = dataFromType(DataTypes::ImageUrlRole).toString();
    if (!imageUrl.isEmpty()
            && !imageUrl.startsWith(QStringLiteral("http://"))
            && !imageUrl.startsWith(QStringLiteral("https://"))
            && !imageUrl.startsWith(QStringLiteral("file://"))) {
        newTrackData[DataTypes::ImageUrlRole] = QUrl::fromUserInput(QStringLiteral("file:/").append(imageUrl));
    }

    Q_EMIT saveTrackModified({newTrackData}, {});
}

void EditableTrackMetadataModel::deleteRadio()
{
    auto &track = displayedTrackData();
    if (track[DataTypes::DatabaseIdRole].toULongLong()) {
        Q_EMIT deleteRadioData(track[DataTypes::DatabaseIdRole].toULongLong());
    }
}

void EditableTrackMetadataModel::removeData(int index)
{
    beginRemoveRows({}, index, index);
    removeDataByIndex(index);
    endRemoveRows();

    modelHasBeenModified();

    validData();

    updateExtraMetadata();
}

void EditableTrackMetadataModel::addData(const QString &name)
{
    beginInsertRows({}, rowCount(), rowCount());
    addDataByName(name);
    endInsertRows();

    modelHasBeenModified();
    validData();

    updateExtraMetadata();
}

void EditableTrackMetadataModel::fillDataFromTrackData(const TrackMetadataModel::TrackDataType &trackData,
                                                       const QList<DataTypes::ColumnsRoles> &fieldsForTrack)
{
    if (mIsNewRadio) {
        return;
    }

    TrackMetadataModel::fillDataFromTrackData(trackData, fieldsForTrack);

    updateExtraMetadata();
}

void EditableTrackMetadataModel::filterDataFromTrackData()
{
    TrackMetadataModel::filterDataFromTrackData();
    validData();
}

void EditableTrackMetadataModel::fillLyricsDataFromTrack()
{
    TrackMetadataModel::fillLyricsDataFromTrack();
    validData();

    updateExtraMetadata();
}

void EditableTrackMetadataModel::fillDataForNewRadio()
{
    mIsNewRadio = true;
    TrackMetadataModel::fillDataForNewRadio();
}

void EditableTrackMetadataModel::initialize(MusicListenersManager *newManager, DatabaseInterface *trackDatabase)
{
    TrackMetadataModel::initialize(newManager, trackDatabase);

    connect(this, &EditableTrackMetadataModel::saveTrackModified,
            &modelDataLoader(), &ModelDataLoader::trackHasBeenModified);
    connect(this, &EditableTrackMetadataModel::deleteRadioData,
            &modelDataLoader(), &ModelDataLoader::removeRadio);
}

void EditableTrackMetadataModel::validData()
{
    bool newValidState = true;

    const auto &resourceData = dataFromType(TrackDataType::key_type::ResourceRole);
    if (resourceData.canConvert<QUrl>()) {
        const auto resourceUrl = resourceData.toUrl();

        newValidState = !resourceUrl.scheme().isEmpty() && resourceUrl.isValid() && !resourceUrl.isRelative();
    } else {
        newValidState = false;
    }

    if (!newValidState) {
        mErrorMessage = i18nc("Error message when track URL is invalid", "Invalid URL.");
        Q_EMIT errorMessageChanged();
    }

    if (newValidState) {
        const auto &titleData = dataFromType(TrackDataType::key_type::TitleRole);

        newValidState = newValidState && !titleData.toString().isEmpty();

        if (!newValidState) {
            mErrorMessage = i18nc("Error message when track title is empty", "Empty title.");
            Q_EMIT errorMessageChanged();
        }
    }

    if (mIsDataValid != newValidState) {
        mIsDataValid = newValidState;

        Q_EMIT isDataValidChanged();
    }
}

void EditableTrackMetadataModel::updateExtraMetadata()
{
    mExtraMetadata.clear();
    for(auto metadataRole : {DataTypes::TitleRole, DataTypes::ArtistRole,
                             DataTypes::AlbumRole, DataTypes::AlbumArtistRole, DataTypes::TrackNumberRole,
                             DataTypes::DiscNumberRole, DataTypes::RatingRole, DataTypes::GenreRole,
                             DataTypes::LyricistRole, DataTypes::ComposerRole, DataTypes::CommentRole,
                             DataTypes::YearRole, DataTypes::ChannelsRole, DataTypes::BitRateRole,
                             DataTypes::SampleRateRole, DataTypes::LyricsRole}) {
        if (!metadataExists(metadataRole)) {
            mExtraMetadata.push_back(nameFromRole(metadataRole));
        }
    }

    Q_EMIT extraMetadataChanged();
}

void EditableTrackMetadataModel::modelHasBeenModified()
{
    if (!mIsDirty) {
        mIsDirty = true;
        Q_EMIT isDirtyChanged();
    }
}


#include "moc_editabletrackmetadatamodel.cpp"
