#include "playlistmodel.h"
#include <QAbstractItemModel>
#include <QList>
#include <QStringList>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QLinkedList>
#include <QMimeData>
#include <QMimeType>
#include <QBrush>
#include "mediaitem.h"


PlaylistModel::PlaylistModel(QList<MediaItem> *playlist, QObject * parent):
    QAbstractListModel(parent)
{
    m_container = playlist;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return m_container->size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if((role == Qt::DisplayRole) && index.isValid())
    {
        return m_container->at(index.row()).fileName;
    }
    if((role == Qt::BackgroundColorRole) && index.isValid())
    {
        if(m_container->at(index.row()).isPlaying)
            return QBrush(Qt::gray);
    }
    if((role == Qt::ToolTipRole) && index.isValid())
    {
        return m_container->at(index.row()).fileName;
    }


    return QVariant();
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    return tr("Заголовок");
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    if(!index.isValid())
        return flags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
          | flags;
}

bool PlaylistModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row >= 0 && (row + count) <= m_container->size() && count > 0)
    {
        for(int i = 0; i < count; i++)
        {
            m_container->removeAt(row);
        }
        return true;
    }
    return false;
}

bool PlaylistModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row >= 0 && row <= m_container->size())
    {
        for(int i = 0; i < count; i++)
        {
            m_container->insert(row + i, MediaItem());
        }
        return true;
    }
    return false;
}

void PlaylistModel::onDelButton(QItemSelectionModel* selectionModel)
{
    QModelIndexList indexList = selectionModel->selectedRows();
    if(indexList.isEmpty())
        return;
    QModelIndexList::const_reverse_iterator rit = indexList.rbegin();
    while(rit != indexList.rend())
    {
        int rowNum = (*rit++).row();
        beginRemoveRows(QModelIndex(), rowNum, rowNum);
        removeRow(rowNum);
    }
    endRemoveRows();
    selectionModel->select(QModelIndex(), QItemSelectionModel::Clear);
}

void PlaylistModel::clearContainer()
{
    if(m_container->size() == 0)
    {
        return;
    }
    beginRemoveRows(QModelIndex(), 0, m_container->size() - 1);
    m_container->clear();
    endRemoveRows();
}

void PlaylistModel::appendItems(QList<MediaItem> &items)
{
    if(items.size() == 0)
    {
        return;
    }
    beginInsertRows(QModelIndex(), m_container->size(), m_container->size() + items.size() - 1);
    m_container->append(items);
    endInsertRows();
}

QMimeData* PlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    Q_ASSERT(indexes.count());
    if(indexes.count() != 1)
    {
        return 0;
    }
    QMimeData *mimeData = new QMimeData;
    QByteArray binaryData;
    MediaItem item = m_container->at(indexes.at(0).row());
    binaryData = item.serialize();
    mimeData->setData(MimeType, binaryData);
    return mimeData;
}

bool PlaylistModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    int insertRow;
    if(action == Qt::IgnoreAction)
        return true;
    if(action != Qt::MoveAction || !data || !data->hasFormat(MimeType))
        return false;
    if(row != -1)
    {
        insertRow = row;
    }
    else if(parent.isValid())
    {
        insertRow = parent.row();
    }
    else
    {
        return false;
    }
    QByteArray binData = data->data(MimeType);
    MediaItem item = MediaItem::deserialize(binData);
    beginInsertRows(parent, insertRow, insertRow);
    m_container->insert(insertRow, item);
    endInsertRows();
    return true;
}

void PlaylistModel::changedData(int top, int bottom)
{
    emit dataChanged(index(top), index(bottom));
}
